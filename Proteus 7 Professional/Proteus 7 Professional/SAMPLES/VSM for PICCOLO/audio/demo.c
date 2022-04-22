#include "DSP28x_Project.h"     // Device Headerfile and Examples Include File
#include <math.h>
#define   MATH_TYPE      IQ_MATH
#define   GLOBAL_Q       15
#include "IQmathLib.h"
#include "fft.h"

extern volatile unsigned long DbgFileReg;

extern void lcd_init();
extern void lcd_set_point(char x, char y);
extern void lcd_line(int x1, int y1, int x2, int y2);
//extern void lcd_clearcolumn(int c);
extern void lcd_paintbegin();
extern void lcd_paintend();
extern void print_str(const char* s, int x_left, int y_bottom);

void initialize_peripheral();
void paint(long maxv);
interrupt void interrupt_global_handler(void);

int sample;

#define TMR0_FREQ    60L
#define TMR0_PERIOD  15L    // 15us  ~ F_sampling = 66,7kHz

/* Create an Instance of FFT module                 */
#define     N        128 
#pragma DATA_SECTION(ipcb, "FFTipcb");
#pragma DATA_SECTION(mag, "FFTmag");
CFFT32  fft=CFFT32_128P_DEFAULTS;   
long ipcb[2*N];
long mag[N]; 
char sample_idx = 0; 
char samples_ready = 0;

extern const unsigned char input[];

/* Define window Co-efficient Array  and place the .constant section in ROM memory	*/
const long win[N/2]=HAMMING128;

void main(void)
 { long maxv;
   initialize_peripheral();
   
   lcd_init();

/* Initialize FFT module                            */
   fft.ipcbptr=ipcb;
   fft.magptr=mag;  
   fft.winptr=(long *)win;
   fft.init(&fft);  

   for(;;)
    { if (samples_ready)
       { DINT;
         CFFT32_brev2(ipcb,ipcb,N);

         fft.win(&fft); 
         fft.izero(&fft);      
         fft.calc(&fft);
         fft.mag(&fft); 

         maxv = fft.peakmag;
         paint(maxv);

         samples_ready = 0;
         EINT;
       }
    }
 }

/*
interrupt void  adc_isr(void)
 { long dw = input[sample_idx]; // syntetic signal
   ipcb[sample_idx] = _IQ(dw);
//   sample_idx++;   
//   ipcb[sample_idx] = 0;      // IM
   sample_idx++;
   if (sample_idx >= N*2)
    { sample_idx = 0;
      samples_ready = 1;
    }
   AdcRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;		//Clear ADCINT1 flag reinitialize for next SOC
   PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;   // Acknowledge interrupt to PIE
   return;
 }
*/

interrupt void  adc_isr(void)
 { long dw = AdcResult.ADCRESULT0;
   ipcb[sample_idx] = _IQ(dw);
   sample_idx++;   
   ipcb[sample_idx] = 0;      // IM
   sample_idx++;
   if (sample_idx >= N*2)
    { sample_idx = 0;
      samples_ready = 1;
    }
   AdcRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;		//Clear ADCINT1 flag reinitialize for next SOC
   PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;   // Acknowledge interrupt to PIE
   return;
 }


