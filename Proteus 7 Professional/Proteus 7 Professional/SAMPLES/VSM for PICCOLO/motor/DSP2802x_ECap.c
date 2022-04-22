// TI File $Revision: /main/1 $
// Checkin $Date: August 14, 2008   16:58:25 $
//###########################################################################
//
// FILE:   DSP2802x_ECap.c
//
// TITLE:  DSP2802x eCAP Initialization & Support Functions.
//
//###########################################################################
// $TI Release: 2802x C/C++ Header Files V1.26 $
// $Release Date: February 2, 2010 $
//###########################################################################

#include "DSP2802x_Device.h"     // DSP2802x Headerfile Include File
#include "DSP2802x_Examples.h"   // DSP2802x Examples Include File

//---------------------------------------------------------------------------
// InitECap:
//---------------------------------------------------------------------------
// This function initializes the eCAP(s) to a known state.
//
void InitECap(void)
{
   // Initialize eCAP1

   //tbd...

}

//---------------------------------------------------------------------------
// Example: InitECapGpio:
//---------------------------------------------------------------------------
// This function initializes GPIO pins to function as ECAP pins
//
// Each GPIO pin can be configured as a GPIO pin or up to 3 different
// peripheral functional pins. By default all pins come up as GPIO
// inputs after reset.
//
// Caution:
// For each eCAP peripheral
// Only one GPIO pin should be enabled for ECAP operation.
// Comment out other unwanted lines.

void InitECapGpio()
{

   InitECap1Gpio();
}

#define QUAL_SYNC    0
#define QUAL_SYNC3   1
#define QUAL_SYNC6   2
#define QUAL_ASYNC   3

void InitECap1Gpio(void)
{
   EALLOW;
/* Enable internal pull-up for the selected pins */
// Pull-ups can be enabled or disabled by the user.
// This will enable the pullups for the specified pins.
// Comment out other unwanted lines.

// GpioCtrlRegs.GPAPUD.bit.GPIO5 = 0;      // Enable pull-up on GPIO5 (CAP1)
   GpioCtrlRegs.GPAPUD.bit.GPIO19 = 0;     // Enable pull-up on GPIO19 (CAP1)

// Inputs are synchronized to SYSCLKOUT by default.
// Comment out other unwanted lines.

// GpioCtrlRegs.GPAQSEL1.bit.GPIO5 = QUAL_SYNC;    // Synch to SYSCLKOUT GPIO5 (CAP1)
   GpioCtrlRegs.GPAQSEL2.bit.GPIO19 = QUAL_ASYNC;   // Synch to SYSCLKOUT GPIO19 (CAP1)

/* Configure eCAP-1 pins using GPIO regs*/
// This specifies which of the possible GPIO pins will be eCAP1 functional pins.
// Comment out other unwanted lines.

// GpioCtrlRegs.GPAMUX1.bit.GPIO5 = 3;     // Configure GPIO5 as CAP1
   GpioCtrlRegs.GPAMUX2.bit.GPIO19 = 3;    // Configure GPIO19 as CAP1

    EDIS;
}
//===========================================================================
// End of file.
//===========================================================================
