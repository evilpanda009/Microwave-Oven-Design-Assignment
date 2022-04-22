#include "include\lm3s_cmsis.h"
#include "include\system_lm3s.h"

typedef char bool;

#define ESC    0x1B

#define DBG(a) *(unsigned char*)0xffffff00 = a;

#define print_ch(ch)                      \
 { DBG(ch)                                \
   DBG(0)                                 \
 }

 #define print(s)                         \
 { for(char* ptr = s; ptr && *ptr; ++ptr) \
    { DBG(*ptr)                           \
    } DBG(0)                              \
 }

enum {MAXBF=256};
char bf[MAXBF];
int cnt;
char ch;
bool ch_rdy;
bool btn_rdy;
bool prevent_echo;
bool cmpvalue;

#define EXTMEM_ADDR     0x50


void UART0_Handler(void)
 { ch = UART0->DR;
   if (!prevent_echo)
    { UART0->DR = ch;
      UART1->DR = ch;
    }
   DBG('i') DBG('n') DBG('0') DBG(' ') DBG(' ') DBG(ch) DBG(0)
   ch_rdy = 1;
 }

void UART1_Handler(void)
 { ch = UART1->DR;
   if (!prevent_echo)
    { UART0->DR = ch;
      UART1->DR = ch;
    }
   DBG('i') DBG('n') DBG('1') DBG(' ') DBG(' ') DBG(ch) DBG(0)
   ch_rdy = 1;
 }

void PORTD_Handler(void)
 { GPIOD->ICR = 0;
   btn_rdy = 1;
 }

void ADC_Handler(void)
 { ADC->ISC = 0x00000000;                    // clear interrupt status
   GPIOC->DATA = ADC->SSFIFO3 >> 2;          // output higher 8 bits
 }

void COMP0_Handler(void)
 { COMP->ACMIS &= ~1;
   cmpvalue = !cmpvalue;
   GPIOE->DATA = cmpvalue;
 }


void init_gpio()
 { SYSCTL->RCGC2 |= 0x0000001F;   // enable GPIOA,GPIOB,GPIOC,GPIOD,GPIOE clocking for changing pin functions
   GPIOA->AFSEL = 3;              // pin.0 and pin.1 are used by UART0
   GPIOB->AFSEL = 0x5C;           // pin.2 and pin.3 are used by I2C0, pin.4 and pin.6 are used by COMP0
   GPIOC->AFSEL = 0x00;           // pin.0-7 are GPIOs
   GPIOC->DIR = 0xFF;             // pin.0-7 are output
   GPIOD->AFSEL = 0x0C;           // pin.2 and pin.3 are used by UART1
   GPIOD->DIR = 0x02;             // pin.0 is input, pin.1 is output
   GPIOD->IM = 0x01;              // enable interrupt from pin.0
   GPIOD->DATA = 0;
   GPIOE->DIR = 0x01;             // pin.0 is output
 }

void init_uart()
 { SYSCTL->RCGC1 |= 0x00000003;   // enable UART0, UART1 clocking (allow access to their registers)
   // for SYSCLK = 20 MHz and baudrate 57600
   UART0->CTL &= 0xFFFFFFFE;      // disable UART
   UART0->IBRD = 21;              // integer part of baudrate = 20M / (16 * 57600)
   UART0->FBRD = 45;              // fractional part of baudrate = (0,701*64 + 0,5)
   UART0->LCRH = 0x00000060;      // setup data format: 8 bits, fifo OFF
   UART0->IM |= 0x00000010;       // enable Rx interrupt
   UART0->CTL |= 0x00000301;      // enable UART, TX and RX
   // for SYSCLK = 20 MHz and baudrate 19200
   UART1->CTL &= 0xFFFFFFFE;      // disable UART
   UART1->IBRD = 65;              // integer part of baudrate = 25M / (16 * 19200)
   UART1->FBRD = 7;               // fractional part of baudrate = (0,104*64 + 0,5)
   UART1->LCRH = 0x00000060;      // setup data format: 8 bits, fifo OFF
   UART1->IM |= 0x00000010;       // enable Rx interrupt
   UART1->CTL |= 0x00000301;      // enable UART, TX and RX

   NVIC_EnableIRQ(UART0_IRQn);
   NVIC_EnableIRQ(UART1_IRQn);
 }

