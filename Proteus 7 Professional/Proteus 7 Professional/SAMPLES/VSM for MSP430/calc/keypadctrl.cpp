/*********************************************************************
**********************************************************************
*****                                                             ****
*****         L A B C E N T E R    E L E C T R O N I C S          ****
*****                                                             ****
*****                  PIC 16F87X Keypad Scanner                  ****
*****                                                             ****
**********************************************************************
*********************************************************************/

// Rows are connected to Port C 
// Columns are connected to Port A with external pull-up resistors.

#include "calc.h"
#include <msp430x11x1.h>

CHAR keycodes[16] = {'7','8','9','/','4','5','6','*','1','2','3','-','.','0','=','+'};

volatile char fix_delay2;

VOID keypad_init()
{
   P2DIR |= BIT2;                       // setup direction
   P2DIR &= ~(BIT0 + BIT1);                       // setup direction
}

CHAR keypadread()
// Find a key, wait for
// it to be released and return.
 { CHAR key = scankeypad();
   if (key)
      while (scankeypad() != 0)
         /* Nothing */  ;  
   return key;
 } 

CHAR scankeypad()
// Scan the keypad for a keypress.
// Return 0 for no press or the char pressed.
 { INT8 row,col,tmp;
   CHAR key=0;

   // Initialise P1 for output
   P1DIR = 0xFF;                      // Direction: output
   INT8 maskrow = 1;
   for (row=0; row < KEYP_NUM_ROWS; row++, maskrow += maskrow)
    { // Drive appropriate row low and read columns:
      // 1. low part
      P2OUT &= ~BIT2;
      P1OUT = ~maskrow;
      fix_delay2 = 0;
      tmp = P2IN & 3;
      // 2.high part
      P2OUT |= BIT2;
      tmp |= (P2IN & 3) << 2;
    
      // See if any column is active (low):
      INT8 mask = 1;
      for (col=0; col<KEYP_NUM_COLS; ++col, mask += mask)
         if ((tmp & mask) == 0)
          { INT8 idx = (row*KEYP_NUM_COLS) + col;
            key = keycodes[idx]; 
            goto DONE;
          }
    }
   DONE:
   return key;
 }



