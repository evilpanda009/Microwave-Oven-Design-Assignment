/********************************
FILE NAME:        stepper.c
CHIP TYPE:        ATMEGA16
CLOCK FREQUENCY:  8MHZ
IDE:              VSMStudio
COMPILER:         AVR-GCC
TIME:             September 2010
********************************/
#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#define uchar unsigned char
#define uint unsigned int

// Low level port/pin definitions 
#define sbit(x,PORT) (PORT) |= (1<<x)
#define cbit(x,PORT) (PORT) &= ~(1<<x)
#define pin(x,PIN) (PIN)&(1<<x)

// Pins definition
#define pos pin(0,PINC) 
#define neg pin(1,PINC) 
#define out PORTD

uchar PROGMEM turn[] = {0x02,0x06,0x04,0x0c,0x08,0x09,0x01,0x03};

int main(void)
 { uchar i=0;
   // Initialize Stack Pointer
   SPL = 0x5f;
   SPH = 0X04;
   // Configure Ports
   DDRD = 0xff;
   DDRC = 0x00;
   out  = 0xff;
   while(1)
    { if(!(pos))
       { i = i<8? i+1: 0;
         out = pgm_read_byte(&turn[i]);
         _delay_ms(50);
       }
      else if(!(neg))
	    { i = i>0? i-1: 7;
	      out = pgm_read_byte(&turn[i]);
	      _delay_ms(50);	
	    }
	 }
 } 	  		
