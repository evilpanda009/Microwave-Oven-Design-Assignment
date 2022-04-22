//    This program will write 1-14 words to EEPROM and read them back.
//    The data written and the EEPROM address written to are contained
//    in the message structure, I2cMsgOut1. The data read back will be
//    contained in the message structure I2cMsgIn1.
//
//    This program will work with the on-board I2C EEPROM supplied on
//    the F2802x eZdsp.
//
//
//###########################################################################

#include "DSP28x_Project.h"     // Device Headerfile and Examples Include File

// Note: I2C Macros used in this example can be found in the
// DSP2802x_I2C_defines.h file

// Prototype statements for functions found within this file.
void   I2CA_Init(void);
interrupt void i2c_int1a_isr(void);
void InitGpioCtrls(void);
void set_7seg_data(char ch);
char get_res_data();

#define I2C_SLAVE_ADDR        0xC0 >> 1
#define I2C_NUMBYTES          2
#define I2C_EEPROM_HIGH_ADDR  0x00
#define I2C_EEPROM_LOW_ADDR   0x30

char dt_tx;
char dt_rx;
char dt_rdy;

void main(void)
{
// Step 1. Initialize System Control:
// PLL, WatchDog, enable Peripheral Clocks
// This example function is found in the DSP2802x_SysCtrl.c file.
   InitSysCtrl();

// Step 2. Initalize GPIO:
// This example function is found in the DSP2802x_Gpio.c file and
// illustrates how to set the GPIO to it's default state.
   InitGpioCtrls();
// Setup only the GP I/O only for I2C functionality
   InitI2CGpio();

// Step 3. Clear all interrupts and initialize PIE vector table:
// Disable CPU interrupts
   DINT;

// Initialize PIE control registers to their default state.
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
   PieVectTable.I2CINT1A = &i2c_int1a_isr;
   EDIS;   // This is needed to disable write to EALLOW protected registers

// Step 4. Initialize all the Device Peripherals:
// This function is found in DSP2802x_InitPeripherals.c
// InitPeripherals(); // Not required for this example
   I2CA_Init();

// Step 5. User specific code

// Enable interrupts required for this example

// Enable I2C interrupt 1 in the PIE: Group 8 interrupt 1
   PieCtrlRegs.PIEIER8.bit.INTx1 = 1;

// Enable CPU INT8 which is connected to PIE group 8
   IER |= M_INT8;
   EINT;

   dt_rdy = 0;
   dt_rx = 0;

   set_7seg_data(0);

   // Application loop
   for(;;)
   { if (dt_rdy)
      { dt_rdy = 0;
        set_7seg_data(dt_rx); 
      }
     if (I2caRegs.I2CSTR.bit.XRDY)
      { dt_tx = get_res_data();
        I2caRegs.I2CDXR = dt_tx;
        set_7seg_data(dt_tx); 
      } 
   }
}   // end of main

void I2CA_Init(void)
{
   // Initialize I2C
   // I2CCLK = SYSCLK/(I2CPSC+1)
   #if (CPU_FRQ_40MHZ||CPU_FRQ_50MHZ)
     I2caRegs.I2CPSC.all = 4;       // Prescaler - need 7-12 Mhz on module clk
   #endif

   #if (CPU_FRQ_60MHZ)
     I2caRegs.I2CPSC.all = 6;       // Prescaler - need 7-12 Mhz on module clk
   #endif
   I2caRegs.I2CCLKL = 10;           // NOTE: must be non zero
   I2caRegs.I2CCLKH = 5;            // NOTE: must be non zero

   I2caRegs.I2COAR = I2C_SLAVE_ADDR;
//   I2caRegs.I2CMDR.all = 0x0000;    // SLAVE, 8bits per data byte, TRX is don't care
   I2caRegs.I2CCNT = 1;	            // Get 1 byte
	I2caRegs.I2CIER.all = 0x18;		// Clear interrupts (was 0)
   I2caRegs.I2CSTR.bit.RRDY = 1;	   // Clear flag
	I2caRegs.I2CIER.bit.RRDY = 1;    // Enable Receive Interrupt

	I2caRegs.I2CMDR.all = 0x0020;	   // Take I2C out of reset

//   I2caRegs.I2CFFTX.all = 0x6000;   // Enable FIFO mode and TXFIFO
//   I2caRegs.I2CFFRX.all = 0x2040;   // Enable RXFIFO, clear RXFFINT,

   return;
}