void initialize_peripheral()
 { volatile Uint16 iVol;
   int16	i;
   Uint32 *Dest;
// PLL, WatchDog, enable Peripheral Clocks
   EALLOW;
   SysCtrlRegs.WDCR= 0x0068;     // Disable watchdog
   // Make sure the PLL is not running in limp mode
   if (SysCtrlRegs.PLLSTS.bit.MCLKSTS != 1)
    { if (SysCtrlRegs.PLLCR.bit.DIV != 0x0A)
       { // Before setting PLLCR turn off missing clock detect
         SysCtrlRegs.PLLSTS.bit.MCLKOFF = 1;
         SysCtrlRegs.PLLCR.bit.DIV = 0x0A;
         while(SysCtrlRegs.PLLSTS.bit.PLLLOCKS != 1);
         SysCtrlRegs.PLLSTS.bit.MCLKOFF = 0;
       }
   }
   

// Clear all interrupts and initialize PIE vector table:
   DINT;
// Initialize PIE control registers to their default state.
// The default state is all PIE interrupts disabled and flags are cleared.
   InitPieCtrl();

   IER = 0x0000;
   IFR = 0x0000;

	Dest = (void *) &PieVectTable;
	for(i=0; i < 128; i++)
	 {	*Dest++ = (Uint32) &interrupt_global_handler;	
    }
	// Enable the PIE Vector Table
	PieCtrlRegs.PIECTRL.bit.ENPIE = 1;	

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
   SysCtrlRegs.PCLKCR0.bit.SCIAENCLK = 0;  	// SCI-A
   SysCtrlRegs.PCLKCR1.bit.ECAP1ENCLK = 0;	// eCAP1
   SysCtrlRegs.PCLKCR1.bit.EPWM1ENCLK = 0;   // ePWM1
   SysCtrlRegs.PCLKCR1.bit.EPWM2ENCLK = 0;   // ePWM2
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

// 5. GPIO
//--------------------------------------------------------------------------------------
//  GPIO-00 - PIN FUNCTION = data0
	GpioCtrlRegs.GPAMUX1.bit.GPIO0= 0;		// 0=GPIO,  1=EPWM2A,  2=Resv,  3=Resv
	GpioCtrlRegs.GPADIR.bit.GPIO0= 1;		// 1=OUTput,  0=INput 
//--------------------------------------------------------------------------------------
//  GPIO-01 - PIN FUNCTION = data1
	GpioCtrlRegs.GPAMUX1.bit.GPIO1= 0;		// 0=GPIO,  1=EPWM2A,  2=Resv,  3=Resv
	GpioCtrlRegs.GPADIR.bit.GPIO1 = 1;		// 1=OUTput,  0=INput 
//--------------------------------------------------------------------------------------
//  GPIO-02 - PIN FUNCTION = data2
	GpioCtrlRegs.GPAMUX1.bit.GPIO2= 0;		// 0=GPIO,  1=EPWM2A,  2=Resv,  3=Resv
	GpioCtrlRegs.GPADIR.bit.GPIO2 = 1;		// 1=OUTput,  0=INput 
//--------------------------------------------------------------------------------------
//  GPIO-03 - PIN FUNCTION = data3
	GpioCtrlRegs.GPAMUX1.bit.GPIO3= 0;		// 0=GPIO,  1=EPWM2A,  2=Resv,  3=Resv
	GpioCtrlRegs.GPADIR.bit.GPIO3 = 1;		// 1=OUTput,  0=INput 
//--------------------------------------------------------------------------------------
//  GPIO-04 - PIN FUNCTION = data4
	GpioCtrlRegs.GPAMUX1.bit.GPIO4= 0;		// 0=GPIO,  1=EPWM2A,  2=Resv,  3=Resv
	GpioCtrlRegs.GPADIR.bit.GPIO4 = 1;		// 1=OUTput,  0=INput 
//--------------------------------------------------------------------------------------
//  GPIO-05 - PIN FUNCTION = data5
	GpioCtrlRegs.GPAMUX1.bit.GPIO5= 0;		// 0=GPIO,  1=EPWM2A,  2=Resv,  3=Resv
	GpioCtrlRegs.GPADIR.bit.GPIO5 = 1;		// 1=OUTput,  0=INput 
//--------------------------------------------------------------------------------------
//  GPIO-06 - PIN FUNCTION = data6
	GpioCtrlRegs.GPAMUX1.bit.GPIO6= 0;		// 0=GPIO,  1=EPWM2A,  2=Resv,  3=Resv
	GpioCtrlRegs.GPADIR.bit.GPIO6 = 1;		// 1=OUTput,  0=INput 
//--------------------------------------------------------------------------------------
//  GPIO-07 - PIN FUNCTION = data7
	GpioCtrlRegs.GPAMUX1.bit.GPIO7= 0;		// 0=GPIO,  1=EPWM2A,  2=Resv,  3=Resv
	GpioCtrlRegs.GPADIR.bit.GPIO7 = 1;		// 1=OUTput,  0=INput 
//--------------------------------------------------------------------------------------
//  GPIO-12 - PIN FUNCTION = E
	GpioCtrlRegs.GPAMUX1.bit.GPIO12= 0;	   // 0=GPIO,  1=SCITXD-A,  2=I2C-SCL,  3=TZ3
	GpioCtrlRegs.GPADIR.bit.GPIO12 = 1;		// 1=OUTput,  0=INput 
   GpioDataRegs.GPASET.bit.GPIO12 = 1;	   // E = 1
//--------------------------------------------------------------------------------------
//  GPIO-34 - PIN FUNCTION = R/$W$
	GpioCtrlRegs.GPBMUX1.bit.GPIO34= 0;	   // 0=GPIO,  1=COMP2OUT,  2=EMU1,  3=Resv
	GpioCtrlRegs.GPBDIR.bit.GPIO34 = 1;		// 1=OUTput,  0=INput 
	GpioDataRegs.GPBSET.bit.GPIO34 = 1;		// uncomment if --> Set High initially
//--------------------------------------------------------------------------------------
//  GPIO-35 - PIN FUNCTION = A0
	GpioCtrlRegs.GPBMUX1.bit.GPIO35= 0;	   // 0=GPIO,  1=COMP2OUT,  2=EMU1,  3=Resv
	GpioCtrlRegs.GPBDIR.bit.GPIO35 = 1;		// 1=OUTput,  0=INput 
	GpioDataRegs.GPBSET.bit.GPIO35 = 1;		// uncomment if --> Set High initially
	EDIS;

   PieCtrlRegs.PIEIER1.bit.INTx1 = 1;	// Enable INT 1.1 in the PIE
   IER |= M_INT1;
   EINT;   // Enable Global interrupt INTM
 }

