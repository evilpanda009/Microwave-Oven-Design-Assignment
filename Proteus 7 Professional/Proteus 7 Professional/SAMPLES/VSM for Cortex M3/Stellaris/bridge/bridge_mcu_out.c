#include "bridge.h"

char blink = 0;
char enabled = 1;

void I2C0_Handler(void)
 { 
 }

void init_gpio()
 { SYSCTL->RCGC2 |= 0x00000002;   // enable GPIOB clocking for changing pin functions
   SYSCTL->RCGC2 |= 0x00000008;   // enable GPIOD clocking
   SYSCTL->RCGC2 |= 0x00000010;   // enable GPIOE clocking
   GPIOB->AFSEL = 0x0C;           // pin.2 and pin.3 are used by I2C0
   GPIOD->DIR = 0xFF;             // direction output
   GPIOE->DIR = 0xFF;             // direction output
 }

void init_i2c_slave()
 { SYSCTL->RCGC1 |= 0x00001000;   // enable I2C0 clocking (allow access to its registers)
   // for SYSCLK = 20 MHz and baudrate 100kbps
   I2C0_MASTER->MCR = 0x00000010; // initialize I2C Master  
   I2C0_MASTER->MCR = 0x00000020; // initialize I2C Slave  
   I2C0_SLAVE->SOAR = 0x0000003B; // sets the own slave address
   *(unsigned long*)&I2C0_SLAVE->SCSR = 0x00000001; // enable operation
 }

void delay()
 { int i;
   for (i=0; i < 5000; i++);
 }

// Write a command
void lcdwrite_command(unsigned char cmd)
 { GPIOE->DATA_Bits[0] = 0x00;            // rs=0, rw=0, e=0
   GPIOD->DIR = 0xFF;             // direction output
   GPIOD->DATA_Bits[0] = cmd;
   GPIOE->DATA_Bits[0] = 0x01;            // rs=0, rw=0, e=1
   delay();
   GPIOE->DATA_Bits[0] = 0x00;            // rs=0, rw=0, e=0
   delay();
 }

// Write Data
void lcdwrite_data(unsigned char data)
 { GPIOE->DATA_Bits[0] = 0x04;            // rs=1, rw=0, e=0
   GPIOD->DIR = 0xFF;             // direction output
   GPIOD->DATA_Bits[0] = data;
   GPIOE->DATA_Bits[0] = 0x05;            // rs=1, rw=0, e=1
   delay();
   GPIOE->DATA_Bits[0] = 0x04;           // rs=1, rw=0, e=0
   delay();
 }

void init_lcd()
 { lcdwrite_command(0x38); // 8-bits, 2 lines, 7x5 dots
   lcdwrite_command(0x0C); // no cursor, no blink, enable display
   lcdwrite_command(0x06); // auto-increment on
   lcdwrite_command(0x01); // clear screen
 }

char stricmp(const char* s1, const char* s2)
 { char ch1, ch2;
   while(*s1 && *s2)
    { ch1 = (*s1 >= 'A' && *s1<='Z') ? *s1 - 'A' + 'a' : *s1;
      ch2 = (*s2 >= 'A' && *s2<='Z') ? *s2 - 'A' + 'a' : *s2;
      DBG(ch1) DBG(' ') DBG(' ') DBG(ch2) DBG(0)
      if (ch1 != ch2)
         return 1;
      ++s1;
      ++s2;      
    }
   if (*s1 != *s2)
      return 1;
   return 0;   
 } 

void print_str(const char* s)
 { for(;*s;s++)
    { lcdwrite_data(*s);
    }
 }
 
void process_command(const char* s)
 { if (!stricmp(s, "clr"))
    { lcdwrite_command(0x01);
    }
   else 
   if (!stricmp(s, "blink"))
    { blink = !blink;
      lcdwrite_command(blink ? 0x0D : 0x0C);    // blink/normal
    }
   else 
   if (!stricmp(s, "bye"))
    { enabled = 0;
      lcdwrite_command(0x08);       //display OFF
    }
   else 
   if (!stricmp(s, "hello"))
    { enabled = 1;
      lcdwrite_command(0x01);       //display clear
      lcdwrite_command(0x0C);       //display ON
      lcdwrite_data('>');
      lcdwrite_data(' ');
    }
 }

int main()
 { unsigned char dt;
   unsigned long scsr;
   char bf[32];
   int sz = 0;
   char cmd = 0;

   init_gpio();
   init_i2c_slave();
   init_lcd();
   
   dt = 0;

   lcdwrite_data('>');
   lcdwrite_data(' ');

   while(1)
    { scsr = I2C0_SLAVE->SCSR;
      if (scsr & 0x01)
       { // receive request
         dt = I2C0_SLAVE->SDR;
         if (dt == '*')
          { if (!cmd)
             { cmd = 1;
             }
            else
             { cmd = 0;
               bf[sz]=0;
               sz=0;
               process_command(bf);
             }
          }
         else
          { if (cmd)
               bf[sz++] = dt;
            else
            if (enabled)
               lcdwrite_data(dt);
          }
       }
    }
   return 0;
 }