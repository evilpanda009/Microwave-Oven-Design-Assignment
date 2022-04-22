/********************************************************************************************************
- FB-EDU-PARM-LPC2138 RTC example
- Author: Kruck Wang(Wang RongHua)
- Date: 2009-04-23
- Guangzhou Windway Electronic Technology Co., Ltd.
********************************************************************************************************/

/****************************************************************************
* File Name: RTC_TIME.C
* Function: SPI, RTC and LED display
****************************************************************************/
#include  "../SRC/Config.h"
#include  "intrinsics.h"
#define   HC595_CS       (1<<29)           	/* P0.29 as 74HC595 CS */
#define   KEY1           (1<<20)

/****************************************************************************
* Function Name: MSpiIni()
* Function Desc: Init SPI interface, as master
* Input: none
* Output: none
****************************************************************************/
void  MSpiIni(void)
 { VPBDIV = 0x02;
   S0SPCCR = 0x52;		 // Set SPI freq div
   S0SPCR = 0x30;		    // Set SPI mode, MSTR=1£¬CPOL=1£¬CPHA=0£¬LSBF=0
 }

/****************************************************************************
* Function Name: MSendData()
* Function Desc: Send a byte over the Master
* Input: data ready to send
* Output: data read
****************************************************************************/
uint8  MSendData(uint8 data)
 { IO0CLR = HC595_CS;
   IO0SET = HC595_CS;
   IO0CLR = HC595_CS;			    // CS
   uint8 temp = S0SPDR;
   S0SPDR = data;
   while( 0==(S0SPSR&0x80) );		// Wait SPIF flag for send data finished

   IO0SET = HC595_CS;
   return(S0SPDR);
 }

/* 0--F */
uint8 const DISP_TAB[16] = { 0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90, 
0x88,0x83,0xC6,0xA1,0x86,0x8E};

/*
*******************************************************************************************************
* Function Name: RTCInit()
* Function Desc: Init RTC timer
* Input: none
* Output: none
*******************************************************************************************************
*/
void RTCInit (void)
 { PREINT = Fpclk / 32768 - 1;	// Setup base freq div
	PREFRAC = Fpclk - (Fpclk / 32768) * 32768;
	  
	CCR_bit.CTCRST = 1; // Reset CTC
   CCR_bit.CTCRST = 0; // Enable CTC

	YEAR  = 2009;
	MONTH = 5;
	DOM   = 18;
	DOW   = 1;
	HOUR  = 8;
	MIN   = 30;
	SEC   = 59;
	
	CIIR_bit.IMSEC = 1;	// Setup interrupt for every 1s
   CCR_bit.CLKSRC = 1; // Use external clock
   CCR_bit.CLKEN  = 1; // Start clock
 }

                          
uint8  rcv_data,time_flag=1; 	
/****************************************************************************
* Function Name: IRQ_Eint3()
* Function Desc: External Interrupt EINT3 Service routine, change display mode
* Input: none
* Output: none
****************************************************************************/
void   IRQ_Eint3(void)
 { Delayms(10);    // delay 10mS
   if((IO0PIN&KEY1)==0)
    { time_flag++;
      if(time_flag==5)
      time_flag=1;
    }
   while((EXTINT&0x08)!=0)
   EXTINT = 0x08;					// Clear EINT3 interrupt flag
   VICVectAddr = 0x00;
 }

/****************************************************************************
* Function Name: main()
* Function Desc: Base Calendar function
****************************************************************************/
int  main(void)
 { PINSEL0 = 0x00001500;			   // Setup SPI pinmap
   PINSEL1 = 0x00000301;            // Setup P0.20 as EINT0
   IO0DIR = HC595_CS;				   // Setup 595 CS pin
   MSpiIni();					         // SPI initialisation
   RTCInit ();			               // RTC initialisation
   EXTMODE  = 0<<3;				      // Setup EINT3 trigger mode as edge tirgger
   EXTPOLAR = 0<<0;                 // Setup EINT3 trigger mode as fall edge
   VICIntSelect = 0x00000000;		   // All interrupt as IRQ
   VICVectCntl0 = 0x20|17;          // 0x20=>IRQ enable, 1<<17 => EINT3 VIC channel 14
   VICVectAddr0 = (int)IRQ_Eint3;   // Setup ISR address
   EXTINT = 1<<3;			            // Clear EINT3 interrupt flag
   VICIntEnable = (1<<17);		      // Enbale EINT3 interrupt
   __enable_irq();                  // Enable IRQ interrupt
   while(1)	
    { switch(time_flag)
       { case 1:
            rcv_data = MSendData(DISP_TAB[YEAR % 10]);	   //  Display 'Year'
            rcv_data = MSendData(DISP_TAB[YEAR%100/ 10]);	
            rcv_data = MSendData(DISP_TAB[YEAR %1000/100]);
            rcv_data = MSendData(DISP_TAB[YEAR /1000]);	
            Delayms(200);
            break;
         
         case 2:
            rcv_data = MSendData(DISP_TAB[DOM % 10]);	      //  Display 'Day'
            rcv_data = MSendData(DISP_TAB[DOM/ 10]);	
            rcv_data = MSendData(DISP_TAB[MONTH %  10]);    //  and 'Month'
            rcv_data = MSendData(DISP_TAB[MONTH /10]);	
            Delayms(200);
            break;
         
         case 3:
            rcv_data = MSendData(DISP_TAB[MIN % 10]);       //  Display 'Minute'
            rcv_data = MSendData(DISP_TAB[MIN/ 10]);	
            rcv_data = MSendData(DISP_TAB[HOUR %  10]);     //  and 'Hour'
            rcv_data = MSendData(DISP_TAB[HOUR /10]);
            Delayms(200);
            break;
         
         case 4:     
            rcv_data = MSendData(DISP_TAB[DOW % 10]);	      //  Display 'Second'
            rcv_data = MSendData(DISP_TAB[DOW/ 10]);	
            rcv_data = MSendData(DISP_TAB[SEC %  10]);      //  and 'Weekday'
            rcv_data = MSendData(DISP_TAB[SEC /10]);	
            Delayms(200);
            break;
      
            default : break;
       }
    }
 }