#define X0  0
#define Y0  52
#define W   128
#define H   50
#define H_IQ30   (long)_IQ30(H)
#define FNT_WIDTH    4 
#define FNT_HEIGHT   6 
#define Ytext  Y0+FNT_HEIGHT+5
#define W2CH   FNT_WIDTH+FNT_WIDTH+1
#define W2CH_HALF   (W2CH>>1)
#define W3CH   W2CH+FNT_WIDTH+1
#define W3CH_HALF   (W3CH>>1)

/*
void paint(int maxv)
// draw X,Y-axis and mag[] graph
 { int i, x;
   long y;
   for(i = 0; i < Nhalf - 5; i++)
    { x = (X0 + i + 1)*2;
      lcd_clearcolumn(x);
      y = maxv ? mag[i]*H/maxv : 0;
      lcd_line(x, Y0, x, Y0 - y);
    }
   lcd_line(X0, Y0, X0 + W, Y0); 
   lcd_line(X0, Y0, X0, Y0 - H);
 }
*/

void paint(long maxv)
// draw X,Y-axis and mag[] graph
 { int i, x;   
   long y;
   float maxvalue, curvalue, du;
   lcd_paintbegin();

   maxvalue = sqrt(maxv);
   for(x = X0, i = 0; x < W-X0; i++, x+=2)
    { curvalue = sqrt(mag[i]);
      if (maxvalue != 0.0)
       { curvalue = H*curvalue/maxvalue;     ///10*log(curvalue/maxvalue);               // 0..100 dB scale
         y = (int) curvalue;
         if (y < 0 ) y = -y;
       }
      else
         y = 0;
         
//      DbgFileReg = y;

      lcd_line(x, Y0, x, Y0 - y);
    }
   lcd_line(X0, Y0, X0 + W, Y0); 
   lcd_line(X0, Y0, X0, Y0 - H);


   // 1pt corresponding 217 Hz
   
   print_str("0", 0, Ytext);
   lcd_line(0, Y0+1, 0, Y0+3); 
   print_str("5k", 23-W2CH_HALF, Ytext);
   lcd_line(23, Y0+1, 23, Y0+3); 
   print_str("10k", 46-W3CH_HALF, Ytext);
   lcd_line(46, Y0+1, 46, Y0+3); 
   print_str("15k", 69-W3CH_HALF, Ytext);
   lcd_line(69, Y0+1, 69, Y0+3); 
   print_str("20k", 92-W3CH_HALF, Ytext);
   lcd_line(92, Y0+1, 92, Y0+3); 
   print_str("25k", 115-W3CH_HALF, Ytext);
   lcd_line(115, Y0+1, 115, Y0+3); 

   lcd_paintend();
 }

interrupt void interrupt_global_handler(void)
{
  // Insert ISR Code here
  
  // Next two lines for debug only to halt the processor here
  // Remove after inserting ISR Code
  asm ("      ESTOP0");
  for(;;);
}
