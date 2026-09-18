# FreeRTOS PWM Dimmer System using Inter-Task Queue Communication (ATmega328P)

This repository contains a bare-metal embedded C application implementing LED brightness control via PWM signal and FreeRTOS Queues on an ATmega328P microcontroller.

## 📌 Project Overview
* **Task_Button (Producer):** Monitors external push-button state with software debouncing, increments brightness level (0%, 25%, 50%, 75%, 100%), and pushes the target duty cycle to a FreeRTOS Queue.
* **Task_LED (Consumer):** Blocks on `xQueueReceive()` waiting for new data, then updates Timer0 PWM output registers to adjust LED intensity.

## 🛠 Tech Stack & Features
* **Microcontroller:** ATmega328P (16 MHz)
* **RTOS:** FreeRTOS (Queue IPC, Task Management)
* **Peripherals:** Timer0 (Fast PWM Mode on PD6 / OC0A pin)
* **Toolchain:** `avr-gcc`, custom `Makefile`, `avrdude`
* **Simulation:** Proteus VSM

## ⚙️ Compilation & Build
```bash
make clean
make