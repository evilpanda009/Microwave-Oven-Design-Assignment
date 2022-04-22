#include "DSP28x_Project.h"     // Device Headerfile and Examples Include File
#define   MATH_TYPE      IQ_MATH
#define   GLOBAL_Q       16
#include "IQmathLib.h"

typedef struct 
 { _iq  Ref;   		// Input: Reference input 
	_iq  Fdb;   		// Input: Feedback input 
	_iq  Err;			// Variable: Error
   _iq  Kp;				// Parameter: Proportional gain
   _iq  Up;				// Variable: Proportional output 
   _iq  Ui;				// Variable: Integral output 
   _iq  Ud;				// Variable: Derivative output 	
   _iq  OutPreSat; 	// Variable: Pre-saturated output
   _iq  OutMax;		// Parameter: Maximum output 
   _iq  OutMin;	   // Parameter: Minimum output
   _iq  Out;   		// Output: PID output 
   _iq  SatErr;		// Variable: Saturated difference
   _iq  Ki;			   // Parameter: Integral gain
   _iq  Kc;		     	// Parameter: Integral correction gain
   _iq  Kd; 		   // Parameter: Derivative gain
   _iq  Up1;		   // History: Previous proportional output
 } PIDREG3;	            

#define PIDREG3_DEFAULTS {0,0,0,_IQ(1.3),0,0,0,0,_IQ(1),_IQ(-1),0,0,_IQ(0.07),_IQ(0.2),_IQ(1.05),0}
#define TCOEF  50.567901234567901234567901234568

#define TMR0_FREQ    60L
#define TMR0_PERIOD  100L

#define PWM_PERIOD   10000  						   // 6kHz when PLL is set to 0xC (60MHz) 

#define DEF_TEMPERATURE    50.0

// Prototype statements for functions found within this file.
void initialize_peripheral();
void scia_xmit(int a);
void scia_msg(char *msg);
void inttostr(char* s, Uint16 v);
char read_temperature(int temp_current, int* temp_todo);
interrupt void  adc_isr(void);
void pid_reg3_calc(PIDREG3 *v);

Uint16 temp_measured;

void main(void)
 { char *msg;
   char s[16];
   int temp_target;
   int temp_last_target;
   _iq prd;
   PIDREG3 pidstate = PIDREG3_DEFAULTS;

   initialize_peripheral();

   msg = "\r\nTdef = 50\r\nEnter target temperature...\r\n\0";
   scia_msg(msg);

   pidstate.Ref = _IQ(DEF_TEMPERATURE * TCOEF);       // for 50.0 grad: 2528.395062
   pidstate.OutMax = PWM_PERIOD-1;
   pidstate.OutMin = 1;

   temp_last_target = DEF_TEMPERATURE;
   temp_target = 0;
   temp_measured = 0;

   for(;;)
    { if(read_temperature(temp_last_target, &temp_target)) 
       { pidstate.Err = 0;
         pidstate.Up = 0;
         pidstate.Ui = 0;
         pidstate.Ud = 0;
         pidstate.SatErr = 0;
         pidstate.Ref = _IQ(temp_target * TCOEF);
         pidstate.OutMax = PWM_PERIOD-1;
         pidstate.OutMin = 1;

         temp_last_target = temp_target;
         temp_target = 0;
       }
      pidstate.Fdb = _IQ(temp_measured);
      pid_reg3_calc(&pidstate);
      EPwm2Regs.CMPA.half.CMPA = pidstate.Out;     // 1 .. PWM_PERIOD-1
    }
 }

// Transmit a character from the SCI
void scia_xmit(int a)
 { while (SciaRegs.SCIFFTX.bit.TXFFST != 0) {}
   SciaRegs.SCITXBUF=a;
 }

void scia_msg(char * msg)
 { int i;
   for(i = 0; msg[i] != '\0'; i++) scia_xmit(msg[i]);
 }

//===========================================================================
// No more.
//===========================================================================

