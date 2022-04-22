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

// Define P3 pins
#define DATA_BUS  (P0) 
#define RS        (P2_bit.P2_0)
#define RW        (P2_bit.P2_1) 
#define E         (P2_bit.P2_2)

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

/*******************************************
    LCD1602 Driver mapped as IO peripheral
*******************************************/  
// Delay
void delay(uint j)
 { uchar i = 60;
   for(; j>0; j--)
    { while(--i);
      i = 59;
      while(--i);
      i = 60;
    }
 }

// Test the Busy bit
void check_busy(void)
 { do
    { DATA_BUS = 0xff;
      E = 0;
      RS = 0; 
      RW = 1;
      E = 1;
      __no_operation();
    } while(DATA_BUS & 0x80);
   E = 0;
 }

// Write a command
void write_command(uchar com)
 { check_busy();
   E = 0;
   RS = 0;
   RW = 0;
   DATA_BUS = com;
   E = 1;
   __no_operation();
   E = 0;
   delay(1);
 }

// Write Data
void write_data(uchar data)
 { check_busy();
   E = 0;
   RS = 1;
   RW = 0;
   DATA_BUS = data;
   E = 1;
   __no_operation();
   E = 0;
   delay(1);   
 }

// Initialize LCD controller
void LCD_init(void)
 { write_command(0x38); // 8-bits, 2 lines, 7x5 dots
   write_command(0x0C); // no cursor, no blink, enable display
   write_command(0x06); // auto-increment on
   write_command(0x01); // clear screen
   delay(1);
 }

// Display a string
void string(uchar ad, uchar *s)
 { write_command(ad);
   while(*s>0)
    { write_data(*s++);
      delay(100);
    }
 }