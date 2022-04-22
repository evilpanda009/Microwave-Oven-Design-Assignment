#include "bridge.h"

enum {MAXBF=16};
char bf[MAXBF];
char* ptr_head;
char* ptr_tail;
int cnt;

bool fifo_isfull()
 { return cnt == MAXBF;
 }

bool fifo_isempty()
 { return cnt == 0;
 }
  
void fifo_write(char ch)
 { ++cnt;   
   *ptr_head = ch;
   ++ptr_head;
   if (ptr_head == &bf[MAXBF])
      ptr_head = &bf[0];
 }

char fifo_read()
 { char ch;
   --cnt;   
   ch = *ptr_tail;
   ++ptr_tail;
   if (ptr_tail == &bf[MAXBF])
      ptr_tail = &bf[0];
   return ch;
 }

void UART0_Handler(void)
 { char ch;
   while((UART0->FR & 0x0010) == 0 && !fifo_isfull())
    { ch = UART0->DR;
      fifo_write(ch);
      DBG('i') DBG('n') DBG(' ') DBG(' ') DBG(ch) DBG(0)
    }
 }

void init_gpio()
 { SYSCTL->RCGC2 |= 0x00000002;   // enable GPIOB clocking for changing pin functions
   SYSCTL->RCGC2 |= 0x00000001;   // enable GPIOA clocking for changing pin functions
   GPIOA->AFSEL = 3;              // pin.0 and pin.1 are used by UART0
   GPIOB->AFSEL = 0x0C;           // pin.2 and pin.3 are used by I2C0
 }

void init_uart()
 { SYSCTL->RCGC1 |= 0x00000001;   // enable UART0 clocking (allow access to its registers)
   // for SYSCLK = 25 MHz and baudrate 57600
   UART0->CTL &= 0xFFFFFFFE;      // disable UART
   UART0->IBRD = 19;              // integer part of baudrate = 25M / (16 * 57600)
   UART0->FBRD = 34;               // fractional part of baudrate = (0,127*64 + 0,5)
   UART0->LCRH = 0x00000060;      // setup data format: 8 bits, fifo OFF
   UART0->CTL |= 0x00000301;      // enable UART, TX and RX
   UART0->IM |= 0x00000010;       // enable Rx interrupt

   NVIC_EnableIRQ(UART0_IRQn);
 }

void init_uart_old()
 { SYSCTL->RCGC1 |= 0x00000001;   // enable UART0 clocking (allow access to its registers)
   // for SYSCLK = 25 MHz and baudrate 57600
   UART0->CTL &= 0xFFFFFFFE;      // disable UART
   UART0->IBRD = 27;              // integer part of baudrate = 25M / (16 * 57600)
   UART0->FBRD = 9;               // fractional part of baudrate = (0,127*64 + 0,5)
   UART0->LCRH = 0x00000070;      // setup data format: 8 bits, fifo ON
   UART0->CTL |= 0x00000301;      // enable UART, TX and RX
   UART0->IM |= 0x00000010;       // enable Rx interrupt

   NVIC_EnableIRQ(UART0_IRQn);
 }
 
void init_i2c_master()
 { SYSCTL->RCGC1 |= 0x00001000;   // enable I2C0 clocking (allow access to its registers)
   I2C0_MASTER->MCR = 0x00000010; // initialize I2C Master  
   // for SYSCLK = 20 MHz and baudrate 100kbps
   I2C0_MASTER->MTPR = 9;         // prescaler for 100kbps
   I2C0_MASTER->MSA = 0x00000076; // sets the slave address to 0x3B with Transmit mode
 }

void print_str(const char* s)
 { while(*s)
    { while(UART0->FR & 0x0020)
       { // fifo is full... wait
       }
      UART0->DR = *s;
      s++;
    }
 }
 
int main()
 { char ch;
   ptr_head = &bf[0];
   ptr_tail = &bf[0];
   cnt = 0;

   init_gpio();
   init_uart();
   init_i2c_master();

   __enable_irq();
   
   print_str("Enter text here: ");

   while(1)
    { // check the status of I2C module
      if ((I2C0_MASTER->MCS & 0x00000040)==0 && !fifo_isempty())
       { // transmit request
         ch = fifo_read();
         I2C0_MASTER->MDR = ch;
         I2C0_MASTER->MCS = 0x00000007; // single byte send of the data from Master to Slave (START, RUN, STOP)         
         DBG('o') DBG('u') DBG('t') DBG(' ') DBG(ch) DBG(0)         
       }
    }
   return 0;
 }