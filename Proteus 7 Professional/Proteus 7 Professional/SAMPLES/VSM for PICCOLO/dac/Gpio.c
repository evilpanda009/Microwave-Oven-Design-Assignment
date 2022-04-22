/**********************************************************************
* Function: InitGpio()                                                *
* Description: Initializes the shared GPIO pins on the DSP.           *
* Device: TMS320F2808, TMS320F2806, TMS320F2801                       *
* Author: David M. Alter, Texas Instruments Inc.                      *
* Function Prototype: void InitGpio(void);                            *
* Useage: InitGpio();                                                 *
* Input Parameters: none                                              *
* Return Value: none                                                  *
* Notes:                                                              *
* 1) The function configures all pins as GPIO inputs.  The user can   *
*    modify the function to configured selected pins differently.     *
* 2) The internal pullups are enabled on all pins that have them.     *
* 3) Input qualification is disabled for all pins.                    *
* 4) This function uses the DSP280x Peripheral Header files, TI       *
*    literature #SPRC191.                                             *
* History:                                                            *
*   02/07/06 - original (D. Alter)                                    *
**********************************************************************/
#include "DSP2802x_Device.h"					// Peripheral address definitions
#include "HRPWM_DAC.h"				// Include file specific to this project

void InitGpio(void)
{
	asm(" EALLOW");								// Enable EALLOW protected register access

/*** Group A pins ***/
	GpioCtrlRegs.GPACTRL.all = 0x0000;			// QUALPRD = SYSCLKOUT for all group A GPIO
	GpioCtrlRegs.GPAQSEL1.all = 0x0000;			// No qualification for all group A GPIO 0-15
	GpioCtrlRegs.GPAQSEL2.all = 0x0000;			// No qualification for all group A GPIO 16-31
	GpioCtrlRegs.GPADIR.all = 0x0000;			// All group A GPIO are inputs
	GpioCtrlRegs.GPAPUD.all = 0x0000;			// All group A pullups enabled

	GpioCtrlRegs.GPAMUX1.bit.GPIO0  = 0;		// 0=GPIO, 1=EPWM1A    2=rsvd       3=rsvd
	GpioCtrlRegs.GPAMUX1.bit.GPIO1  = 0;		// 0=GPIO, 1=EPWM1B    2=SPISIMOD   3=rsvd
	GpioCtrlRegs.GPAMUX1.bit.GPIO2  = 0;		// 0=GPIO, 1=EPWM2A    2=rsvd       3=rsvd
	GpioCtrlRegs.GPAMUX1.bit.GPIO3  = 0;		// 0=GPIO, 1=EPWM2B    2=SPISOMID   3=rsvd
	GpioCtrlRegs.GPAMUX1.bit.GPIO4  = 0;		// 0=GPIO, 1=EPWM3A    2=rsvd       3=rsvd
	GpioCtrlRegs.GPAMUX1.bit.GPIO5  = 0;		// 0=GPIO, 1=EPWM3B    2=SPICLKD    3=ECAP1
	GpioCtrlRegs.GPAMUX1.bit.GPIO6  = 0;		// 0=GPIO, 1=EPWM4A    2=EPWMSYNCI  3=EPWMSYNCO
	GpioCtrlRegs.GPAMUX1.bit.GPIO7  = 0;		// 0=GPIO, 1=EPWM4B    2=SPISTED    3=ECAP2
	GpioCtrlRegs.GPAMUX1.bit.GPIO8  = 0;		// 0=GPIO, 1=EPWM5A    2=CANTXB     3=ADCSOCAO
	GpioCtrlRegs.GPAMUX1.bit.GPIO9  = 0;		// 0=GPIO, 1=EPWM5B    2=SCITXB     3=ECAP3
	GpioCtrlRegs.GPAMUX1.bit.GPIO10 = 0;		// 0=GPIO, 1=EPWM6A    2=CANRXB     3=ADCSOCBO
	GpioCtrlRegs.GPAMUX1.bit.GPIO11 = 0;		// 0=GPIO, 1=EPWM6B    2=SCIRXB     3=ECAP4
	GpioCtrlRegs.GPAMUX1.bit.GPIO12 = 0;		// 0=GPIO, 1=TZ1       2=CANTXB     3=SPISIMOB
	GpioCtrlRegs.GPAMUX1.bit.GPIO13 = 0;		// 0=GPIO, 1=TZ2       2=CANRXB     3=SPISOMIB
	GpioCtrlRegs.GPAMUX1.bit.GPIO14 = 0;		// 0=GPIO, 1=TZ3       2=SCITXB     3=SPICLKB
	GpioCtrlRegs.GPAMUX1.bit.GPIO15 = 0;		// 0=GPIO, 1=TZ4       2=SCIRXB     3=SPISTEB
	GpioCtrlRegs.GPAMUX2.bit.GPIO16 = 0;		// 0=GPIO, 1=SPISIMOA  2=CANTXB     3=TZ5
	GpioCtrlRegs.GPAMUX2.bit.GPIO17 = 0;		// 0=GPIO, 1=SPISOMIA  2=CANRXB     3=TZ6
	GpioCtrlRegs.GPAMUX2.bit.GPIO18 = 0;		// 0=GPIO, 1=SPICLKA   2=SCITXB     3=rsvd
	GpioCtrlRegs.GPAMUX2.bit.GPIO19 = 0;		// 0=GPIO, 1=SPISTEA   2=SCIRXB     3=rsvd
	GpioCtrlRegs.GPAMUX2.bit.GPIO20 = 0;		// 0=GPIO, 1=EQEP1A    2=SPISIMOC   3=CANTXB
	GpioCtrlRegs.GPAMUX2.bit.GPIO21 = 0;		// 0=GPIO, 1=EQEP1B    2=SPISOMIC   3=CANRXB
	GpioCtrlRegs.GPAMUX2.bit.GPIO22 = 0;		// 0=GPIO, 1=EQEP1S    2=SPICLKC    3=SCITXB
	GpioCtrlRegs.GPAMUX2.bit.GPIO23 = 0;		// 0=GPIO, 1=EQEP1I    2=SPISTEC    3=SCIRXB
	GpioCtrlRegs.GPAMUX2.bit.GPIO24 = 0;		// 0=GPIO, 1=ECAP1     2=EQEP2A     3=SPISIMOB
	GpioCtrlRegs.GPAMUX2.bit.GPIO25 = 0;		// 0=GPIO, 1=ECAP2     2=EQEP2B     3=SPISOMIB
	GpioCtrlRegs.GPAMUX2.bit.GPIO26 = 0;		// 0=GPIO, 1=ECAP3     2=EQEP2I     3=SPICLKB
	GpioCtrlRegs.GPAMUX2.bit.GPIO27 = 0;		// 0=GPIO, 1=ECAP4     2=EQEP2S     3=SPISTEB
	GpioCtrlRegs.GPAMUX2.bit.GPIO28 = 0;		// 0=GPIO, 1=SCIRXDA   2=rsvd       3=TZ5
	GpioCtrlRegs.GPAMUX2.bit.GPIO29 = 0;		// 0=GPIO, 1=SCITXDA   2=rsvd       3=TZ6
	GpioCtrlRegs.GPAMUX2.bit.GPIO30 = 0;		// 0=GPIO, 1=CANRXA    2=rsvd       3=rsvd
	GpioCtrlRegs.GPAMUX2.bit.GPIO31 = 0;		// 0=GPIO, 1=CANTXA    2=rsvd       3=rsvd

/*** Group B pins ***/
	GpioCtrlRegs.GPBCTRL.all = 0x0000;			// QUALPRD = SYSCLKOUT for all group B GPIO
	GpioCtrlRegs.GPBQSEL1.all = 0x0000;			// No qualification for all group B GPIO 32-35
											    // GPBQSEL2 is reserved on F2808/06/01 devices
	GpioCtrlRegs.GPBDIR.all = 0x0000;			// All group B GPIO are inputs
	GpioCtrlRegs.GPBPUD.all = 0x0000;			// All group B pullups enabled

	GpioCtrlRegs.GPBMUX1.bit.GPIO32 = 0;		// 0=GPIO, 1=SDAA      2=EPWMSYNCI  3=ADCSOCAO
	GpioCtrlRegs.GPBMUX1.bit.GPIO33 = 0;		// 0=GPIO, 1=SCLA      2=EPWMSYNCO  3=ADCSOCBO
	GpioCtrlRegs.GPBMUX1.bit.GPIO34 = 0;		// 0=GPIO, 1=rsvd      2=rsvd       3=rsvd

/*** Finish up ***/
	asm(" EDIS");								// Disable EALLOW protected register access

} // end InitGpio()


/*** end of file *****************************************************/
