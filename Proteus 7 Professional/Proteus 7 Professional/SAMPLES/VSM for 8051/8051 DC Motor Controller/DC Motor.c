/********************************
FILE NAME:        dcmotor.c
CHIP TYPE:        AT89C51
CLOCK FREQUENCY:  12MHZ
IDE:              VSMStudio
COMPILER:         IAR for 8051
TIME:             September 2010
********************************/

#include "ioAT89C51.h"
#include "intrinsics.h"

// Define P3 pins
#define Inc   (P3_bit.P3_4) 
#define Dec   (P3_bit.P3_5)
#define Dir   (P3_bit.P3_6) 
#define PWM   (P3_bit.P3_7)

// Define new types
typedef unsigned char   uchar;
typedef unsigned int    uint;

void delay(uint);

void main(void)
 { int speed;
   // Select initial direction and speed.
   Dir = 1;
   if (Dir)
      speed = 400;
   else
      speed = 100;
   
   // Main control loop
   while(1)
    { if(!Inc)
      // Increase speed   
         speed = speed > 0 ? speed - 1 : 0;
      if(!Dec)
      // Decrease speed
         speed = speed < 500 ? speed + 1 : 500;
      
      // Drive a PWM signal out. 
      PWM=1;
      delay(speed);
      PWM=0;
      delay(500-speed);
    }
 }

void delay(uint j)
 { for(; j>0; j--)
    { __no_operation();
    }
 }