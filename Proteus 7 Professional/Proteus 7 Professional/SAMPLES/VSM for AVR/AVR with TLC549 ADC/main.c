/********************************
FILE NAME:        S_AD.c
CHIP TYPE:        ATMEGA16
CLOCK FREQUENCY:  8MHZ
IDE:              VSMStudio
COMPILER:         AVR-GCC
TIME:             September 2010
********************************/
#include <avr/io.h>
#include <util/delay.h>


#define uchar unsigned char
#define uint  unsigned int

// Low level port/pin definitions 
#define sbit(x,PORT) (PORT) |= (1<<x)
#define cbit(x,PORT) (PORT) &= ~(1<<x)
#define pin(x,PIN)   (PIN)&(1<<x)

// Pins definition
#define out   PORTA
#define s_ss  sbit(4,PORTB)
#define c_ss  cbit(4,PORTB)


volatile uchar data;
volatile long i;


int main(void)   
 { // Initialize Stack Pointer
   SPL = 0x54;
   SPH = 0x04;
   // Initialize ports
   DDRA = 0xff;
   PORTA = 0;
   // Initialize SPI 
   DDRB = 0x90;  // SCK & $SS are outputs and SDO as input
   SPCR = 0x53;  // SPI enabled, Master mode, SCK=Fosc/128
   SPSR &= 0xfe; // Normal SPI speed.
   while(1)
    { c_ss;                         // Select the TLC549
      SPDR = 0;
      while(!(SPSR & (1<<SPIF)));   // Wait for reception complete
      data = SPDR;                  // get data from SPI data register
      out = ~data;                  // Invert all bits
      s_ss;                         // Disable TLC549
      _delay_ms(50);
    }
 }