interrupt void i2c_int1a_isr(void)     // I2C-A
{
   Uint16 IntSource;

   // Read interrupt source
   IntSource = I2caRegs.I2CISRC.all;

   // Interrupt source = stop condition detected
   if(IntSource == I2C_RX_ISRC)
   {  // Data received
      dt_rdy = 1;       
      dt_rx = I2caRegs.I2CDRR;
   }  // end of stop condition detected
   // Enable future I2C (PIE Group 8) interrupts
   PieCtrlRegs.PIEACK.all = PIEACK_GROUP8;
}

void InitGpioCtrls(void)
{
   EALLOW;
   GpioCtrlRegs.GPAMUX1.all = 0x0000;     // GPIO functionality GPIO0-GPIO15
   GpioCtrlRegs.GPAMUX2.all = 0x0000;     // GPIO functionality GPIO16-GPIO31
   GpioCtrlRegs.GPBMUX1.all = 0x0000;     // GPIO functionality GPIO32-GPIO34
   GpioCtrlRegs.AIOMUX1.all = 0x0000;     // Dig.IO funct. applies to AIO2,4,6,10,12,14

   GpioCtrlRegs.GPADIR.all = 0xFFFFFFF0;      // GPIO0-GPIO7 are GP inputs, GPIO8-GPIO31 are outputs
   GpioCtrlRegs.GPBDIR.all = 0xFFFFFFFF;      // GPIO32-GPIO35 are outputs
   GpioCtrlRegs.AIODIR.all = 0x00000000;      // AIO2,4,6,19,12,14 are digital inputs

   // Each input can have different qualification
   // a) input synchronized to SYSCLKOUT
   // b) input qualified by a sampling window
   // c) input sent asynchronously (valid for peripheral inputs only)
   GpioCtrlRegs.GPAQSEL1.all = 0x0000;    // GPIO0-GPIO15 Synch to SYSCLKOUT
   GpioCtrlRegs.GPAQSEL2.all = 0x0000;    // GPIO16-GPIO31 Synch to SYSCLKOUT
   GpioCtrlRegs.GPBQSEL1.all = 0x0000;    // GPIO32-GPIO34 Synch to SYSCLKOUT

   // Pull-ups can be enabled or disabled.
//   GpioCtrlRegs.GPAPUD.all = 0x0000;      // Pullup's enabled GPIO0-GPIO31
//   GpioCtrlRegs.GPBPUD.all = 0x0000;      // Pullup's enabled GPIO32-GPIO34
   GpioCtrlRegs.GPAPUD.all = 0xFFFFFFFF;      // Pullup's disabled GPIO0-GPIO31
   GpioCtrlRegs.GPBPUD.all = 0xFFFFFFFF;      // Pullup's disabled GPIO32-GPIO34
   EDIS;

}

// 12,16,17,18 - GPA; 32,33,34,35 - GPB
void set_7seg_data(char ch)
 { if (ch & 0x01)
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

   if (ch & 0x10)
      GpioDataRegs.GPBSET.bit.GPIO32 = 1;
   else
      GpioDataRegs.GPBCLEAR.bit.GPIO32 = 1;
   if (ch & 0x20)
      GpioDataRegs.GPBSET.bit.GPIO33 = 1;
   else
      GpioDataRegs.GPBCLEAR.bit.GPIO33 = 1;
  if (ch & 0x40)
      GpioDataRegs.GPBSET.bit.GPIO34 = 1;
   else
      GpioDataRegs.GPBCLEAR.bit.GPIO34 = 1;
  if (ch & 0x80)
      GpioDataRegs.GPBSET.bit.GPIO35 = 1;
   else
      GpioDataRegs.GPBCLEAR.bit.GPIO35 = 1;
 }

char get_res_data()
 { return GpioDataRegs.GPADAT.all & 0x000000FF;
 }

     

//===========================================================================
// No more.
//===========================================================================
