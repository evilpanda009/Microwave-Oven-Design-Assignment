/*
 * Copyright (c) 2001-2003, Adam Dunkels.
 * All rights reserved. 
 *
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following conditions 
 * are met: 
 * 1. Redistributions of source code must retain the above copyright 
 *    notice, this list of conditions and the following disclaimer. 
 * 2. Redistributions in binary form must reproduce the above copyright 
 *    notice, this list of conditions and the following disclaimer in the 
 *    documentation and/or other materials provided with the distribution. 
 * 3. The name of the author may not be used to endorse or promote
 *    products derived from this software without specific prior
 *    written permission.  
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.  
 *
 * This file is part of the uIP TCP/IP stack.
 *
 * $Id: main.c,v 1.10.2.4 2003/10/21 21:27:51 adam Exp $
 *
 */


#include <stdlib.h>   /* For system(). */
#include <stdio.h>    /* For printf(). */

#undef HTONS

#include "uip.h"
#include "terminal.h"
#include "uip_arp.h"
//#include "tapdev.h"
#include "httpd.h"
#include "telnetd.h"
#include "app.h"
#include  "msp430x24x.h"

static const struct uip_eth_addr ethaddr = {{0x00,0x00,0xe2,0x58,0xb6,0x6b}};

#define BUF ((struct uip_eth_hdr *)&uip_buf[0])

#ifndef NULL
#define NULL (void *)0
#endif /* NULL */

static unsigned short start, current;

#define RT_CLOCK_SECOND 3

/*-----------------------------------------------------------------------------------*/
/**
 * \internal
 * A real-time clock.
 *
 * This example main() function uses polling of a real-time clock in
 * order to know when the periodic processing should be
 * performed. This is implemented using this function - rt_ticks(). In
 * this example unix implementation, it simply calls the unix function
 * gettimeofday() which returns the current wall clock time.
 *
 * For a micro-controller, a simple way to implement this function is
 * by having a counter that is incremented by a timer interrupt and
 * read by this function.
 * 
 * The macro RT_CLOCK_SECOND should be defined as the approximate
 * number of ticks that are elapsed during one second. 
 */
static unsigned short
rt_ticks(void)
{
  extern unsigned short ticks;
  return ticks;
}
/*-----------------------------------------------------------------------------------*/

void init_mcu(void)
{ 
  // XT1 as high-frequency
  BCSCTL1 |= XTS;
  // turn on XT1 oscillator
  _BIC_SR(OSCOFF);
  // Wait for crystal to stabilize
  do
    IFG1 &= ~OFIFG;
  while (IFG1 & OFIFG);
  // ACLK = XT1 / 2
  BCSCTL1 |= DIVA0;
  BCSCTL1 &= ~DIVA1;
  // Disable WDT interrupt and  clear WDT interrupt flag
  IE1 &= ~WDTIE;
  IFG1 &= ~WDTIFG;
  // Use WDT as timer, flag each 512 pulses from ACLK
  WDTCTL = WDTPW | WDTTMSEL | WDTCNTCL | WDTSSEL | WDTIS1;
  // Count 1024 pulses from XT1 (until XT1's amplitude is OK)
  while (!(IFG1 & WDTIFG));                      
  // Clear oscillator fault interrupt flag
  IFG1 &= ~OFIFG;
  // Set XT1 as MCLK
  BCSCTL2 = SELM0 | SELM1;

  // ACLK = XT1 / 4 = 2 MHz
  BCSCTL1 &= ~DIVA0; 
  BCSCTL1 |= DIVA1;
  // Stop timer, use ACLK / 8 = 250 kHz, generate interrupts
  TACTL = ID1 | ID0 | TASSEL0 | TAIE;
  // Start timer in continuous up-mode
  TACTL |= MC1;
  // Let the timer off the leash by enabling interrupts
  _EINT();
}
/*-----------------------------------------------------------------------------------*/

u8_t extract_ip_part(char* ptr)
 { u8_t res = 0;
   while(*ptr && *ptr != '.')
    { res = res * 10 + (*ptr - '0');
      ptr++;
    }
   if (*ptr == '.')
      ptr++;
   return res;
 }

