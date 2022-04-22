//**************************************************
//  Target     : MSP430 ComparatorA
//  Description: Simple Voltmeter 
//              (0.1 < Vsrc < 4.0)
//   
//  Copyright (c) Usachev M.  
//  February, 2009
//  Built with IAR Embedded Workbench Version: 4.11C
//   
//  Warning! vref contains empirical values.
//  If you change the schema or clock-source
//  or even "while(1) counter++;", it would
//  calibrate these values again
//**************************************************

#include <msp430x11x1.h>
#include "intrinsics.h"

//              Voltage   = {0, 0.5, 1.0, 1.5, 2.0, 2.5, 3.0, 3.5, 4.0, end}
static const short vref[] = {0,  20,  43,  68,  99, 132, 175, 232, 303, -1};
unsigned short counter;

#define CHARGER P2OUT

#pragma vector=COMPARATORA_VECTOR
__interrupt void ComparatorAHandler( void )
{ for (int i = 0; vref[i] >= 0; i++)
  { if (counter < vref[i])
    { short vmax = i * 5;                 // max voltage
      short ddref = vref[i] - vref[i-1];
      short dd = vref[i] - counter;
      vmax -= (5 * dd)/ddref;
      unsigned char ch = ((vmax / 10) << 4) + (vmax % 10);
      P1OUT = ch;
      break;
    }
  }
  // Discharge Capacitor
  CHARGER &= 0;
  for(counter = 5000; counter; counter--);
  CACTL1 &= ~CAIFG;
  // Start charging again
  CHARGER |= 1;
}

int main( void )
{ __disable_interrupt();
  WDTCTL = WDTPW + WDTHOLD;               // Stop watchdog timer

  P1DIR = 0xFF;                           // Direction: output
  P1SEL = 0;                              // Pins configured as I/O
  P1OUT = 0xff;                           // Default value means 'evaluating...'
  
  P2DIR = 1;                              // P2.0 is output
  P2SEL = 0x1c;                           // Enable Function
  CACTL2 = P2CA0 | P2CA1;                 // Enable physical pins connection
  CACTL1 = CAON | CAIE;                   // Turn on Comparator A with Interrupt

  counter = 0;
  __enable_interrupt();
  // Charge Capacitor
  CHARGER |= 1;
  while(1) counter++;
  return 0;
}
