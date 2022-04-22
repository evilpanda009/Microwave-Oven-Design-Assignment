/**********************************************************************
* Function: InitDac()                                                 *
* Description: Initializes an ePWM channel for the DAC driver         *
* Device: TMS320F2808, TMS320F2806, TMS320F2801                       *
* Author: David M. Alter, Texas Instruments Inc.                      *
* Function Prototype: void InitDac(Uint16, Uint16);                   *
* Useage: InitDac(channel, period);                                   *
* Input Parameters: channel = ePWM module number to be initialized    *
*                   period = desired PWM period (cycle counts - 1)    *
* Return Value: none                                                  *
* Notes:                                                              *
* 1) On F2808 and F2806 devices, 'channel' can be 1, 2, 3, or 4.      *
*    On F2801 devices, 'channel' can be 1, 2, or 3.                   *
*    There is no error checking in the function if an invalid channel *
*    is specified, and may cause a runtime software crash.            *
* 2) Asymmetric PWM is configured.                                    *
* 3) 'period' specifies the value written to the timer period         *
*    register, TBPRD.  'period' should be  determined as follows:     *
*                                                                     *
*       period = [(SYSCLKOUT frequency)/(desired PWM frequency)] - 1  *
*                                                                     *
*    For example, if 1 MHz PWM is desired on a DSP operating at       *
*    100 MHz, period should be 99.                                    *
* 4) This function uses the DSP280x Peripheral Header files, TI       *
*    literature #SPRC191.                                             *
* 5) This function accesses EALLOW protected registers.  It returns   *
*    with EALLOW disabled.                                            *
* 6) The function will set the GPIO pin mux so that the ePWMnA pin    *
*    for the selected channel is configured for ePWM function.        *
* History:                                                            *
*   02/07/06 - original (D. Alter)                                    *
**********************************************************************/
#include "DSP2802x_Device.h"					// Peripheral address definitions
#include "HRPWM_DAC.h"				// Include file specific to this project

volatile struct EPWM_REGS* epwm_regs[] = {&EPwm1Regs, &EPwm2Regs, &EPwm3Regs, &EPwm4Regs};