void inttostr(char* s, Uint16 v)
 { Uint16 tmp;
   unsigned char i;
   for (i = 0; i < 4; i++, s++)
    { tmp = (v & 0xf000) >> 12;
      *s = tmp > 9 ? (tmp-9) + 'a' : tmp + '0';
      v <<= 4;
    } 
   *s = 0;
 }

char read_temperature(int temp_current, int* temp_todo)
 { int value = 0;
   Uint16 ch = 0; 
   enum {CH_ESCAPE = 0x1c, CH_ENTER=0x0d};  
   if (SciaRegs.SCIFFRX.bit.RXFFST)
    { ch = SciaRegs.SCIRXBUF.all;
      if (ch == CH_ESCAPE)
       { *temp_todo = temp_current;
         return 1;
       }
      if (ch == CH_ENTER)
         return 1;
      if (ch >= '0' && ch <= '9')
       { *temp_todo = *temp_todo * 10 + (ch - '0');
       } 
    }
   return 0;
 }


interrupt void  adc_isr(void)
 { temp_measured = AdcResult.ADCRESULT0;
   AdcRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;		//Clear ADCINT1 flag reinitialize for next SOC
   PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;   // Acknowledge interrupt to PIE
   return;
 }


void pid_reg3_calc(PIDREG3 *v)
 { // Compute the error
   v->Err = v->Ref - v->Fdb;
   // Compute the proportional output
   v->Up = _IQmpy(v->Kp,v->Err);
   // Compute the integral output
   v->Ui = v->Ui + _IQmpy(v->Ki,v->Up) + _IQmpy(v->Kc,v->SatErr);
   // Compute the derivative output
   v->Ud = _IQmpy(v->Kd,(v->Up - v->Up1));
   // Compute the pre-saturated output
   v->OutPreSat = v->Up + v->Ui + v->Ud;
   // Saturate the output
   if (v->OutPreSat > v->OutMax)
      v->Out =  v->OutMax;
   else if (v->OutPreSat < v->OutMin)
      v->Out =  v->OutMin;
   else
      v->Out = v->OutPreSat;
   // Compute the saturate difference
   v->SatErr = v->Out - v->OutPreSat;
   // Update the previous proportional output
   v->Up1 = v->Up; 
 }

