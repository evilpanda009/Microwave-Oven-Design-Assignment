/********************************
FILE NAME:        dc_motor.c
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

/**** Low level port/pin definitions ****/
#define sbit(x,PORT) (PORT) |= (1<<x)
#define cbit(x,PORT) (PORT) &= ~(1<<x)
#define pin(x,PIN)   (PIN)  &  (1<<x)

/********** Main Program *******************/
int main(void)
 { SPL = 0x5f;      // Initialize Stack Pointer
   SPH = 0x04;
   DDRB = 0x0c;     // pins PB2 & PB3 as outputs
   PORTB = 0xff;
 
   /* Set PWM Duty Cycle */
   TCCR0 = 0x65;  
   OCR0 = 0xcc;   
   
   while(1);
 } 
