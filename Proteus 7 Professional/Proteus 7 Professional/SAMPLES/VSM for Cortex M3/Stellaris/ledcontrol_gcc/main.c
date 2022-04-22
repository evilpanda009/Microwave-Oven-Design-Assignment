#include <lm3s_cmsis.h>

int main(void)
 { SYSCTL->RCGC2 |= 0x0000001F;   // enable GPIOA,GPIOB,GPIOC,GPIOD,GPIOE clocking for changing pin functions
   GPIOD->AFSEL = 0x00;           // all pins are GPIO
   GPIOD->DIR = 0x01;             // pin.0 is output
   GPIOD->DATA = 0;
   int i;
   char ledstate = 0;
   for(;;)
	 { for (i = 0; i < 10000; i++);
      ledstate = ~ledstate;
      GPIOD->DATA = ledstate & 1;
	 }
	return 0;
 }
