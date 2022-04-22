/**********************************************************************
* Function: InitTimer0()                                              *
* Description: Initializes 32-bit Timer1 in the C28x CPU to generate  *
*   a periodic interrupt.                                             *
* Device: TMS320F28xx                                                 *
* Author: David M. Alter, Texas Instruments Inc.                      *
* Function Prototype: void InitTimer0(void);                          *
* Useage: InitTimer0();                                               *
* Input Parameters: none                                              *
* Return Value: none                                                  *
* Notes:                                                              *
* 1) This function uses the DSP280x Peripheral Header files, TI       *
*    literature #SPRC191.                                             *
* History:                                                            *
*   02/07/06 - original (D. Alter)                                    *
**********************************************************************/
#include "DSP2802x_Device.h"					// Peripheral address definitions
#include "HRPWM_DAC.h"				// Include file specific to this project

void InitTimer0(void)
{
	CpuTimer0Regs.TCR.bit.TSS = 1;				// Stop the timer
	CpuTimer0Regs.TIM.all = 0;					// Zero the counter
	CpuTimer0Regs.TPR.bit.PSC = 0;				// Zero the lower byte of the prescale counter
	CpuTimer0Regs.TPRH.bit.PSCH = 0;			// Zero the upper byte of the prescale counter

	CpuTimer0Regs.TPR.bit.TDDR = Timer0_Prescale & 0xFF;				// Lower byte of prescale
	CpuTimer0Regs.TPRH.bit.TDDRH = (Timer0_Prescale & 0xFF00) >> 8;		// Upper byte of prescale

	CpuTimer0Regs.PRD.all = Timer0_Period;		// Set the period

	CpuTimer0Regs.TCR.all = 0xC820;
/*
 bit 15        1:      TIF, Timer interrupt flag (write 1 to clear)
 bit 14        1:      TIE, Timer interrupt flag
 bit 13-12     00:     reserved
 bit 11        1:      FREE, 1 = do not stop on emulation halt
 bit 10        0:      SOFT, don't care when FREE=1
 bit 9-6       0000:   reserved
 bit 5         1:      TRB, 1 = reload PRDH:PRD and PSCH:PSC now
 bit 4         0:      TSS, 0 = start the timer
 bit 3-0       0's:    reserved
*/

	PieCtrlRegs.PIEIER1.bit.INTx7 = 1;			// Enable TINT0 in PIE group 1
	IER |= 0x0001;								// Enable INT1 in IER to enable PIE group 1

} // end InitTimer0()


/*** end of file *****************************************************/
