/********************************
FILE NAME:        stepper.c
CHIP TYPE:        AT89C51
CLOCK FREQUENCY:  12MHZ
IDE:              VSMStudio
COMPILER:         IAR for 8051
TIME:             September 2010
********************************/

#include "ioAT89C51.h"

// Definition for output port and input pins
#define out_port  (P2)
#define key_for   (P0_bit.P0_0) 
#define key_rev   (P0_bit.P0_1)

// Define new types
typedef unsigned char   uchar;
typedef unsigned int    uint;

void delayms(uint);

// Array of Stepping Sequences 
uchar const sequence[8] = {0x02,0x06,0x04,0x0c,0x08,0x09,0x01,0x03};

void main(void)
 { uchar i;
   out_port = 0x03;
   while(1)
    { // Has the forward key been pressed ? 
      if (!key_for)
       { i = i<8 ? i+1 : 0;
         out_port = sequence[i];
         delayms(50);
       }
      // Has the reverse key been pressed ?
      else if (!key_rev)
       { i = i>0 ? i-1 : 7;	
         out_port = sequence[i];
         delayms(50);
       }	
    }
 }

void delayms(uint j)
 { uchar i;
   for(; j>0; j--)
    { i = 120;
      while (i--);  
    }
 }