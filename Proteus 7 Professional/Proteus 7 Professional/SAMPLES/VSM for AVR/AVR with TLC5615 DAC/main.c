/********************************
FILE NAME:        main.c
CHIP TYPE:        ATMEGA16
CLOCK FREQUENCY:  8MHZ
IDE:              VSMStudio
COMPILER:         AVR-GCC
TIME:             September 2010
********************************/
#include <avr/io.h>
#define uchar unsigned char
#define uint unsigned int

// Low level port/pin definitions
#define sbit(x,PORT) (PORT) |= (1<<x)
#define cbit(x,PORT) (PORT) &= ~(1<<x)
#define pin(x,PIN) (PIN)&(1<<x)

// Pins definition
#define  out  PORTA
#define s_ss  sbit(4,PORTB)
#define c_ss  cbit(4,PORTB)

volatile  uint data = 0x200;  //Vout=2*Vref*512/1024=Vref
volatile long i;

int main(void)
 { // Initialize Stack Pointer
   SPL = 0x54;
   SPH = 0x04;
   // Initialize ports
   DDRB = 0xff;
   PORTB = 0xff;
   SPCR = 0x53;                  // SPI enabled, Master mode, SCK=Fosc/128
   SPSR &= 0xfe;                 // Normal SPI speed.
   data <<= 2;
   data &= 0x0fff;               // Initialize data 
   while(1)
    { c_ss;                      // Select the TLC5615
      SPDR = (data>>8) & 0xff;   // Put high byte in the Data Register
      while(!(SPSR&(1<<SPIF)));  // Wait for reception complete
      SPDR = data & 0x0ff;       // Put low byte in the Data Register
      while(!(SPSR&(1<<SPIF)));  // Wait for reception complete
      s_ss;
      for (i=0; i<50000; i++);
    }
 } 
