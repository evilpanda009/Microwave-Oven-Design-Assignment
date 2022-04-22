/*********************************************************************
**********************************************************************
*****                                                             ****
*****         L A B C E N T E R    E L E C T R O N I C S          ****
*****                                                             ****
*****                    PIC 16F87X LCD Driver                     ****
*****                                                             ****
**********************************************************************
*********************************************************************/

#include "calc.h"
#include <msp430x11x1.h>

volatile char fix_delay;

VOID lcd_init ()
// Initialise the LCD Display. 
{  P2DIR |= BIT3 + BIT4 + BIT5;  // Direction: output
   wrcmd(0x30);                  // 8-bit mode - 1 line.
   wrcmd(LCD_SETVISIBLE+0x04);   // Display only - no cursors.
   wrcmd(LCD_SETMODE+0x03);      // Automatic Increment - Display shift left.
   wrcmd(LCD_SETDDADDR+0x0F);    // Initial Position far right.
 }

VOID clearscreen ()
// Clear the LCD Screen and reset
// initial position.
 { wrcmd(LCD_CLS);
   wrcmd(LCD_SETDDADDR+0x0F);
 }

/***** Utility Functions *****/

VOID wrcmd (CHAR cmdcode)
// Write a command to the LCD display.
 { P1DIR = 0xFF;                 // Direction: output
   P1OUT = cmdcode;

   // Write to PORTB to latch data into the display.
   // Toggle Pin 'E' to send the command.
   P2OUT  = LCD_CMD_WR << 3;
   P2OUT |= BIT5;                     // E = 1
   fix_delay = 0;
   P2OUT &= ~BIT5;                    // E = 0
   
   lcd_wait();
 }
     
VOID wrdata (CHAR data)
// Write a Character to the LCD Display. 
{  P1DIR = 0xFF;                     // Direction: output
   P1OUT = data;
   
   P2OUT = LCD_DATA_WR << 3;
   P2OUT |= BIT5;                     // E = 1
   fix_delay = 1;
   P2OUT &= ~BIT5;                    // E = 0
   
   lcd_wait();
 }

VOID lcd_wait ()
// Wait for the LCD busy flag to clear. 
 { BYTE status;
   P1DIR = 0;                        // Direction: input
   
   P2OUT = LCD_BUSY_RD << 3;
   do
    { P2OUT |= BIT5;                 // E = 1
      fix_delay = 2;
      status = P1IN;
      P2OUT &= ~BIT5;                // E = 0
    } while (status & 0x80);
 }


