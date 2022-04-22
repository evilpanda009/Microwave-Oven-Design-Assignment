#include "DSP28x_Project.h"     // Device Headerfile and Examples Include File

// ECCTL1 ( ECAP Control Reg 1) 
//========================== 
// CAPxPOL bits 
#define EC_RISING       0x0 
#define EC_FALLING      0x1 
// CTRRSTx bits 
#define EC_ABS_MODE     0x0 
#define EC_DELTA_MODE   0x1 
// PRESCALE bits 
#define EC_BYPASS       0x0 
#define EC_DIV1         0x0     
#define EC_DIV2         0x1 
#define EC_DIV4         0x2
#define EC_DIV6         0x3 
#define EC_DIV8         0x4 
#define EC_DIV10        0x5 
#define EC_DIV60        0x1e 
// ECCTL2 ( ECAP Control Reg 2)
//========================== 
// CONT/ONESHOT bit 
#define EC_CONTINUOUS   0x0 
#define EC_ONESHOT      0x1
// STOPVALUE bit 
#define EC_EVENT1       0x0 
#define EC_EVENT2       0x1 
#define EC_EVENT3       0x2 
#define EC_EVENT4       0x3 
// RE-ARM bit 
#define EC_ARM          0x1 
// TSCTRSTOP bit 
#define EC_FREEZE       0x0 
#define EC_RUN          0x1 
// SYNCO_SEL bit 
#define EC_SYNCIN       0x0 
#define EC_CTR_PRD      0x1 
#define EC_SYNCO_DIS    0x2
// CAP/APWM mode bit 
#define EC_CAP_MODE     0x0 
#define EC_APWM_MODE    0x1 
// APWMPOL bit 
#define EC_ACTV_HI      0x0 
#define EC_ACTV_LO      0x1 
// Generic 
#define EC_DISABLE      0x0 
#define EC_ENABLE       0x1
#define EC_FORCE        0x1

// Prototype statements for functions found within this file.
interrupt void ecap1_isr(void);
void InitECapture(void);
void InitGpioCtrls(void);
void display(Uint32 v);

// Global variables used in this example
Uint32 period; 
Uint32 freq;
char should_recalc;

void main(void)
{

// Step 1. Initialize System Control:
// PLL, WatchDog, enable Peripheral Clocks
// This example function is found in the DSP2802x_SysCtrl.c file.
   InitSysCtrl();

// Step 2. Initalize GPIO:
// This example function is found in the DSP2802x_Gpio.c file and
// illustrates how to set the GPIO to it's default state.
// InitGpio();  // Skipped for this example

   InitEPwm3Gpio();
   InitGpioCtrls();
   InitECap1Gpio();

// Step 3. Clear all interrupts and initialize PIE vector table:
// Disable CPU interrupts
   DINT;

// Initialize the PIE control registers to their default state.
// The default state is all PIE interrupts disabled and flags
// are cleared.
// This function is found in the DSP2802x_PieCtrl.c file.
   InitPieCtrl();

// Disable CPU interrupts and clear all CPU interrupt flags:
   IER = 0x0000;
   IFR = 0x0000;

// Initialize the PIE vector table with pointers to the shell Interrupt
// Service Routines (ISR).
// This will populate the entire table, even if the interrupt
// is not used in this example.  This is useful for debug purposes.
// The shell ISR routines are found in DSP2802x_DefaultIsr.c.
// This function is found in DSP2802x_PieVect.c.
   InitPieVectTable();

// Interrupts that are used in this example are re-mapped to
// ISR functions found within this file.
   EALLOW;  // This is needed to write to EALLOW protected registers
   PieVectTable.ECAP1_INT = &ecap1_isr;
   EDIS;    // This is needed to disable write to EALLOW protected registers

// Step 4. Initialize all the Device Peripherals:
// This function is found in DSP2802x_InitPeripherals.c
// InitPeripherals();  // Not required for this example
//   InitEPwmTimer();    // For this example, only initialize the EPwm Timers
   InitECapture();

// Step 5. User specific code, enable interrupts:

// Enable CPU INT4 which is connected to ECAP1-4 INT:
   IER |= M_INT4;

// Enable eCAP INTn in the PIE: Group 3 interrupt 1-6
   PieCtrlRegs.PIEIER4.bit.INTx1 = 1;

// PERIPHERAL CLOCK ENABLES 
//---------------------------------------------------
// If you are not using a peripheral you may want to switch
// the clock off to save power, i.e. set to =0 
   EALLOW;
   SysCtrlRegs.PCLKCR0.bit.ADCENCLK = 0;     // ADC
   SysCtrlRegs.PCLKCR3.bit.GPIOINENCLK = 0;	// GPIO
   SysCtrlRegs.PCLKCR3.bit.COMP1ENCLK = 0;	// COMP1
   SysCtrlRegs.PCLKCR3.bit.COMP2ENCLK = 0;	// COMP2
   SysCtrlRegs.PCLKCR0.bit.I2CAENCLK = 0;    // I2C
   SysCtrlRegs.PCLKCR0.bit.SPIAENCLK = 0;	   // SPI-A
   SysCtrlRegs.PCLKCR0.bit.SCIAENCLK = 0;  	// SCI-A
   SysCtrlRegs.PCLKCR1.bit.ECAP1ENCLK = 1;	// eCAP1
   SysCtrlRegs.PCLKCR1.bit.EPWM1ENCLK = 0;   // ePWM1
   SysCtrlRegs.PCLKCR1.bit.EPWM2ENCLK = 0;   // ePWM2
   SysCtrlRegs.PCLKCR1.bit.EPWM3ENCLK = 0;   // ePWM3
   SysCtrlRegs.PCLKCR1.bit.EPWM4ENCLK = 0;   // ePWM4
   SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 0;    // Enable TBCLK
   EDIS;

// Enable global Interrupts and higher priority real-time debug events:
   EINT;   // Enable Global interrupt INTM
   ERTM;   // Enable Global realtime interrupt DBGM

// Step 6. IDLE loop. Just sit and loop forever (optional):
   period = 0; 
   should_recalc = 0;
   for(;;)
   {
      if (should_recalc)
      { should_recalc = 0;
        freq = 60000000 / period;
        display(freq);  
      }
   }
}

