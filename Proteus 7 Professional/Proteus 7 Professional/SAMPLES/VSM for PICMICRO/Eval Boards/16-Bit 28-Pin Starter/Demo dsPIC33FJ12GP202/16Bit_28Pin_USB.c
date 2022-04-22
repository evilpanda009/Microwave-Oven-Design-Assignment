//*****************************************************************
//* 28pin_UART_TX-RX_Using_USB                                              		
//*****************************************************************

#include "p33FJ12GP202.h"
// the above include path may be different for each user.  If a compile
// time error appears then check the path for the file above and edit
// the include statement above.

_FOSCSEL( FNOSC_FRCPLL )

//#define XTFREQ          7370000         //FRC frequency 
#define FCY             6500000  // Reduced frequency for real time simulation purposes

#define BAUDRATE         9600		      
#define BRGVAL          ((FCY/BAUDRATE)/16)-1 

int main(void)
{

// Configure Oscillator to operate the device at 40Mhz
// Fosc= Fin*M/(N1*N2), Fcy=Fosc/2
// Fosc= 7.37*43/(2*2)=79.23MHz
	PLLFBD=5; //41;					// M=43
	CLKDIVbits.PLLPOST=0;		// N1=2
	CLKDIVbits.PLLPRE=0;		// N2=2

// Disable Watch Dog Timer
	RCONbits.SWDTEN=0;

// Wait for PLL to lock
	while(OSCCONbits.LOCK!=1) {};

	RPINR18 = 9;			// Make Pin RP9 U1RX
	RPOR4bits.RP8R = 3;		// Make Pin RP8 U1TX

	AD1PCFGL = 0x03C0;		// Make analog pins digital 

	LATB = 0x0000;
	TRISB = 0x0FFF;			// Config LED's as output and U1RX as Input

	TMR1 = 0;				// clear timer 1
	PR1 = 0x1FFF;			// interrupt every 400ms
	IFS0bits.T1IF = 0;		// clr interrupt flag
	IEC0bits.T1IE = 1;		// set interrupt enable bit
	T1CON = 0x8030;			// 1:64 prescale, start TMR1


	U1BRG  = BRGVAL;
	U1MODE = 0x8000; /* Reset UART to 8-n-1, alt pins, and enable */
	U1STA  = 0x0440; /* Reset status register and enable TX & RX*/


_U1RXIF=0;
	
while(1)
	{
	
		int a;
		
		while (_U1RXIF==0);			// Wait and Receive One Character
		a = U1RXREG;
		
		while(!U1STAbits.TRMT);
		U1TXREG = '"';
	
		while(!U1STAbits.TRMT);		// Echo Back Received Character with quotes
		U1TXREG = a;
	
		while(!U1STAbits.TRMT);
		U1TXREG = '"';
	
		while(!U1STAbits.TRMT);
		U1TXREG = ' '; 	
	
		_U1RXIF=0;					// Clear UART RX Interrupt Flag
}

return 0;
}

void __attribute__((__interrupt__, no_auto_psv)) _T1Interrupt(void)
{
	IFS0bits.T1IF = 0;	// clear interrupt flag
	
	LATB ^= 0xF000;		//Toggle LED's
}