void init_i2c_master()
 { SYSCTL->RCGC1 |= 0x00001000;   // enable I2C0 clocking (allow access to its registers)
   I2C0_MASTER->MCR = 0x00000010; // initialize I2C Master  
   // for SYSCLK = 20 MHz and baudrate 100kbps
   I2C0_MASTER->MTPR = 9;         // prescaler for 100kbps
 }

#define ENABLE_TIMER0      TIMER0->CTL |= 0x00000021;
#define DISABLE_TIMER0     TIMER0->CTL &= ~0x00000101;

void init_adc()
 { SYSCTL->RCGC0 |= 0x00011000;   // enable ADC clocking and 250K samples/sec
   ADC->EMUX = 0x00005000;        // SS3 trigger is Timer
   ADC->SAC = 0x00000000;         // no hardware oversampling
   ADC->SSMUX3 = 0x00000001;      // pin ADC1 is the source analogue input
   ADC->SSCTL3 = 0x00000006;      // 1st Sample is End of Sequence, interrupt enable
   ADC->ACTSS = 0x00000008;       // enable SS3
   ADC->ISC = 0x00000008;         // clear current SS3 interrupt state
   ADC->IM = 0x00000008;          // enable SS3 interrupt
   NVIC_EnableIRQ(ADCSeq3_IRQn);  // enable SS3 global interrupt
   
   // tune Timer0 to start ADC (required)
   SYSCTL->RCGC1 |= 0x00010000;   // enable Timer0 clocking
   DISABLE_TIMER0
   TIMER0->CFG = 0x00000000;      // 32-bit timer configuration
   TIMER0->TAMR = 0x00000002;     // periodic timer mode
   TIMER0->TBMR = 0x00000000;
   TIMER0->TAILR = 200;           // tick every 10us = 200 / 20MHz
   TIMER0->TBILR = 0;
 }

void init_cmp()
 { SYSCTL->RCGC1 |= 0x01000000;   // enable COMP0 clocking (allow access to its registers)
   COMP->ACCTL0 = 0x0000020C;     // compare with C0+, interrupt enabled on every output edge
   COMP->ACINTEN = 0;             // disable interrupt from COMP0
   NVIC_EnableIRQ(Comp0_IRQn);
 }

#define PRINTCH(c)    while(UART0->FR & 0x0020 || UART1->FR & 0x0020); UART0->DR = c; UART1->DR = c;

void print_str(const char* s)
 { while(*s)
    { if (*s == '\n')
       { PRINTCH(10)
         PRINTCH(13)
       }
      else
         PRINTCH(*s)
      s++;
    }
 }

#define WAIT_I2C     while(I2C0_MASTER->MCS & 0x00000001);

void i2c_send(const char* str)
 { I2C0_MASTER->MSA = EXTMEM_ADDR<<1; // sets the slave address to 0x50 with Transmit mode
   I2C0_MASTER->MDR = 0;
   I2C0_MASTER->MCS = 0x00000003; // START, RUN
   WAIT_I2C
   while(*str)
    { // check the status of I2C module
      //for(i=0;i<100;i++);
      I2C0_MASTER->MDR = *str;
      I2C0_MASTER->MCS = 0x00000001; // RUN
      WAIT_I2C
      DBG('i') DBG('2') DBG('c') DBG('>') DBG(' ') DBG(*str) DBG(0)
      str++;
    }
   I2C0_MASTER->MDR = 0;
   I2C0_MASTER->MCS = 0x00000005; // STOP, RUN
   WAIT_I2C
 }

void action_writemem()
 { // Scenario. Write entered string into the external memory using I2C interface
   // WARN! bf[0] must contains 0 = starting address for transfer
   print_str("\nEnter text here (the dot closes up the string): ");
   ch_rdy = 0;
   cnt = 0;
   while(1)
    { if (!ch_rdy)
         continue;
      ch_rdy = 0;
      bf[cnt++] = ch;
      if (ch == '.')
       { bf[cnt++] = 0;       // trailing zero
         break;
       }
    }

   print_str("\nsent: ");
   print_str(bf);
   i2c_send(bf);
 }