void InitECapture()
{
/* ECap1Regs.ECEINT.all = 0x0000;             // Disable all capture interrupts
   ECap1Regs.ECCLR.all = 0xFFFF;              // Clear all CAP interrupt flags
   ECap1Regs.ECCTL1.bit.CAPLDEN = 0;          // Disable CAP1-CAP4 register loads
   ECap1Regs.ECCTL2.bit.TSCTRSTOP = 0;        // Make sure the counter is stopped

   // Configure peripheral registers
   ECap1Regs.ECCTL2.bit.CONT_ONESHT = 1;      // One-shot
   ECap1Regs.ECCTL2.bit.STOP_WRAP = 3;        // Stop at 4 events
   ECap1Regs.ECCTL1.bit.CAP1POL = 1;          // Falling edge
   ECap1Regs.ECCTL1.bit.CAP2POL = 0;          // Rising edge
   ECap1Regs.ECCTL1.bit.CAP3POL = 1;          // Falling edge
   ECap1Regs.ECCTL1.bit.CAP4POL = 0;          // Rising edge
   ECap1Regs.ECCTL1.bit.CTRRST1 = 1;          // Difference operation
   ECap1Regs.ECCTL1.bit.CTRRST2 = 1;          // Difference operation
   ECap1Regs.ECCTL1.bit.CTRRST3 = 1;          // Difference operation
   ECap1Regs.ECCTL1.bit.CTRRST4 = 1;          // Difference operation
   ECap1Regs.ECCTL2.bit.SYNCI_EN = 1;         // Enable sync in
   ECap1Regs.ECCTL2.bit.SYNCO_SEL = 0;        // Pass through
   ECap1Regs.ECCTL1.bit.CAPLDEN = 1;          // Enable capture units

   ECap1Regs.ECCTL2.bit.TSCTRSTOP = 1;        // Start Counter
   ECap1Regs.ECCTL2.bit.REARM = 1;            // arm one-shot
   ECap1Regs.ECCTL1.bit.CAPLDEN = 1;          // Enable CAP1-CAP4 register loads
   ECap1Regs.ECEINT.bit.CEVT4 = 1;            // 4 events = interrupt
*/

   ECap1Regs.ECCLR.all = 0xFFFF;              // Clear all CAP interrupt flags
   ECap1Regs.ECCTL1.bit.CAPLDEN = 0;          // Disable CAP1-CAP4 register loads
   ECap1Regs.ECCTL2.bit.TSCTRSTOP = 0;        // Make sure the counter is stopped

// ECAP module 1 config 
   ECap1Regs.ECCTL1.bit.CAP1POL = EC_RISING;
   ECap1Regs.ECCTL1.bit.CAP2POL = EC_RISING;
   ECap1Regs.ECCTL1.bit.CTRRST1 = EC_DELTA_MODE;
   ECap1Regs.ECCTL1.bit.CTRRST2 = EC_DELTA_MODE; 
   ECap1Regs.ECCTL1.bit.CAPLDEN = EC_ENABLE;
//   ECap1Regs.ECCTL1.bit.PRESCALE = EC_DIV1; 
   ECap1Regs.ECCTL1.bit.PRESCALE = EC_DIV1;
   ECap1Regs.ECCTL2.bit.CAP_APWM = EC_CAP_MODE;
   ECap1Regs.ECCTL2.bit.CONT_ONESHT = EC_CONTINUOUS; 
   ECap1Regs.ECCTL2.bit.SYNCO_SEL = EC_SYNCO_DIS;
   ECap1Regs.ECCTL2.bit.SYNCI_EN = EC_DISABLE; 
//   ECap1Regs.ECCTL2.bit.STOP_WRAP = 1;        // Stop at 2 events
   ECap1Regs.ECCTL2.bit.STOP_WRAP = 0;        // Stop at 1 event
//   ECap1Regs.ECEINT.bit.CEVT2 = 1;            // 2 events = interrupt
   ECap1Regs.ECEINT.bit.CEVT1 = 1;            // 1 event = interrupt
   ECap1Regs.ECCTL2.bit.TSCTRSTOP = EC_RUN;   // Allow TSCTR to run 
}