void initialize_peripheral()
 { 
// PLL, WatchDog, enable Peripheral Clocks
   InitSysCtrl();

// Clear all interrupts and initialize PIE vector table:
   DINT;
// Initialize PIE control registers to their default state.
// The default state is all PIE interrupts disabled and flags are cleared.
   InitPieCtrl();

   IER = 0x0000;
   IFR = 0x0000;

   InitPieVectTable();

   EALLOW;
// 1. PERIPHERAL CLOCK ENABLES 
//---------------------------------------------------
// If you are not using a peripheral you may want to switch
// the clock off to save power, i.e. set to =0 
   SysCtrlRegs.PCLKCR0.bit.ADCENCLK = 1;     // ADC
   SysCtrlRegs.PCLKCR3.bit.GPIOINENCLK = 0;	// GPIO
   SysCtrlRegs.PCLKCR3.bit.COMP1ENCLK = 0;	// COMP1
   SysCtrlRegs.PCLKCR3.bit.COMP2ENCLK = 0;	// COMP2
   SysCtrlRegs.PCLKCR0.bit.I2CAENCLK = 0;    // I2C
   SysCtrlRegs.PCLKCR0.bit.SPIAENCLK = 0;	   // SPI-A
   SysCtrlRegs.PCLKCR0.bit.SCIAENCLK = 1;  	// SCI-A
   SysCtrlRegs.PCLKCR1.bit.ECAP1ENCLK = 0;	// eCAP1
   SysCtrlRegs.PCLKCR1.bit.EPWM1ENCLK = 0;   // ePWM1
   SysCtrlRegs.PCLKCR1.bit.EPWM2ENCLK = 1;   // ePWM2
   SysCtrlRegs.PCLKCR1.bit.EPWM3ENCLK = 0;   // ePWM3
   SysCtrlRegs.PCLKCR1.bit.EPWM4ENCLK = 0;   // ePWM4
   SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 1;    // Enable TBCLK


// 2. Timer 0
   CpuTimer0Regs.PRD.all = (long) (TMR0_FREQ * TMR0_PERIOD);
	CpuTimer0Regs.TPR.all  = 0;         // Set pre-scale counter to divide by 1 (SYSCLKOUT):
	CpuTimer0Regs.TPRH.all  = 0;
	CpuTimer0Regs.TCR.bit.TSS = 1;      // 1 = Stop timer, 0 = Start/Restart Timer
	CpuTimer0Regs.TCR.bit.TRB = 1;      // 1 = reload timer
	CpuTimer0Regs.TCR.bit.SOFT = 0;
	CpuTimer0Regs.TCR.bit.FREE = 0;     // Timer Free Run Disabled
	CpuTimer0Regs.TCR.bit.TIE = 1;      // 0 = Disable/ 1 = Enable Timer Interrupt
   CpuTimer0Regs.TCR.all = 0x4001;     // Use write-only instruction to set TSS bit = 0

// 2. ADC
   AdcRegs.ADCCTL1.bit.ADCREFSEL = 0;      // Select interal BG
   AdcRegs.ADCCTL1.bit.ADCBGPWD  = 1;      // Power ADC BG
   AdcRegs.ADCCTL1.bit.ADCREFPWD = 1;      // Power reference
   AdcRegs.ADCCTL1.bit.ADCPWDN   = 1;      // Power ADC
   AdcRegs.ADCCTL1.bit.ADCENABLE = 1;      // Enable ADC
asm(" RPT#100 || NOP");
	AdcRegs.ADCCTL1.bit.INTPULSEPOS=1;	    //ADCINT1 trips after AdcResults latch
	AdcRegs.ADCSOC0CTL.bit.ACQPS 	= 6;	    //set SOC0 S/H Window to 7 ADC Clock Cycles, (6 ACQPS plus 1)
	AdcRegs.INTSEL1N2.bit.INT1SEL	= 0;	    //setup EOC0 to trigger ADCINT1 to fire
	AdcRegs.INTSEL1N2.bit.INT1CONT= 0;	    //Disable ADCINT1 Continuous mode
	AdcRegs.INTSEL1N2.bit.INT1E   = 1;	    //Enabled ADCINT1
	AdcRegs.ADCSOC0CTL.bit.CHSEL 	= 4;	    //set SOC0 channel select to ADCINA4
	AdcRegs.ADCSOC0CTL.bit.TRIGSEL= 1;	    //set SOC0 start trigger on Timer0
   GpioCtrlRegs.AIOMUX1.bit.AIO4 = 2;      // Configure AIO4 for A4 (analog input) operation

   PieVectTable.ADCINT1 = &adc_isr;
	EDIS;

// 3. SCI
   SciaRegs.SCICCR.all =0x0007;   // 1 stop bit,  No loopback, No parity,8 char bits,, async mode, idle-line protocol
   SciaRegs.SCICTL1.all =0x0003;  // Enable: TX, RX, internal SCICLK. Disable: RX ERR, SLEEP, TXWAKE
   SciaRegs.SCICTL2.all =0x0003;
   SciaRegs.SCICTL2.bit.TXINTENA =1;
   SciaRegs.SCICTL2.bit.RXBKINTENA =1;
   SciaRegs.SCIHBAUD    =0x0000;  // 9600 baud @LSPCLK = 15MHz (60 MHz SYSCLK).
   SciaRegs.SCILBAUD    =0x00C2;
//   SciaRegs.SCILBAUD    =0x0020; // 57600 baud @LSPCLK = 15MHz (60 MHz SYSCLK).
   SciaRegs.SCICTL1.all =0x0023;  // Relinquish SCI from Reset
   SciaRegs.SCIFFTX.all=0xE040;
   SciaRegs.SCIFFRX.all=0x2044;
   SciaRegs.SCIFFCT.all=0x0;

// 4. PWM
// B-channel will be configured as example but will not be used
	EPwm2Regs.TBPRD = PWM_PERIOD;   		         // Set timer period, PWM frequency = 1 / period
   EPwm2Regs.TBPHS.all = 0;				         // Time-Base Phase Register
   EPwm2Regs.TBCTR = 0;					            // Time-Base Counter Register	
   EPwm2Regs.TBCTL.bit.PRDLD = TB_IMMEDIATE;    // Set Immediate load
   EPwm2Regs.TBCTL.bit.CTRMODE = TB_COUNT_UP;   // Count-up mode: used for asymmetric PWM
	EPwm2Regs.TBCTL.bit.PHSEN = TB_DISABLE;	   // Disable phase loading
	EPwm2Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_DISABLE;
	EPwm2Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;
	EPwm2Regs.TBCTL.bit.CLKDIV = TB_DIV1;

  	// Setup shadow register load on ZERO
  	EPwm2Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
  	EPwm2Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
  	EPwm2Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;	// load on CTR=Zero
  	EPwm2Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;	// load on CTR=Zero

  	// Set Compare values
  	EPwm2Regs.CMPA.half.CMPA = 1;               // Set duty 0% initially
  	EPwm2Regs.CMPB = PWM_PERIOD/2;	           // Set duty 50% initially

  	// Set actions
  	EPwm2Regs.AQCTLA.bit.ZRO = AQ_SET;          // Set PWM2A on Zero
  	EPwm2Regs.AQCTLA.bit.CAU = AQ_CLEAR;        // Clear PWM2A on event A, up count
  	EPwm2Regs.AQCTLB.bit.ZRO = AQ_CLEAR;        // Set PWM2B on Zero
  	EPwm2Regs.AQCTLB.bit.CBU = AQ_SET;          // Clear PWM2B on event B, up count

   EALLOW;

// 5. GPIO
//--------------------------------------------------------------------------------------
//  GPIO-02 - PIN FUNCTION = --Spare--
	GpioCtrlRegs.GPAMUX1.bit.GPIO2 = 1;		// 0=GPIO,  1=EPWM2A,  2=Resv,  3=Resv
	GpioCtrlRegs.GPADIR.bit.GPIO2 = 1;		// 1=OUTput,  0=INput 
//--------------------------------------------------------------------------------------
//  GPIO-28 - PIN FUNCTION = --Spare--
	GpioCtrlRegs.GPAMUX2.bit.GPIO28 = 1;	// 0=GPIO,  1=SCIRX-A,  2=I2C-SDA,  3=TZ2
	GpioCtrlRegs.GPADIR.bit.GPIO28 = 0;		// 1=OUTput,  0=INput 
   GpioCtrlRegs.GPAPUD.bit.GPIO28 = 0;    // Enable pull-up for GPIO28 (SCIRXDA)
	GpioCtrlRegs.GPAQSEL2.bit.GPIO28 = 3;  // Async input GPIO28 (SCIRXDA)
//--------------------------------------------------------------------------------------
//  GPIO-29 - PIN FUNCTION = --Spare--
	GpioCtrlRegs.GPAMUX2.bit.GPIO29 = 1;	// 0=GPIO,  1=SCITXD-A,  2=I2C-SCL,  3=TZ3
	GpioCtrlRegs.GPADIR.bit.GPIO29 = 1;		// 1=OUTput,  0=INput 
   GpioCtrlRegs.GPAPUD.bit.GPIO29 = 1;	   // Disable pull-up for GPIO29 (SCITXDA)
//--------------------------------------------------------------------------------------
//  GPIO-34 - PIN FUNCTION = LED for F28027 USB dongle
	GpioCtrlRegs.GPBMUX1.bit.GPIO34 = 0;	// 0=GPIO,  1=COMP2OUT,  2=EMU1,  3=Resv
	GpioCtrlRegs.GPBDIR.bit.GPIO34 = 1;		// 1=OUTput,  0=INput 
	GpioDataRegs.GPBSET.bit.GPIO34 = 1;		// uncomment if --> Set High initially
//--------------------------------------------------------------------------------------
	EDIS;

   PieCtrlRegs.PIEIER1.bit.INTx1 = 1;	// Enable INT 1.1 in the PIE
   IER |= M_INT1;
   EINT;   // Enable Global interrupt INTM
 }

