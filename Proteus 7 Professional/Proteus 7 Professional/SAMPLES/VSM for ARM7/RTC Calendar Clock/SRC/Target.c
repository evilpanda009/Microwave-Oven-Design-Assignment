/********************************************************************************************************
- FB-EDU-PARM-LPC2138 Target Initial file
-
- Author: Kruck Wang(Wang RongHua)
-
- Date: 2009-04-23
-
- Guangzhou Windway Electronic Technology Co., Ltd.
********************************************************************************************************/

#include "Target.h"
#include "Config.h"

/*********************************************************************************************************
** Function Name: IRQ_Exception
** Function Desc: Interrupt exception handler
**
** Input: none
**
** Output: none
**         __ICFEDIT_region_ROM_start__
** Global var: none
** Called module: none
**
********************************************************************************************************/
 __irq void IRQ_Handler (void)
{
  void (*interrupt_function)();
  unsigned int vector;

  vector = VICVectAddr;     // Get interrupt vector.
  interrupt_function = (void(*)())vector;
  (*interrupt_function)();  // Call vectored interrupt function.
}
/*********************************************************************************************************
** Function Name: FIQ_Exception
** Function Desc: Fast Interrupt exception handler
**
** Input: none
**
** Output: none
**
** Global var: none
** Called module: none
********************************************************************************************************/
void FIQ_Exception(void)
{
    while(1);                   // You should replace the code here if you are using FIQ
}

/*********************************************************************************************************
** Function Name: TargetInit
** Function Desc: Target board initialisation routine
**
** Input: none
**
** Output: none
**
** Global var: none
** Called module: none
********************************************************************************************************/
void TargetInit(void)
{
    /* Add your code here */
}

/*********************************************************************************************************
** Function Name: TargetResetInit
** Function Desc: Initialisation after reset
**
** Input: none
**
** Output: none
**
** Global var: none
** Called module: none
********************************************************************************************************/
void TargetResetInit(void)
{
#ifndef NDEBUG
    MEMMAP = 0x2;                   //remap
#else
    MEMMAP = 0x1;                   //remap
#endif

    /* Set up System clock */
    PLLCON = 1;
#if (Fpclk / (Fcclk / 4)) == 1
    VPBDIV = 0;
#endif
#if (Fpclk / (Fcclk / 4)) == 2
    VPBDIV = 2;
#endif
#if (Fpclk / (Fcclk / 4)) == 4
    VPBDIV = 1;
#endif

#if (Fcco / Fcclk) == 2
    PLLCFG = ((Fcclk / Fosc) - 1) | (0 << 5);
#endif
#if (Fcco / Fcclk) == 4
    PLLCFG = ((Fcclk / Fosc) - 1) | (1 << 5);
#endif
#if (Fcco / Fcclk) == 8
    PLLCFG = ((Fcclk / Fosc) - 1) | (2 << 5);
#endif
#if (Fcco / Fcclk) == 16
    PLLCFG = ((Fcclk / Fosc) - 1) | (3 << 5);
#endif
    PLLFEED = 0xaa;
    PLLFEED = 0x55;
    while((PLLSTAT & (1 << 10)) == 0);
    PLLCON = 3;
    PLLFEED = 0xaa;
    PLLFEED = 0x55;

    /* Set up memory accelerate module */
    MAMCR = 0;
#if Fcclk < 20000000
    MAMTIM = 1;
#else
#if Fcclk < 40000000
    MAMTIM = 2;
#else
    MAMTIM = 3;
#endif
#endif
    MAMCR = 2;

    /* Init VIC */
    VICIntEnClear = 0xffffffff;
    VICVectAddr = 0;
    VICIntSelect = 0;

    /* Add your additional init code here */
}


/****************************************************************************
* Function Name: Delayms()
* Function Desc: software delay
* Input: dly		delay
* Output: none
****************************************************************************/
void  Delayms(uint32  dly)
{  uint32  i;

   for(; dly>0; dly--)
      for(i=0; i<2000; i++);
}
