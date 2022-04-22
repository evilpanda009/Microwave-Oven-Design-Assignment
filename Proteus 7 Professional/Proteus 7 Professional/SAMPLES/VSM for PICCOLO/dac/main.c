/**********************************************************************
* Function: main()                                                    *
* Description: main function for PWM/DAC example                      *
* Device: TMS320F2808, TMS320F2806, TMS320F2801                       *
* Author: David M. Alter, Texas Instruments Inc.                      *
* Function Prototype: void main(void)                                 *
* Useage: N/A                                                         *
* Input Parameters: none                                              *
* Return Value: none                                                  *
* Notes: none                                                         *
* History:                                                            *
*   02/07/06 - original (D. Alter)                                    *
*********************************************************************/
#include "DSP2802x_Device.h"					// Peripheral address definitions
#include "HRPWM_DAC.h"				// Include file specific to this project

interrupt void cpu_timer0_isr(void);

//---------------------------------------------------------------------
// Global Variables used by HRPWM SFO functions
//
volatile struct EPWM_REGS *ePWM[] = {&EPwm1Regs, &EPwm2Regs, &EPwm3Regs, &EPwm4Regs};

int16 MEP_ScaleFactor[5] = {0,0,0,0,0}; 	// HRPWM Scale Factor values for ePWM1-4


//---------------------------------------------------------------------
void main(void)
{

//*** CPU Initialization
	InitSysCtrl();							// Initialize the CPU (FILE: SysCtrl.c)
	InitGpio();								// Initialize the shared GPIO pins (FILE: Gpio.c)

// Disable CPU interrupts
   DINT;

	InitPieCtrl();							// Initialize and enable the PIE (FILE: PieCtrl.c)

// Disable CPU interrupts and clear all CPU interrupt flags:
   IER = 0x0000;
   IFR = 0x0000;
   InitPieVectTable();
   EALLOW;  // This is needed to write to EALLOW protected registers
   PieVectTable.TINT0 = &cpu_timer0_isr;
   EDIS;    // This is needed to disable write to EALLOW protected registers

//*** PWM/DAC Initialization

// Do the initial HRPWM calibration.
//
// In this example, one ePWM HRPWM module is being used as the calibrator,
// under the assumption that the other ePWM modules are all similar.  For
// more information, see the "TMS320x280x High-Resolution Pulse Width
// Modulator (HRPWM) Reference Guide," Literature #SPRU924.
	//while ( MEP_ScaleFactor[HRPWM_CAL_CH] == 0 ) SFO_MepDis(HRPWM_CAL_CH);

// Setup the ePWM for the DAC function
	InitDac(DAC1_pin, DAC1_period);

//*** Enable the clocks to the ePWM module
// Note: this should be done after all ePWM modules are configured to
// ensure synchronization between the ePWM modules.
	asm(" EALLOW");							// Enable EALLOW protected register access
	SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 1;	// HSPCLK to ePWM modules enabled
	asm(" EDIS");							// Disable EALLOW protected register access

//*** Configure CPU Timer0
	InitTimer0();

//*** Enable interrupts
// Enable CPU int1 which is connected to CPU-Timer 0, CPU int13
// which is connected to CPU-Timer 1, and CPU int 14, which is connected
// to CPU-Timer 2:
   IER |= M_INT1;
//   IER |= M_INT13;
//   IER |= M_INT14;

// Enable TINT0 in the PIE: Group 1 interrupt 7
   PieCtrlRegs.PIEIER1.bit.INTx7 = 1;

// Enable global Interrupts and higher priority real-time debug events:
   EINT;   // Enable Global interrupt INTM
   ERTM;   // Enable Global realtime interrupt DBGM

//    SetDBGIER(IER);							// Configure the DBGIER for realtime debug
//    asm(" CLRC INTM, DBGM");				// Enable global interrupts and realtime debug

//*** Main Loop
	while(1)								// Dummy loop.  Wait for an interrupt.
	{
		//SFO_MepDis(HRPWM_CAL_CH);			// Continuously do the HRPWM calibration in the background loop
	}

} //end of main()



/*********************************************************************/
interrupt void cpu_timer0_isr(void)
{
static Uint16 i=0;
Uint16 DAC1_frac;

	PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;			// Must acknowledge the PIE group

#if OUTPUT == SQUARE								// Output a square wave

	if(i == 128)
	{
		DAC1_frac = Q15_SCALE*OFFSET/PWM_VOLTAGE;	// Output low level
		WriteDac(DAC1_pin, DAC1_frac);				// Write to the DAC
		i++;										// Increment the counter
	}
	else if(i == 255)
	{
		DAC1_frac = Q15_SCALE*(OFFSET+AMPLITUDE)/PWM_VOLTAGE;	// Output hi level
		WriteDac(DAC1_pin, DAC1_frac);							// Write to the DAC
		i = 0;													// Reset the counter
	}
	else i++;										// Increment the counter

#elif OUTPUT == SINE								// Output a sine wave

	DAC1_frac = sine256Q15[i];						// Read sine wave table
	WriteDac(DAC1_pin, DAC1_frac);					// Write to the DAC
	i++;											// Increment the index
	i &= 0xFF;										// Wrap index modulo 8-bit

#elif OUTPUT == CONSTANT							// Output a constant voltage

	DAC1_frac = Q15_SCALE*AMPLITUDE/PWM_VOLTAGE;	// Output selected amplitude
	WriteDac(DAC1_pin, DAC1_frac);					// Write to the DAC

#endif

}
/*** end of file *****************************************************/