void action_readmem()
 { // Scenario. Read saved string from the external memory using I2C interface
   // Reset target address
   I2C0_MASTER->MSA = EXTMEM_ADDR<<1; // sets the slave address to 0x50 with Transmit mode
   I2C0_MASTER->MDR = 0;
   I2C0_MASTER->MCS = 0x00000007; // START, RUN, STOP
   // clear buffer
   while(cnt)
    { --cnt;
      bf[cnt] = 0;
    }
   WAIT_I2C
   I2C0_MASTER->MSA = (EXTMEM_ADDR<<1) + 1; // sets the slave address to 0x50 with Receive mode
   I2C0_MASTER->MCS = 0x0000000B; // ACK, START, RUN
   while(cnt<MAXBF)
    { WAIT_I2C
      ch = I2C0_MASTER->MDR;
      if (ch)
       { DBG('i') DBG('2') DBG('c') DBG('<') DBG(' ') DBG(ch) DBG(0)         
       }
      else
         break;
      bf[cnt] = ch;   
      cnt++;
    }
   I2C0_MASTER->MCS = 0x00000005; // NACK, STOP, RUN
   WAIT_I2C
   // fake read
   ch = I2C0_MASTER->MDR;
   print_str("\nText: ");
   print_str(bf);
 }

void action_ledcontrol()
 { // Scenario. Turn on/off the LED pressing the button 
   prevent_echo = 1;
   ch_rdy = 0;
   btn_rdy = 0;
   bool ledstate = 0;
   NVIC_EnableIRQ(GPIOPortD_IRQn);
   print_str("\nPress the button to switch on/off the LED (or ESC to exit)...\n");
   while(1)
    { if (ch_rdy)
       { if (ch == ESC)
            break;
       }
      if (btn_rdy)
       { ledstate = !ledstate;
         GPIOD->DATA = ledstate ? 2 : 0;
         print_str(ledstate ? "LED is ON\n" : "LED is OFF\n");
         btn_rdy = 0;
       }
    }
   NVIC_DisableIRQ(GPIOPortD_IRQn);
   prevent_echo = 0;
 }

void action_adc()
 { // Scenario. Perform adc-dac conversion
   prevent_echo = 1;
   ENABLE_TIMER0
   print_str("\nPress ESC to exit...\n");
   while(1)
    { if (ch_rdy)
       { if (ch == ESC)
            break;
       }
    }
   DISABLE_TIMER0
   prevent_echo = 0;
 }

void action_cmp()
 { // Scenario. Compare two input signals
   prevent_echo = 1;
   GPIOE->DATA = 0;
   cmpvalue = 0;
   COMP->ACINTEN = 1;             // enable interrupt from COMP0
   print_str("\nPress ESC to exit...\n");
   while(1)
    { if (ch_rdy)
       { if (ch == ESC)
            break;
       }
    }
   COMP->ACINTEN = 0;             // disable interrupt from COMP0
   prevent_echo = 0;
 }

int select_action()
 { print_str("Select action here:\n");
   print_str(" 0: save string in the external memory\n");
   print_str(" 1: read string from the external memory\n");
   print_str(" 2: control the LED using the button\n");
   print_str(" 3: perfrom adc-dac conversion\n");
   print_str(" 4: compare two signals (PE0 outputs result)\n");
   print_str("\n> ");
   ch_rdy = 0;
   while(1)
    { if (!ch_rdy)
         continue;
      switch(ch)
       { case '0': return 0;
         case '1': return 1;
         case '2': return 2;
         case '3': return 3;
         case '4': return 4;
         default : print_str("\n> "); break;
       }
      ch_rdy = 0;
    }
 }
 
int main()
 { ch_rdy = 0;
   cnt = 1;
   bf[0] = 0;
   prevent_echo = 0;

   init_gpio();
   init_uart();
   init_i2c_master();
   init_adc();
   init_cmp();

   __enable_irq();

   print_str("*** Stellaris Demo using Luminary Micro LM3S308 ***\n");

   while(1)
    { switch(select_action())
       { case 0: action_writemem(); break;
         case 1: action_readmem(); break;
         case 2: action_ledcontrol(); break;
         case 3: action_adc(); break;
         case 4: action_cmp(); break;
       }
      print_str("\n*** Demo action is over ***\n\n");
    }


   return 0;
 }