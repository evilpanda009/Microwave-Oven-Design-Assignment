#include "inc\hw_types.h"
#include "inc\hw_adc.h"
#include "inc\hw_gpio.h"
#include "inc\hw_memmap.h"
#include "inc\hw_sysctl.h"
#include "inc\hw_ints.h"
#include "driverlib\adc.h"
#include "driverlib\gpio.h"
#include "driverlib\sysctl.h"
#include "driverlib\timer.h"
#include "driverlib\interrupt.h"

#define WNDSZ        9
#define EDGE         2
#define MEDIANPOS    4
unsigned long bf[WNDSZ];
unsigned char g_ucOversampleCnt;
unsigned char idx_write;

// Truncated mean filter
unsigned long truncated_mean()
 { char i, j;
   unsigned long r;
   unsigned long tmp[WNDSZ];
   for (i=0; i<WNDSZ; i++) tmp[i] = bf[i];
   for (i=0; i<WNDSZ-1; i++)
    { for (j=i+1; j<WNDSZ; j++)
       { if (tmp[i]<tmp[j])
          { r=tmp[i];
            tmp[i]=tmp[j];
            tmp[j]=r;
          }
       }
    }
   r = 0;
   for (i=EDGE; i<WNDSZ-EDGE; i++) r += tmp[i];
   return r/(WNDSZ-2*EDGE);
 }

void dac_write(unsigned long value)
 { GPIOPinWrite(GPIO_PORTA_BASE, 0xFF, value & 0xFF);
   GPIOPinWrite(GPIO_PORTB_BASE, 0x0F, (value>>6) & 0x0F);
   GPIOPinWrite(GPIO_PORTC_BASE, 0x01, 1);
   GPIOPinWrite(GPIO_PORTC_BASE, 0x01, 0);
 }

void dbgout(long v)
 { long x;
   char* ptr = (char*) 0xFFFFFF00;
   x = v/100;
   *ptr = x + '0';
   v -= x*100;
   x = v/10;
   *ptr = x + '0';
   v -= x*10;
   *ptr = v + '0';
   *ptr = 0; 
 }
 
void ADC_Handler(void)
 { unsigned long v;
   //
   // Clear the interrupt
   //
   ADCIntClear(ADC_BASE, 3);
    
   v = HWREG(ADC_BASE + ADC_O_SSFIFO3);

   bf[idx_write] = v;
   idx_write = (idx_write + 1) % WNDSZ;

   v = truncated_mean();
   dac_write(v);
   //dbgout(v);

   //
   // Placeholder for ADC processing code
   // 
 }

int main()
 { //
   // Initialize the GPIO
   //
   HWREG(SYSCTL_RCGC2) |= SYSCTL_RCGC2_GPIOA;   // enable GPIOA clocking for changing pin functions
   HWREG(SYSCTL_RCGC2) |= SYSCTL_RCGC2_GPIOB;   // enable GPIOB clocking for changing pin functions
   HWREG(SYSCTL_RCGC2) |= SYSCTL_RCGC2_GPIOC;   // enable GPIOC clocking for changing pin functions
   GPIODirModeSet(GPIO_PORTA_BASE, 0xFF, GPIO_DIR_MODE_OUT);
   GPIODirModeSet(GPIO_PORTB_BASE, 0xFF, GPIO_DIR_MODE_OUT);
   GPIODirModeSet(GPIO_PORTC_BASE, 0x01, GPIO_DIR_MODE_OUT);
   dac_write(0);   

   //
   // Initialize the ADC to take a single sample on channel 1, sequencer 3
   // when a trigger is detected.
   //
   HWREG(SYSCTL_RCGC0) |= SYSCTL_RCGC0_ADC0;          // enable ADC0
   HWREG(SYSCTL_RCGC0) |= SYSCTL_RCGC0_ADCSPD250K;    // 250K samples/second
   ADCSequenceConfigure(ADC_BASE, 3, ADC_TRIGGER_TIMER, 0);
   ADCSequenceStepConfigure(ADC_BASE, 3, 0, (ADC_CTL_CH1 | ADC_CTL_IE | ADC_CTL_END));
   //
   // Initialize Timer 0 to trigger an ADC conversion once every 100 microseconds
   //
   HWREG(SYSCTL_RCGC1) |= SYSCTL_RCGC1_TIMER0;        // enable Timer0
   TimerConfigure(TIMER0_BASE, TIMER_CFG_32_BIT_PER);
   TimerLoadSet(TIMER0_BASE, TIMER_A, SysCtlClockGet() / 10000);
   TimerControlTrigger(TIMER0_BASE, TIMER_A, true);

   //
   // Enable the sequencer and interrupt
   //
   ADCSequenceEnable(ADC_BASE, 3);
   ADCIntEnable(ADC_BASE, 3);
   IntEnable(INT_ADC3);
   //
   // Zero the oversample counter and buffer
   //
   for (idx_write=WNDSZ-1;idx_write;--idx_write)
    { bf[idx_write] = 0;
    }
   //
   // Enable the timer and start conversion process
   //
   TimerEnable(TIMER0_BASE, TIMER_A);   

   while(1)
    { 
    }
   return 0;
 }