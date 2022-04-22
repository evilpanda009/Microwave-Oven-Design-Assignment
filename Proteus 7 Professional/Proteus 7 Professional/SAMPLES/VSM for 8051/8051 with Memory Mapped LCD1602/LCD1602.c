/********************************
FILE NAME:        lcd1602.c
CHIP TYPE:        AT89C51
CLOCK FREQUENCY:  12MHZ
IDE:              VSMStudio
COMPILER:         IAR for 8051
TIME:             September 2010
********************************/
#include "ioAT89C51.h"
#include "intrinsics.h"

// Define new types
typedef unsigned char   uchar;
typedef unsigned int    uint;

// Function Prototypes
void check_busy(void);
void write_command(uchar com);
void write_data(uchar data);
void LCD_init(void);
void string(uchar ad ,uchar *s);
void lcd_test(void);
void delay(uint);

// Pins are mapped at absolute memory locations
__xdata __no_init char LCD_WC @ 0x7ffc;
__xdata __no_init char LCD_WD @ 0x7ffd;
__xdata __no_init char LCD_RC @ 0x7ffe;

void main(void)
 { LCD_init(); 
   while(1) 
    { string(0x80,"Have a nice day!");
      string(0xC0,"  Proteus VSM");
      delay(100); 
      write_command(0x01);
      delay(100);       
    }
 }

// Delay of j milliseconds - adjusted for 12MHz clock
void delay(uint j)
 { uchar i = 60;
   for(; j>0; j--)
    { while(--i);
      i=59;
      while(--i);
      i=60;
    }
 }


/*******************************************
    LCD1602 Driver Memory mapped 
*******************************************/  

// Test the Busy bit
void check_busy(void)
 { uchar dt;
   do
    { dt = LCD_RC;
      delay(1);
      } while(dt & 0x80);
    }

// Write a command
void write_command(uchar com)
 { check_busy();
   LCD_WC = com;
   delay(1);
 }

// Write Data
void write_data(uchar data)
 { check_busy();
   LCD_WD = data;
   delay(1);   
 }

// Initialize LCD controller
void LCD_init(void)
 { write_command(0x30); // 8-bits, 1 lines, 7x5 dots 
   write_command(0x38); // 8-bits, 2 lines, 7x5 dots
   write_command(0x0C); // no cursor, no blink, enable display
   write_command(0x01); // clear screen
   write_command(0x06); // auto-increment on
 }

// Display a string
void string(uchar ad,uchar *s)
 { write_command(ad);
   while(*s>0)
    { write_data(*s++);
      delay(100);
    }
 }