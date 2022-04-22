/********************************
FILE NAME:        dac0832.c
CHIP TYPE:        ATMEGA16
CLOCK FREQUENCY:  8MHZ
IDE:              VSMStudio
COMPILER:         AVR-GCC
TIME:             September 2010
********************************/
#include <avr/io.h>
#include <util/delay.h>
#define uchar unsigned char
#define uint unsigned int

// Low level port/pin definitions
#define sbit(x,PORT) (PORT) |= (1<<x)
#define cbit(x,PORT) (PORT) &= ~(1<<x)
#define pin(x,PIN)   (PIN)&(1<<x)

// DAC pin definitions
#define S_CS sbit(0,PORTD) 
#define C_CS cbit(0,PORTD)
#define S_WR sbit(1,PORTD) 
#define C_WR cbit(1,PORTD)
#define BUS PORTC

int main(void)
 { uchar data = 0;
   // Initalize the Stack Pointer
   SPL = 0x5f;
   SPH = 0x04;
   DDRC = 0xff;
   DDRD = 0xff;
   PORTD = 0xff;
   BUS = 0xff;
   C_CS;
   while(1)
    { BUS = data;
      C_WR;
      asm("nop");
      S_WR;
      data++;
      _delay_ms(3);
    }
 } 
