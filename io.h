#ifndef IO_H_
#define IO_H_

#define INCLUDE_XTAL_PINS

enum TERMS { input='i', output='o', input_pullup='p', HIGH=1, LOW=0 };

typedef struct DIGITAL_IO
{
	unsigned char port;
	unsigned char pin;
}DIGITAL_IO;

void pin_mode(unsigned char, unsigned char, unsigned char);

void set_state(unsigned char, unsigned char, unsigned char);

char get_state(unsigned char, unsigned char);

void pinMode(struct DIGITAL_IO, unsigned char);

void setState(struct DIGITAL_IO, unsigned char);

char getState(struct DIGITAL_IO);

#endif
