/********************************************************************************************************
- FB-EDU-PARM-LPC2138 User Function file
- Author: Luo XieBing 
- Date: 2009-05-18
- Guangzhou Windway Electronic Technology Co., Ltd.
********************************************************************************************************/

/****************************************************************************
* File name: PWM_Music.C
* Function Desc: Use PWM to play music "CAI HONG MEI MEI"
****************************************************************************/
#include "../SRC/Config.h"
#include "Target.h"

#define TEMPO   8
#define _1      TEMPO*4   
#define _1d     TEMPO*6     
#define _2      TEMPO*2     
#define _2d     TEMPO*3     
#define _4      TEMPO*1
#define _4d     TEMPO*3/2
#define _8      TEMPO*1/2
#define _8d     TEMPO*3/4
#define _16     TEMPO*1/4
#define _16d    TEMPO*3/8
#define _32     TEMPO*1/8

#define _1DO    1000000/(262*2)
#define _1RE    1000000/(294*2)
#define _1MI    1000000/(330*2)
#define _1FA    1000000/(349*2)
#define _1SO    1000000/(392*2)
#define _1LA    1000000/(440*2)
#define _1SI    1000000/(494*2)

#define _DO    1000000/(523*2)
#define _RE    1000000/(587*2)
#define _MI    1000000/(659*2)
#define _FA    1000000/(698*2)
#define _SO    1000000/(784*2)
#define _LA    1000000/(880*2)
#define _SI    1000000/(988*2)

#define _DO1    1000000/(1047*2)
#define _RE1    1000000/(1175*2)
#define _MI1    1000000/(1319*2)
#define _FA1    1000000/(1397*2)
#define _SO1    1000000/(1568*2)
#define _LA1    1000000/(1760*2)
#define _SI1    1000000/(1976*2)

/* CAI HONG MEI MEI opern */
const uint32 HCMM[] =
 { _LA, _SO, _MI, _LA, _SO, _MI,
	_LA, _LA, _SO, _LA,
	_LA, _SO, _MI, _LA, _SO, _MI,
	_RE, _RE, _DO, _RE,
	_MI, _MI, _SO, _LA, _DO1, _LA, _SO,
	_MI, _MI, _SO, _DO,
	_MI, _MI, _MI, _MI, _MI,
	_1LA,_1LA,_1SO,_1LA,
 };

const uint32 HCMM_L[] =
 { _4, _8, _8, _4, _8, _8,
	_8, _4, _8, _2,
	_4, _8, _8, _4, _8, _8,
	_8, _4, _8, _2,
	_4, _8, _8, _8, _8, _8, _8,
	_8, _4, _8, _2,
	_4, _4, _4, _8, _8,
	_8, _4, _8, _2,
 };


/*
********************************************************************************************************
** Function Name: Delay
** Function Desc: Delay
*******************************************************************************************************
*/
void Delay(uint8 dly)
 { uint32 i;
   for(; dly > 0; dly--)
      for(i = 0; i < 0x7FFF; i++);
 }


/****************************************************************************
* Function Name: main()
****************************************************************************/
int  main(void)
 { uint8 i;
    
   VICIntEnClear = 0xffffffff;
   VICVectAddr = 0;
   VICIntSelect = 0;

   PINSEL0  = 0x02 << 14;		         // P0.7 ==> PWM2

   /* PWM Init */
   PWMPR    = 0;		                  // Freq = Fpclk
   PWMMCR   = 0x02;
   PWMMR0   = Fpclk / 2000;
   PWMMR2   = PWMMR0;	    	         // 50%
   PWMPCR   = 0x0400;			         // Enable PWM2 Output, single edge PWM
   PWMLER   = 0x05;			
   PWMTCR   = 0x02;                    // Restart PWMTC
   PWMTCR   = 0x01;                    // Start PWM output

   while(1)
    { for(i = 0; i < sizeof(HCMM); i++)
       { PWMMR0 = Fpclk / HCMM[i];     // Set output frequency
         PWMLER = 0x05;		            // Latch
         Delay(HCMM_L[i]);			      // Control play speed
       }
    }
 }

