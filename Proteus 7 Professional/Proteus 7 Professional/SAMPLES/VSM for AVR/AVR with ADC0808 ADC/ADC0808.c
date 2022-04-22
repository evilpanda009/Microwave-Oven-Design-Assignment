/********************************
FILE NAME:        c8255.c
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
#define pin(x,PIN) (PIN)&(1<<x)


// ADC0808 pins definition
#define S_START sbit(0,PORTD) 
#define C_START cbit(0,PORTD)
#define S_ADD_A sbit(2,PORTD) 
#define C_ADD_A cbit(2,PORTD)
#define S_ADD_B sbit(3,PORTD) 
#define C_ADD_B cbit(3,PORTD)
#define S_ADD_C sbit(4,PORTD) 
#define C_ADD_C cbit(4,PORTD)
#define S_OE sbit(5,PORTD) 
#define C_OE cbit(5,PORTD)
#define EOC   pin(1,PIND)
#define BUS PORTC
#define LED PORTA
#define IN PINC

int main(void)
 { SPL = 0x5f;
   SPH = 0x04;
   DDRA = 0xff;
   DDRC = 0x00;
   DDRB = 0xff;
   DDRD = 0xfd;
   PORTD = 0xff;
   LED = 0x00;
   // Configure ADC clock
   TCCR0 = 0x1a;
   OCR0 = 0x01;// Foc = Fclk/2*(1+OCR0)
   // Configure the channel IN0
   C_ADD_A;
   C_ADD_B;
   C_ADD_C;
   while(1)
    { // Start a new conversion
      C_START;
      S_START;
      C_START;
      while(1)
       { if(EOC) //Sample complete so output it to LEDs
          { S_OE;
            asm("nop");
            asm("nop");
            LED=~IN;
            C_OE;
            break;
          }
         else
          { asm("nop");
          }
       }
    }
 } 
