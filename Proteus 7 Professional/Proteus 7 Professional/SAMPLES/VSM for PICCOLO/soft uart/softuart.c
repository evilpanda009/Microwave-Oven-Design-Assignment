#include "DSP28x_Project.h"     // Device Headerfile and Examples Include File

#define STOPMASK   0x0800
#define DTMASKBITS (1 << STOPMASK) - 1
#define NBITS  8

struct CONTEXT
 { unsigned short dt;
   unsigned short mask;
   char active;
   char dtrdy;
   char has_parity;        // 0 - no parity, 1 - odd, 2 - even
 } rxcontext, txcontext;

interrupt void cpu_timer1_isr(void);

void init()
 { // Pinout
   int i;
   EALLOW;
   GpioCtrlRegs.GPBPUD.bit.GPIO32 = 0;    // Enable pull-up for Rx
   GpioCtrlRegs.GPBPUD.bit.GPIO34 = 1;    // Disable pull-up for Tx
   GpioCtrlRegs.GPBQSEL1.bit.GPIO32 = 3;  // Asynch input GPIO28 (SCIRXDA)
   GpioCtrlRegs.GPBQSEL1.bit.GPIO34 = 3;  // Asynch input GPIO28 (SCIRXDA)
   GpioCtrlRegs.GPBMUX1.bit.GPIO32 = 0;   // GPIO
   GpioCtrlRegs.GPBMUX1.bit.GPIO34 = 0;   // GPIO
   GpioCtrlRegs.GPBDIR.bit.GPIO32 = 0;		// 1=OUTput,  0=INput 
   GpioCtrlRegs.GPBDIR.bit.GPIO34 = 1;		// 1=OUTput,  0=INput 
   GpioDataRegs.GPBSET.bit.GPIO34 = 1;    // Initially set Tx = 1
   EDIS;
   for(i = 0; i < 1000; i++);             // some delay
   // Timers
   InitCpuTimers();
   ConfigCpuTimer(&CpuTimer1, 60, 104);  // 60MHz CPU Freq, 9600 baud -> period = 104 uS (in uSeconds)
   // To ensure precise timing, use write-only instructions to write to the entire register. Therefore, if any
   // of the configuration bits are changed in ConfigCpuTimer and InitCpuTimers (in DSP2802x_CpuTimers.h), the
   // below settings must also be updated.
   CpuTimer1Regs.TCR.all = 0x4001; // Use write-only instruction to set TSS bit = 0
 }

char send_char(char ch)
 { unsigned short nbits;
   unsigned char i, numones, parity;
   if (txcontext.active)
     return 0;
   nbits = NBITS + (txcontext.has_parity ? 1 : 0);
   txcontext.dt = ~((1 << nbits)-1);                  // STOP bits
   if (txcontext.has_parity)
    { numones = 0;
      for (i = 0; i < NBITS; i++)
       { if (ch & (1 << i)) numones++;            
       }
      parity = (((txcontext.has_parity & 1) + (numones & 1)) & 1) << NBITS;
      txcontext.dt |= parity;                         // PARITY bit
    }
   txcontext.dt |= ch;                                // DATA bits
   txcontext.dt <<= 1;                                // START bit
   txcontext.mask = 1;
   txcontext.active = 1;
   return 1;
 }

char is_read_char()
 { return rxcontext.dtrdy;
 }

char read_char()
 { rxcontext.dtrdy = 0;
   return rxcontext.dt;
 }

void print_str(const char* str)
 { while(*str)
    { while(txcontext.active); 
      send_char(*str);
      str++;      
    }
 }

void main(void)
 { char ch;
   txcontext.dt = 0;
   txcontext.active = 0; 
   txcontext.mask = 1; 
   txcontext.has_parity = 0;
   rxcontext.dt = 0;
   rxcontext.active = 0; 
   rxcontext.mask = 1; 
   rxcontext.dtrdy = 0;
   rxcontext.has_parity = 0;
// Step 1. Initialize System Control:
// PLL, WatchDog, enable Peripheral Clocks
// This example function is found in the DSP2802x_SysCtrl.c file.
   InitSysCtrl();

   init();
   	
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
   PieVectTable.TINT1 = &cpu_timer1_isr;
   EDIS;    // This is needed to disable write to EALLOW protected registers
   IER |= M_INT13;     // enable CPU-Timer1 interrupt
   EINT;   // Enable Global interrupt INTM

// Step 5. User specific code:
   print_str("Hello, world!\n");

   for (;;)
    { if (is_read_char())
       { ch = read_char();
         send_char(ch + 1);  
       }
    }
  }

interrupt void cpu_timer1_isr(void)
 { char v;
   if (txcontext.active)
    { v = (txcontext.dt & txcontext.mask) ? 1 : 0;
      txcontext.mask <<= 1; 
      if (v)
       { GpioDataRegs.GPBSET.bit.GPIO34 = 1;
       }
      else
       { GpioDataRegs.GPBCLEAR.bit.GPIO34 = 1;
       } 
      if (txcontext.mask == STOPMASK)
       { txcontext.active = 0;
       }
    }
   v = GpioDataRegs.GPBDAT.bit.GPIO32;
   if (rxcontext.active)
    { if (v) 
        rxcontext.dt |= rxcontext.mask;
      rxcontext.mask <<= 1;
      if (rxcontext.mask == STOPMASK)
       { rxcontext.dt &= 0xff;
         rxcontext.active = 0;
         rxcontext.dtrdy = 1;
       }
    }
   else
    { if (v == 0)
       { rxcontext.dt = 0;
         rxcontext.mask = 1;  
         rxcontext.active = 1;           
       }
    }
   // The CPU acknowledges the interrupt.
   EDIS;
}