void InitDac(Uint16 channel, Uint16 period)
{
volatile struct EPWM_REGS *EPwmRegs;		// pointer to ePWM regs of specified channel

// Assign the pointer to the selected ePWM channel
	//EPwmRegs = (volatile struct EPWM_REGS *)((Uint32)&EPwm1Regs + (0x0040u)*(channel - 1));
   EPwmRegs = epwm_regs[channel - 1];

// Proceed with the register configurations
	EPwmRegs->TBCTL.all = 0xC003;			// Configure timer control register
// bit 15-14     11:     FREE/SOFT, 11 = ignore emulation suspend
// bit 13        0:      PHSDIR, 0 = count down after sync event
// bit 12-10     000:    CLKDIV, 000 => TBCLK = HSPCLK/1
// bit 9-7       000:    HSPCLKDIV, 000 => HSPCLK = SYSCLKOUT/1
// bit 6         0:      SWFSYNC, 0 = no software sync produced
// bit 5-4       00:     SYNCOSEL, 11 = sync-out disabled
// bit 3         0:      PRDLD, 0 = reload PRD on counter=0
// bit 2         0:      PHSEN, 0 = phase control disabled
// bit 1-0       11:     CTRMODE, 11 = timer stopped (disabled)

	EPwmRegs->TBCTR = 0x0000;			// Clear timer counter
	EPwmRegs->TBPRD = period;			// Set timer period
	EPwmRegs->TBPHS.all = 0x0000L;		// Set timer phase

	EPwmRegs->CMPCTL.all = 0x0001;		// Compare control register
// bit 15-10     0's:    reserved
// bit 9         0:      SHDWBFULL, read-only
// bit 8         0:      SHDWAFULL, read-only
// bit 7         0:      reserved
// bit 6         0:      SHDWBMODE, 0 = shadow mode, 1 = immediate mode
// bit 5         0:      reserved
// bit 4         0:      SHDWAMODE, 0 = shadow mode, 1 = immediate mode
// bit 3-2       00:     LOADBMODE, don't care
// bit 1-0       01:     LOADAMODE, 01 = load on CNT=PRD

	EPwmRegs->AQCTLA.all = 0x0021;		// Action-qualifier control register A
// bit 15-12     0000:   reserved
// bit 11-10     00:     CBD --
// bit 9-8       00:     CBU   \         00 = do nothing
// bit 7-6       00:     CAD    \______  01 = clear
// bit 5-4       10:     CAU    |        10 = set
// bit 3-2       00:     PRD   |
// bit 1-0       01:     ZRO --

	EPwmRegs->AQSFRC.all = 0x0000;		// Action-qualifier s/w force register
// bit 15-8      0's:    reserved
// bit 7-6       00:     RLDCSF, 00 = reload AQCTLx on zero
// bit 5         0:      OTSFB, 0 = do not initiate a s/w forced event on output B
// bit 4-3       00:     ACTSFB, don't care
// bit 2         0:      OTSFA, 0 = do not initiate a s/w forced event on output A
// bit 1-0       00:     ACTSFA, don't care

	EPwmRegs->AQCSFRC.all = 0x0000;		// Action-qualifier continuous s/w force register
// bit 15-4      0's:    reserved
// bit 3-2       00:     CSFB, 00 = forcing disabled
// bit 1-0       00:     CSFA, 00 = forcing disabled

// Configure the Deadband and Trip Zone modules
	asm(" EALLOW");						// Enable EALLOW protected register access
	EPwmRegs->DBCTL.bit.IN_MODE = 0;	// Deadband disabled
	EPwmRegs->DBCTL.bit.OUT_MODE = 0;	// Deadband disabled
	EPwmRegs->PCCTL.bit.CHPEN = 0;		// PWM chopper unit disabled
	EPwmRegs->TZCTL.bit.TZA = 0x3;		// Trip action disabled for output A
	EPwmRegs->TZCTL.bit.TZB = 0x3;		// Trip action disabled for output B

// Configure the EPwm HRPWM module
	EPwmRegs->HRCNFG.all = 0x0000;		// Clear all bits first
	EPwmRegs->HRCNFG.bit.EDGMODE = 1;	// Position Control: 1=Rising edge, 2=Falling edge
	EPwmRegs->HRCNFG.bit.CTLMODE = 0;	// 0 = Select compare module bus
	EPwmRegs->HRCNFG.bit.HRLOAD = 1;	// 1 = Load on CNTR=PRD

//   EPwmRegs->HRPCTL.bit.HRPE = 0;	// Disable HrPWM

	asm(" EDIS");						// Disable EALLOW protected register access

// Set the initial PWM duty cycle to zero (D/A output is zero)
	EPwmRegs->CMPA.all = 0xFFFFFF00L;	//  Set PWM duty cycle

// Enable the timer
	EPwmRegs->TBCTL.bit.CTRMODE = 0x0;	// Enable the timer in count-up mode

// Configure the pin mux for ePWMnA function on the selected channel
	asm(" EALLOW");											// Enable EALLOW protected register access
   switch(channel)
    { case 1: GpioCtrlRegs.GPAMUX1.bit.GPIO0 = 1; break;     // Set the mux bits to 01b (ePWMnA function)
      case 2: GpioCtrlRegs.GPAMUX1.bit.GPIO2 = 1; break;     // Set the mux bits to 01b (ePWMnA function)
      case 3: GpioCtrlRegs.GPAMUX1.bit.GPIO4 = 1; break;     // Set the mux bits to 01b (ePWMnA function)
      case 4: GpioCtrlRegs.GPAMUX1.bit.GPIO6 = 1; break;     // Set the mux bits to 01b (ePWMnA function)
    }
//	GpioCtrlRegs.GPAMUX1.all &= ~(3 << ((channel-1)<<2) );	// Set the mux bits to 00b
//	GpioCtrlRegs.GPAMUX1.all |=  (1 << ((channel-1)<<2) );	// Set the mux bits to 01b (ePWMnA function)
	asm(" EDIS");											// Disable EALLOW protected register access

} // end InitDac()


/*** end of file *****************************************************/
