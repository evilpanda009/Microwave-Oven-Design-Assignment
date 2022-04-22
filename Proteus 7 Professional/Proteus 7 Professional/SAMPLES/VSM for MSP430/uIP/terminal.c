/*****************************************************************************
*  Module Name:       Terminal I/O
*  
*  Created By:        Mikhail Usachev
*
*  Original Release:  October 29, 2009 
*
*  Module Description:  
*  Provides functions to interface with a Terminal using USCIA_0
*
*****************************************************************************/

#include "terminal.h"
#include  "msp430x24x.h"

#define VK_RETURN    13
#define RXTX_MASK    0x30

char* cur_ptr = 0;
char* ptr_max = 0;

void init_terminal()
 { UCA0CTL1 |= UCSSEL_1;                     // CLK = ACLK
   UCA0BR0 = 0xd0;                           // 2MHz/9600 = 0x341
   UCA0BR1 = 0x00;                           //
   UCA0MCTL = UCBRS1 + UCBRS0;               // Modulation UCBRSx = 3
 }

#define CR  0x0d

void putchar(char ch)
 { if (ch == '\n')
    { while (!(IFG2&UCA0TXIFG));                 // USCI_A0 TX buffer ready?
      UCA0TXBUF = CR;
    }
   while (!(IFG2&UCA0TXIFG));                 // USCI_A0 TX buffer ready?
   UCA0TXBUF = ch;
 }

void print_str(char* s)
 { P3SEL |= RXTX_MASK;                       // P3.4,5 = USCI_A0 TXD/RXD
   UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
   while(*s)
    { putchar(*s);
      s++;
    }
   while (!(IFG2&UCA0TXIFG));                 // USCI_A0 TX buffer ready?
   UCA0CTL1 |= UCSWRST;                      // **STOP UART**
   P3SEL &= ~RXTX_MASK; 
 }

void read_str(char* ptr, int max_sz)
{ unsigned char i;
  if (max_sz <= 0)
      return;
  cur_ptr = ptr;
  ptr_max = cur_ptr + max_sz; 
  *ptr = 0;

  P3SEL |= RXTX_MASK;                       // P3.4,5 = USCI_A0 TXD/RXD
  UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
  IE2 |= UCA0RXIE;                          // Enable USCI_A0 RX interrupt
  
  __bis_SR_register(LPM0_bits + GIE);       // Enter LPM0, interrupts enabled

  UCA0CTL1 |= UCSWRST;                      // **STOP UART**
  P3SEL &= ~RXTX_MASK; 
} 

//  Echo back RXed character, confirm TX buffer is ready first
#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void)
{ char ch = UCA0RXBUF; 
  if (ch == VK_RETURN)
      ch = 0; 
  *cur_ptr = ch; 
  if (cur_ptr == ptr_max || ch == 0) 
   { IE2 &= ~UCA0RXIE;                       // Prevent USCIA0 RX interrupts
     __bic_SR_register_on_exit(LPM0_bits);   // Clear CPUOFF bit from 0(SR)
   }
  else
     cur_ptr++;
}

