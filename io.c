#include <avr/io.h>
#include <avr/interrupt.h>
#include "io.h"

#define _SLEEP() __asm__ __volatile__ ( "sleep" "\n\t" :: )
#define _NOP() __asm__ __volatile__ ( "nop" "\n\t" :: )

#ifdef INCLUDE_XTAL_PINS
// function to set the mode of a digital pin
void pin_mode(unsigned char port, unsigned char pin, unsigned char mode)
{
	if(pin <= 7 )
	{
		switch( mode )
		{
			case 'i' :
			case 'I' :
			// pin to be set as input
				if(port == 'B')
				{
					PORTB &= (~(1<<pin));
					DDRB &= (~(1<<pin));
				}
				else if(port == 'C' && pin<6)
				{
					PORTC &= (~(1<<pin));
					DDRC &= (~(1<<pin));
				}
				else if(port == 'D')
				{
					PORTD &= (~(1<<pin));
					DDRD &= (~(1<<pin));
				}
			break;

			case 'o' :
			case 'O' :
			// set the pin as output
				if(port == 'B')
				{
					PORTB &= (~(1<<pin));
					DDRB |= (1<<pin);
				}
				else if(port == 'C' && pin<6)
				{
					PORTC &= (~(1<<pin));
					DDRC |= (1<<pin);
				}
				else if(port == 'D')
				{
					PORTD &= (~(1<<pin));
					DDRD |= (1<<pin);
				}
			break;

			case 'p' :
			case 'P' :
			// set the pin as input with pull up enabled
				if(port == 'B')
				{
					PORTB |= (1<<pin);
					DDRB &= (~(1<<pin));
				}
				else if(port == 'C' && pin<6)
				{
					PORTC |= (1<<pin);
					DDRC &= (~(1<<pin));
				}
				else if(port== 'D')
				{
					PORTD |= (1<<pin);
					DDRD &= (~(1<<pin));
				}

			break;

		}
	}
}

// set the output level of a digital input pin
void set_state(unsigned char port, unsigned char pin, unsigned char state)
{
	if(pin <= 7 )
	{
		switch( port )
		{
			case 'B' :
			if( DDRB & (1<<pin) )
			{
				if(state)
					PORTB |= (1<<pin);
				else
					PORTB &= (~(1<<pin));
			}
			break;

			case 'C':
			if(pin >= 6) break;
			if( DDRC & (1<<pin) )
			{
				if(state)
					PORTC |= (1<<pin);
				else
					PORTC &= (~(1<<pin));
			}
			break;

			case 'D':
			if( DDRD & (1<<pin) )
			{
				if(state)
					PORTD |= (1<<pin);
				else
					PORTD &= (~(1<<pin));
			}
			break;

		}
	}
}

// function to get the input level of a digital input pin
char get_state(unsigned char port, unsigned char pin)
{
	if(pin <= 7 && (port=='B' || port=='C' || port=='D') )
	{
		unsigned char PORT;
		volatile unsigned char input;
		switch( port )
		{
			case 'B' :
			PORT=PORTB;
			PORTB=PORT;
			_NOP();
			input=PINB;
			break;

			case 'C':
			if(pin>=6) break;
			PORT=PORTC;
			PORTC=PORT;
			_NOP();
			input=PINC;
			break;

			case 'D':
			PORT=PORTD;
			PORTD=PORT;
			_NOP();
			input=PIND;
			break;

		}
		if( input & (1<<pin) ) return 1;
		else return 0;
	}
	else
		return -1;
}

#endif

#ifndef INCLUDE_XTAL_PINS
// function to set the mode of a digital pin
void pin_mode(unsigned char port, unsigned char pin, unsigned char mode)
{
	if(pin <= 7 )
	{
		switch( mode )
		{
			case 'i' :
			case 'I' :
			// pin to be set as input
				if(port == 'B' && pin<6)
				{
					PORTB &= (~(1<<pin));
					DDRB &= (~(1<<pin));
				}
				else if(port == 'C' && pin<6)
				{
					PORTC &= (~(1<<pin));
					DDRC &= (~(1<<pin));
				}
				else if(port == 'D')
				{
					PORTD &= (~(1<<pin));
					DDRD &= (~(1<<pin));
				}
			break;

			case 'o' :
			case 'O' :
			// set the pin as output
				if(port == 'B' && pin<6)
				{
					PORTB &= (~(1<<pin));
					DDRB |= (1<<pin);
				}
				else if(port == 'C' && pin<6)
				{
					PORTC &= (~(1<<pin));
					DDRC |= (1<<pin);
				}
				else if(port == 'D')
				{
					PORTD &= (~(1<<pin));
					DDRD |= (1<<pin);
				}
			break;

			case 'p' :
			case 'P' :
			// set the pin as input with pull up enabled
				if(port == 'B' && pin<6)
				{
					PORTB |= (1<<pin);
					DDRB &= (~(1<<pin));
				}
				else if(port == 'C' && pin<6)
				{
					PORTC |= (1<<pin);
					DDRC &= (~(1<<pin));
				}
				else if(port== 'D')
				{
					PORTD |= (1<<pin);
					DDRD &= (~(1<<pin));
				}

			break;

		}
	}
}

// set the output level of a digital input pin
void set_state(unsigned char port, unsigned char pin, unsigned char state)
{
	if(pin <= 7 )
	{
		switch( port )
		{
			case 'B' :
			if(pin >= 6) break;
			if( DDRB & (1<<pin) )
			{
				if(state)
					PORTB |= (1<<pin);
				else
					PORTB &= (~(1<<pin));
			}
			break;

			case 'C':
			if(pin >= 6) break;
			if( DDRC & (1<<pin) )
			{
				if(state)
					PORTC |= (1<<pin);
				else
					PORTC &= (~(1<<pin));
			}
			break;

			case 'D':
			if( DDRD & (1<<pin) )
			{
				if(state)
					PORTD |= (1<<pin);
				else
					PORTD &= (~(1<<pin));
			}
			break;

		}
	}
}

// function to get the input level of a digital input pin
char get_state(unsigned char port, unsigned char pin)
{
	if(pin <= 7 && (port=='B' || port=='C' || port=='D') )
	{
		unsigned char PORT;
		volatile unsigned char input;
		switch( port )
		{
			case 'B' :
			if(pin>=6) break;
			PORT=PORTB;
			PORTB=PORT;
			_NOP();
			input=PINB;
			break;

			case 'C':
			if(pin>=6) break;
			PORT=PORTC;
			PORTC=PORT;
			_NOP();
			input=PINC;
			break;

			case 'D':
			PORT=PORTD;
			PORTD=PORT;
			_NOP();
			input=PIND;
			break;

		}
		if( input & (1<<pin) ) return 1;
		else return 0;
	}
	else
		return -1;
}

#endif



void pinMode(struct DIGITAL_IO PIN, unsigned char mode)
{
	pin_mode(PIN.port,PIN.pin,mode);
}

void setState(struct DIGITAL_IO PIN, unsigned char st)
{
	set_state(PIN.port,PIN.pin,st);
}

char getState(struct DIGITAL_IO PIN)
{
	return get_state(PIN.port,PIN.pin);
}
