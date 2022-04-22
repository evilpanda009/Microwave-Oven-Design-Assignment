/********************************************************************/
/********************************************************************/
/*****                                                          *****/
/*****        L A B C E N T E R    E L E C T R O N I C S        *****/
/*****                                                          *****/
/*****       LABCENTER INTEGRATED SIMULATION ARCHITECTURE       *****/
/*****                                                          *****/
/*****            Test program for AD1674 and 80C51             *****/
/*****                                                          *****/
/********************************************************************/
/********************************************************************/

#include <8051.h>
#include <stdio.h>
 
 //typedef P1.0 STATUS;
 volatile __sbit __at (0x80) STATUS;

__xdata __at (0x2FFF) unsigned char CTRL;
__xdata __at (0x4FFF) unsigned char ADSEL;
unsigned char hByte;
unsigned char lByte;

void adc_Convert (void)
 { // Start a conversion with A0 and A/$C$ low. 
   // The convesion takes place on rising CE edge.
   CTRL  = 0x00; 
   ADSEL = 0x00;
   // Wait until we have completed a conversion .
   while(STATUS==1);
   // Set R/$C$ with A0 low and read the low byte. 
   CTRL  = 0x02; 
   hByte = ADSEL; 
   // Set R/$C$ with A0 high and read the high. 
   CTRL  = 0x03; 
   lByte = ADSEL; 
 }
 
 void putchar (char c) 
  { while (!TI)    /* assumes UART is initialized */ 
      ; 
    TI = 0; 
    SBUF = c; 
  } 

void main(void)
 { unsigned int delay, MSB , LSB, adc_Res;
   // Initialize serial interface
   SCON  = 0xDA;        // SCON: mode 1, 8-bit UART, enable rcvr      */
   TMOD |= 0x20;        // TMOD: timer 1, mode 2, 8-bit reload        */
   TH1   = 0xFD;        // TH1:  reload value for 1200 baud @ 12MHz   */
   TR1   = 1;           // TR1:  timer 1 run                          */
   TI    = 1;           // TI:   set TI to send first char of UART    */
   
   while(1)
    { adc_Convert();
      MSB=(unsigned int)(hByte << 4);
      LSB=(unsigned int)(lByte >> 4);
      // adc_Res now has the converted data with 12-bit resolution.
      adc_Res = MSB + LSB;
      // Send adc results to the serial interface
      printf("ADC READINGS: %03Xh\r\n", adc_Res);
     // simple delay - it is mcu clock dependent !
      for (delay=0; delay<10000; ++delay) 
	    ;
    }
 }
 