int
main(void)
{
  u16_t addr[2];
  char ips[16], *ptr;
  u8_t ip[4], i, def, arptimer = 0;
  const u8_t def_ip[] = {192, 168, 56, 102};
  const u8_t def_mask[] = {255, 255, 255, 0};
  const u8_t def_router[] = {127, 0, 0, 1};

  /* Configure clock */
  init_mcu();

  /* Configure Terminal */
  init_terminal();
  print_str("uIP for MSP430 (NIC driver RTL8019)\n\n");

  /* Initialize the uIP TCP/IP stack. */
  uip_init();
  uip_arp_init();
  
  /* Register the Ethernet MAC address with uIP. In reality, the MAC
     address should be obtained from the EEPROM of the network
     hardware before calling this function. */
//  uip_setethaddr(ethaddr);
  
  /* Setup the IP address and netmask. */
  print_str("Enter IP address (default 192.168.56.102):\n"); 
  read_str(ips, sizeof(ips) - 1);
  def = *ptr == 0 ? 1 : 0;
  for (i = 0; i < 4; i++)
   { ip[i] = def ? def_ip[i] : extract_ip_part(ptr);
   }
  uip_ipaddr(addr, ip[0],ip[1],ip[2],ip[3]);
  uip_sethostaddr(addr);

  print_str("Enter subnet mask (default 255.255.255.0):\n"); 
  read_str(ips, sizeof(ips) - 1);
  ptr = ips;
  def = *ptr == 0 ? 1 : 0;
  for (i = 0; i < 4; i++)
   { ip[i] = def ? def_mask[i] : extract_ip_part(ptr);
   }
  uip_ipaddr(addr, ip[0], ip[1], ip[2], ip[3]);
  uip_setnetmask(addr);

  /* Configure the IP address of the default router. */
  print_str("Enter IP of the router (default 127.0.0.1):\n"); 
  read_str(ips, sizeof(ips) - 1);
  ptr = ips;
  def = *ptr == 0 ? 1 : 0;
  for (i = 0; i < 4; i++)
   { ip[i] = def ? def_router[i] : extract_ip_part(ptr);
   }
  uip_ipaddr(addr, ip[0], ip[1], ip[2], ip[3]);
  uip_setdraddr(addr);

  /* Initialize the device driver. */ 
  print_str("NIC initialization... ");
  nic_init();
  print_str("OK\n");

  /* Initialize applications */
  init_tasks();
 
  /* Initialize the HTTP server. */
  print_str("HTTP server startup... ");
  httpd_init();
  print_str("OK\n");
  
  /* Initialize the TELNET server. */
  print_str("TELNET server startup... ");
  telnetd_init();
  print_str("OK\n");
  
  start = rt_ticks();
  arptimer = 0;

  print_str("\nRunning...\n");

  while(1) {
    /* Let the tapdev network device driver read an entire IP packet
       into the uip_buf. If it returns > 0, there is a packet in the
       uip_buf buffer. */
    uip_len = nic_poll();
    
    if(uip_len > 0) {
      /* A packet is present in the packet buffer. We call the
	 appropriate ARP functions depending on what kind of packet we
	 have received. If the packet is an IP packet, we should call
	 uip_input() as well. */
      if(BUF->type == htons(UIP_ETHTYPE_IP)) {
	uip_arp_ipin();
	uip_input();
	/* If the above function invocation resulted in data that
	   should be sent out on the network, the global variable
	   uip_len is set to a value > 0. */
	if(uip_len > 0) {
	  uip_arp_out();
	  nic_send();
	}
      } else if(BUF->type == htons(UIP_ETHTYPE_ARP)) {
	uip_arp_arpin();
	/* If the above function invocation resulted in data that
	   should be sent out on the network, the global variable
	   uip_len is set to a value > 0. */	
	if(uip_len > 0) {	
	  nic_send();
	}
      }
    } else {
      /* The poll function returned 0, so no packet was
	 received. Instead we check if there is time that we do the
	 periodic processing. */
      current = rt_ticks();
      
  if(current != start)       // (u16_t)RT_CLOCK_SECOND / 2)
   { start = current;
	  for(i = 0; i < UIP_CONNS; i++) 
      { uip_periodic(i);
	     /* If the above function invocation resulted in data that
   	     should be sent out on the network, the global variable
	       uip_len is set to a value > 0. */
	     if(uip_len > 0) 
         { uip_arp_out();
	        nic_send();
	      }
	   }

#if UIP_UDP
	for(i = 0; i < UIP_UDP_CONNS; i++) {
	  uip_udp_periodic(i);
	  /* If the above function invocation resulted in data that
	     should be sent out on the network, the global variable
	     uip_len is set to a value > 0. */
	  if(uip_len > 0) {
	    uip_arp_out();
	    tapdev_send();
	  }
	}
#endif /* UIP_UDP */
      
     /* Call the ARP timer function every 10 seconds. */
	  if(arptimer == 0) 
      { uip_arp_timer();
	     arptimer = 20;
	   }
     else
      arptimer--;
   }    
 }    
}
  return 0;
}
/*-----------------------------------------------------------------------------------*/
void
uip_log(char *m)
{
  // printf("uIP log message: %s\n", m);
}
/*-----------------------------------------------------------------------------------*/