interrupt void ecap1_isr(void)
{
//   period = ECap1Regs.CAP2; 
   period = ECap1Regs.CAP1; 
   should_recalc = 1;
//   ECap1Regs.ECCLR.bit.CEVT2 = 1;
   ECap1Regs.ECCLR.bit.CEVT1 = 1;
   ECap1Regs.ECCLR.bit.INT = 1;

   // Acknowledge this interrupt to receive more interrupts from group 4
   PieCtrlRegs.PIEACK.all = PIEACK_GROUP4;
}

void InitGpioCtrls(void)
{
   EALLOW;
   GpioCtrlRegs.GPAMUX1.all = 0x0000;     // GPIO functionality GPIO0-GPIO15
   GpioCtrlRegs.GPAMUX2.all = 0x0000;     // GPIO functionality GPIO16-GPIO31
   GpioCtrlRegs.GPBMUX1.all = 0x0000;     // GPIO functionality GPIO32-GPIO34
   GpioCtrlRegs.AIOMUX1.all = 0x0000;     // Dig.IO funct. applies to AIO2,4,6,10,12,14

   GpioCtrlRegs.GPADIR.all = 0xFFFFFFFF;      // GPIO0-GPIO7 are GP outputs, GPIO8-GPIO31 are outputs
   GpioCtrlRegs.GPBDIR.all = 0xFFFFFFFF;      // GPIO32-GPIO35 are outputs
   GpioCtrlRegs.AIODIR.all = 0x00000000;      // AIO2,4,6,19,12,14 are digital inputs
 }

void display(Uint32 v)
 { // 1-7,12,16,17,18 - GPA; 32,33,34,35 - GPB
   int ch = v % 10;
   v /= 10;
   if (ch & 0x01)
      GpioDataRegs.GPASET.bit.GPIO0 = 1;
   else
      GpioDataRegs.GPACLEAR.bit.GPIO0 = 1;
   if (ch & 0x02)
      GpioDataRegs.GPASET.bit.GPIO1 = 1;
   else
      GpioDataRegs.GPACLEAR.bit.GPIO1 = 1;
  if (ch & 0x04)
      GpioDataRegs.GPASET.bit.GPIO2 = 1;
   else
      GpioDataRegs.GPACLEAR.bit.GPIO2 = 1;
  if (ch & 0x08)
      GpioDataRegs.GPASET.bit.GPIO3 = 1;
   else
      GpioDataRegs.GPACLEAR.bit.GPIO3 = 1;

   ch = v % 10;
   v /= 10;
   if (ch & 0x01)
      GpioDataRegs.GPASET.bit.GPIO4 = 1;
   else
      GpioDataRegs.GPACLEAR.bit.GPIO4 = 1;
   if (ch & 0x02)
      GpioDataRegs.GPASET.bit.GPIO5 = 1;
   else
      GpioDataRegs.GPACLEAR.bit.GPIO5 = 1;
  if (ch & 0x04)
      GpioDataRegs.GPASET.bit.GPIO6 = 1;
   else
      GpioDataRegs.GPACLEAR.bit.GPIO6 = 1;
  if (ch & 0x08)
      GpioDataRegs.GPASET.bit.GPIO7 = 1;
   else
      GpioDataRegs.GPACLEAR.bit.GPIO7 = 1;

   ch = v % 10;
   v /= 10;
   if (ch & 0x01)
      GpioDataRegs.GPASET.bit.GPIO12 = 1;
   else
      GpioDataRegs.GPACLEAR.bit.GPIO12 = 1;
   if (ch & 0x02)
      GpioDataRegs.GPASET.bit.GPIO16 = 1;
   else
      GpioDataRegs.GPACLEAR.bit.GPIO16 = 1;
  if (ch & 0x04)
      GpioDataRegs.GPASET.bit.GPIO17 = 1;
   else
      GpioDataRegs.GPACLEAR.bit.GPIO17 = 1;
  if (ch & 0x08)
      GpioDataRegs.GPASET.bit.GPIO18 = 1;
   else
      GpioDataRegs.GPACLEAR.bit.GPIO18 = 1;

   ch = v % 10;
   if (ch & 0x1)
      GpioDataRegs.GPBSET.bit.GPIO32 = 1;
   else
      GpioDataRegs.GPBCLEAR.bit.GPIO32 = 1;
   if (ch & 0x2)
      GpioDataRegs.GPBSET.bit.GPIO33 = 1;
   else
      GpioDataRegs.GPBCLEAR.bit.GPIO33 = 1;
  if (ch & 0x4)
      GpioDataRegs.GPBSET.bit.GPIO34 = 1;
   else
      GpioDataRegs.GPBCLEAR.bit.GPIO34 = 1;
  if (ch & 0x8)
      GpioDataRegs.GPBSET.bit.GPIO35 = 1;
   else
      GpioDataRegs.GPBCLEAR.bit.GPIO35 = 1;
}