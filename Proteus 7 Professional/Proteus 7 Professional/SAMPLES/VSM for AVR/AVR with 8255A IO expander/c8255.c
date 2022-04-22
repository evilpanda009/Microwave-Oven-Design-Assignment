/********************************
FILE NAME:        c8255.c
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
#define sbit(x,PORT) ((PORT) |= (1<<x))
#define cbit(x,PORT) ((PORT) &= ~(1<<x))
#define pin(x,PIN)   ((PIN)  & (1<<x))

// 8255 pins definition

// RST
#define srst sbit(0,PORTD)
#define crst cbit(0,PORTD)
// LE 
#define sle  sbit(1,PORTD)
#define cle  cbit(1,PORTD)
// CS
#define scs  sbit(2,PORTD)
#define ccs  cbit(2,PORTD)
// RD
#define srd  sbit(3,PORTD)
#define crd  cbit(3,PORTD)
// WR
#define swr  sbit(4,PORTD)
#define cwr  cbit(4,PORTD)
// I/O BUS
#define  out  PORTC
#define  in   PINC

// Address latch multiplexer. 
void latch_it(void)
 { cle;
   asm("nop");
   sle;
   asm("nop");
   cle;
 }


int main()
 { uchar temp;
   // Initialize Stack Pointer
   SPL = 0x54;
   SPH = 0x04;
   // Initialize PORTD & C for all output pins. 
   // NOTE: in order to prevent any bus contention the PORTC should 
   // be initialized as output only after the 8255A is fully disabled.
   DDRD = 0xff;
   // Disable 8255A
   srd;
   swr;
   scs;
   // PORTC pins can now safely initialized as outputs
   DDRC=0xff;
   // Enable the 8255A and reset it
   ccs;
   srst;
   asm("nop"); asm("nop"); asm("nop");
   crst;
   //
   out = 0x03;
   latch_it();
   out = 0x82;
   cwr;
   asm("nop");
   swr;
   while(1)
    { //
      out = 0x01;
      latch_it();
      DDRC = 0;
      asm("nop");
      crd;
      asm("nop");
      temp = in;
      srd;
      //
      DDRC = 0xff;	
      out = 0x00;
      latch_it();
      out = temp;
      cwr;
      asm("nop");
      swr;
    }
 }

