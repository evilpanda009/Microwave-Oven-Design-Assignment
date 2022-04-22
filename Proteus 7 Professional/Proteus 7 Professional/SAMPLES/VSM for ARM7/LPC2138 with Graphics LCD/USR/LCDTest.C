/********************************************************************************************************
- FB-EDU-PARM-LPC2138 User Function file
- Author: Kruck Wang(Luo XieBing )
- Date: 2009-04-23
- Guangzhou Windway Electronic Technology Co., Ltd.
- Date: 2009-04-23
- Guangzhou Windway Electronic Technology Co., Ltd.
- Date: 2010-10-29
- Labcenter Electronics Ltd.
********************************************************************************************************/

/****************************************************************************
* FileName：LCDTEST.C
* Function：Display a picture
****************************************************************************/
#include "../SRC/Config.h"
#include "../SRC/Target.h"
#include "../SRC/T6963C.h"

/****************************************************************************
* Function Name: DelayNS()
* Function Description: Software delay
* Input parameter: dly		delay
* Output parameter: none
****************************************************************************/
void  DelayNS(uint32  dly)
 { uint32  i;
   for(; dly>0; dly--) 
      for(i=0; i<10000; i++);
 }

void init_lcd (void)
 { IO0CLR=rst;
   IO0SET=rst;
   delay1(50);
   IO0CLR=ce;
   IO0SET=wr;
   IO0SET=rd;
   wr_xd(addr_w,0x40);                   // Set text display buffer base address
   wr_xd(addr_t,0x42);                   // Set graphics display buffer base address
   wr_td(width,0x00,0x41);               // Set text display width
   wr_td(width,0x00,0x43);               // Set graphics display width
   wr_comm(0x81);                        // Set display mode: text xor graphics
   wr_td(0x56,0x00,0x22);                // Set CGRAM offset address
   wr_comm(0x9c);                        // Set text and graphic display on
 }

// Main Program
void main ()
 { CCR_bit.CLKSRC = 1; // Use external clock

   PINSEL0 = 0x00000000;		            // All GPIO
   PINSEL1 = 0x00000000;                 // All GPIO
   IO0DIR =  0xffffffff; 
   init_lcd ();                          // Initialize LCD
   while(1)
    { clrram();                         // Clear the screen
      disp_img(0,16,64,nBitmapDot);     // Display a bitmap
      //disp_hz(4,4,"广州风标");          // Display Chinese character
      disp_zf(0,10, "Text:"); // Display a string
      disp_zf(1,12, "LABCENTER"); // Display a string
      disp_zf(1,14, "Proteus VSM");
      DelayNS(150);
      clrram();
      disp_dz(0xcc,0x33);               // Display a matrix
      DelayNS(100);
    }  
 } 
