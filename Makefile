MCU = atmega328p
CC = avr-gcc
OBJCOPY = avr-objcopy
AVRDUDE = avrdude
PORT = COM5
BAUD = 115200

# FreeRTOS paths
FREERTOS_PATH = C:\Users\hp\Desktop\freertos_atmega328p_pwm_queue_control\FreeRTOS_Source

INCLUDES = -I"$(FREERTOS_PATH)/include" \
           -I"$(FREERTOS_PATH)/source/portable" \
           -I.

# FreeRTOS source list
FREERTOS_SRC = $(FREERTOS_PATH)/source/tasks.c \
               $(FREERTOS_PATH)/source/queue.c \
               $(FREERTOS_PATH)/source/list.c \
               $(FREERTOS_PATH)/source/timers.c \
               $(FREERTOS_PATH)/source/event_groups.c \
               $(FREERTOS_PATH)/source/croutine.c \
               $(FREERTOS_PATH)/source/portable/heap_1.c \
               $(FREERTOS_PATH)/source/portable/port.c

# Convert ./task.c → ./task.o in current folder 
FREERTOS_OBJ = tasks.o queue.o list.o timers.o event_groups.o croutine.o heap_1.o port.o io.o pwm.o

# -------- BUILD --------

all: main.hex

main.hex: main.elf
	$(OBJCOPY) -R .eeprom -O ihex main.elf main.hex

main.elf: main.o $(FREERTOS_OBJ)
	$(CC) -mmcu=$(MCU) -o main.elf main.o $(FREERTOS_OBJ)

main.o: main.c
	$(CC) -c -mmcu=$(MCU) $(INCLUDES) main.c -o main.o

# Compile FreeRTOS .c → .o into current folder
tasks.o: $(FREERTOS_PATH)/source/tasks.c
	$(CC) -c -mmcu=$(MCU) $(INCLUDES) $< -o $@

queue.o: $(FREERTOS_PATH)/source/queue.c
	$(CC) -c -mmcu=$(MCU) $(INCLUDES) $< -o $@

list.o: $(FREERTOS_PATH)/source/list.c
	$(CC) -c -mmcu=$(MCU) $(INCLUDES) $< -o $@

timers.o: $(FREERTOS_PATH)/source/timers.c
	$(CC) -c -mmcu=$(MCU) $(INCLUDES) $< -o $@

event_groups.o: $(FREERTOS_PATH)/source/event_groups.c
	$(CC) -c -mmcu=$(MCU) $(INCLUDES) $< -o $@

croutine.o: $(FREERTOS_PATH)/source/croutine.c
	$(CC) -c -mmcu=$(MCU) $(INCLUDES) $< -o $@

heap_1.o: $(FREERTOS_PATH)/source/portable/heap_1.c
	$(CC) -c -mmcu=$(MCU) $(INCLUDES) $< -o $@

port.o: $(FREERTOS_PATH)/source/portable/port.c
	$(CC) -c -mmcu=$(MCU) $(INCLUDES) $< -o $@    
	
io.o: C:\Users\hp\Desktop\colle_ex2/io.c
	$(CC) -c -mmcu=$(MCU) $(INCLUDES) $< -o $@
	
pwm.o: C:\Users\hp\Desktop\colle_ex2/pwm.c
	$(CC) -c -mmcu=$(MCU) $(INCLUDES) $< -o $@
# -------- UPLOAD --------

upload: main2.hex
	"C:/Users/hp/Desktop/Arduino/hardware/tools/avr/bin/avrdude" \
		-C"C:/Users/hp/Desktop/Arduino/hardware/tools/avr/etc/avrdude.conf" \
		-v -p$(MCU) -carduino -P$(PORT) -b$(BAUD) -D \
		-Uflash:w:main.hex:i


# -------- CLEAN --------

clean:
	del /Q *.elf *.hex *.o 2>nul || echo Clean complete