/***********************************************************************
 *
 *  PICDEM LCD2 Demo Board 946 PIM
 *
 ***********************************************************************
 * FileName:            main.C
 * Dependencies:
 * Processor:           PIC16F946
 * Assembler:
 * Linker:
 * Compiler:
 * Company:             Microchip Technology, Inc.
 *
 * Software License Agreement
 *
 * The software supplied herewith by Microchip Technology Incorporated
 * (the "Company") for its PICmicro® Microcontroller is intended and
 * supplied to you, the Company's customer, for use solely and
 * exclusively on Microchip PICmicro Microcontroller products. The
 * software is owned by the Company and/or its supplier, and is
 * protected under applicable copyright laws. All rights are reserved.
 * Any use in violation of the foregoing restrictions may subject the
 * user to criminal sanctions under applicable laws, as well as to
 * civil liability for the breach of the terms and conditions of this
 * license.
 *
 * THIS SOFTWARE IS PROVIDED IN AN "AS IS" CONDITION. NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
 * TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
 * IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
 * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 *
 * ANY SPECIAL DESCRIPTION THAT MIGHT BE USEFUL FOR THIS FILE.
 *
 * Author               Date            Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 **********************************************************************/



#include <htc.h>
#include "lcdmap.h"


// Configuration Bits

__CONFIG(INTIO & WDTDIS & PWRTDIS & MCLRDIS & BORDIS & IESODIS & FCMDIS & DEBUGDIS);


// variable declarations

unsigned char hours, minutes, seconds, Mode, SW1, SW2;
unsigned char tens,ones;
unsigned char numBars;
unsigned int boardVoltage, boardTemp;
bit timeFlag;

// Function Prototypes
void init(void); 		//Initialization Routine
void clearscreen(void);
void displayNumber(unsigned char Position, unsigned char Number);
void numberBreak(unsigned char number);
void displayBars(unsigned char value);
void displayTime(void);
void displayVoltage(void);
void display4dig(unsigned int number);
void getTemperature(void);
void displayTemp(unsigned int number);
void tickDisplay(unsigned char value);
void delay(void);


void interrupt ISR(void){

	if(TMR1IF && TMR1IE){
		timeFlag = 1; 	// and update has occured, set time flag
		TMR1IF = 0;	
		TMR1H =+ 0x80;	// preload TMR1 for 1 second roll over
		seconds++;		
		if (seconds > 60){
			minutes++;
			seconds = 0;
			if (minutes > 60){
				hours++;
				minutes = 0;
				if (hours >12){
					hours = 1;
				}
			}
		}
	}

	if(RBIF && RBIE){
		SW1 = 0;
		SW2 = 0;

		if(RB6==0){SW1=1;}
		if(RB7==0){SW2=1;}

		RBIF = 0;

	}


}

void init(){

//	TRISA = TRISB = TRISC = TRISD = TRISF = TRISG = 1;

	// Configure LCD

	LCDPS 	= 0b00000001;
	LCDSE0 	= 0b00111110;
	LCDSE1 	= 0b10000100;
	LCDSE2 	= 0xff;
	LCDSE3  = 0xff;
	LCDSE4 	= 0b11111100;
	LCDSE5 	= 0xff;

	LCDCON 	= 0b11011011;

	clearscreen();

	// Configure Timer 1
	T1CON 	= 0b00001111;

	// Configure analog ports
	ANS1 = ANS0 = 1;		// Enable AN0, AN1 analog ports
	ADCON0 = 0b10000001;	// left justify, Vss, Vdd,  CH0, DONE, ON
	ADCS0 = 1;
	ADCS1 = 1;
	ADCS2 = 0;

	// Initialize Variables
	minutes = seconds = 0; 
	hours = 12;
	numBars = 0;
	Mode = 1;

	// Turn on Colon for RTC
	COL = 1;
	TRISB0 = 0;
	TRISB7 = 1;
	TRISB6 = 1;
	RB0 = 1;		// turn on RB0
	IOCB6 = 1;
	IOCB7 = 1;
	// Configure Interrupts
	TMR1IF 	= 0; 
	TMR1IE 	= 1; 
	RBIF 	= 0;
	RBIE	= 1;

	PEIE = 1; 
	GIE = 1; 
}

void clearscreen(){
	LCDDATA0 = 0;
	LCDDATA1 = 0;
	LCDDATA2 = 0;
	LCDDATA3 = 0;
	LCDDATA4 = 0;
	LCDDATA5 = 0;
	LCDDATA6 = 0;
	LCDDATA7 = 0;
	LCDDATA8 = 0;
	LCDDATA9 = 0;
	LCDDATA10 = 0;
	LCDDATA11 = 0;
	LCDDATA12 = 0;
	LCDDATA13 = 0;
	LCDDATA14 = 0;
	LCDDATA15 = 0;
	LCDDATA16 = 0;
	LCDDATA17 = 0;
	LCDDATA18 = 0;
	LCDDATA19 = 0;
	LCDDATA20 = 0;
	LCDDATA21 = 0;
	LCDDATA22 = 0;
	LCDDATA23 = 0;
	S1 = 1;			// always turn mchp logo on
	S13 = 1;		// always turn varitronix logo on
}

void main(){

	init();
		

	while(1){
		

		while(Mode == 1){
			F1 = 1;	// turn on F1 segment
			COL = 1; //turn on colon
			if (timeFlag){
				displayTime();
				timeFlag = 0;
			}
			
			if(SW1 == 1){	// Switch 1 pressed?
				Mode = 2;	// if so, change mode
				SW1 = 0;	//	reset switch
				clearscreen(); // clear screen
				delay();
			}

			if(SW2 == 1){	// Switch 2 pressed?
				minutes++; 	// if so, increment time
				if(minutes == 60){
					minutes = 0;
					hours ++;
					if(hours>12){
						hours = 1;
					}
				}
				SW2 = 0;
				timeFlag = 1;
				delay();
			}

		}
		while(Mode == 2){
			F2 = 1;	

			displayVoltage();

			if(SW1 == 1){
				Mode = 3;
				SW1 = 0;
				clearscreen();
				delay();
			}
		
			if(SW2 == 1){
				SW2 = 0;
				delay();
			}

			
		}
		while(Mode == 3){
			F3 = 1; 
			
			getTemperature();

			if(SW1 == 1){
				Mode = 1;
				SW1 = 0;
				clearscreen();
				delay();
			}
		
			if(SW2 == 1){
				SW2 = 0;
				delay();
			}
		
		}
	
	}
}


void displayNumber(unsigned char Position, unsigned char Number){
	
	switch(Position){
	case 1:
		D1A = D1B = D1C = D1D = D1E = D1F = D1G = 0; //intialize cleared
		switch(Number){
		case 1:
			D1B = D1C = 1;
			break;
		case 2:
			D1A = D1B = D1G = D1E = D1D = 1;
			break;
		case 3:
			D1A = D1B = D1G = D1C = D1D = 1;
			break;
		case 4:
			D1F = D1G = D1B = D1C = 1;
			break;
		case 5:
			D1A = D1F = D1G = D1C = D1D = 1;
			break;
		case 6:
			D1A = D1F = D1E = D1D = D1C = D1G = 1;
			break;
		case 7:
			D1A = D1B = D1C = 1;
			break;
		case 8:
			D1A = D1B = D1C = D1D = D1E = D1F = D1G = 1;
			break;
		case 9:
			D1A = D1B = D1C = D1D = D1F = D1G = 1;
			break;
		case 0:
			D1A = D1B = D1C = D1D = D1E = D1F = 1;
			break;
		}
		break;
	case 2:
		D2A = D2B = D2C = D2D = D2E = D2F = D2G = 0; //intialize cleared
		switch(Number){

		case 1:
			D2B = D2C = 1;
			break;
		case 2:
			D2A = D2B = D2G = D2E = D2D = 1;
			break;
		case 3:
			D2A = D2B = D2G = D2C = D2D = 1;
			break;
		case 4:
			D2F = D2G = D2B = D2C = 1;
			break;
		case 5:
			D2A = D2F = D2G = D2C = D2D = 1;
			break;
		case 6:
			D2A = D2F = D2E = D2D = D2C = D2G = 1;
			break;
		case 7:
			D2A = D2B = D2C = 1;
			break;
		case 8:
			D2A = D2B = D2C = D2D = D2E = D2F = D2G = 1;
			break;
		case 9:
			D2A = D2B = D2C = D2D = D2F = D2G = 1;
			break;
		case 0:
			D2A = D2B = D2C = D2D = D2E = D2F = 1;
			break;
		}
		break;
	case 3:
		D3A = D3B = D3C = D3D = D3E = D3F = D3G = 0; //intialize cleared
		switch(Number){
		case 1:
			D3B = D3C = 1;
			break;
		case 2:
			D3A = D3B = D3G = D3E = D3D = 1;
			break;
		case 3:
			D3A = D3B = D3G = D3C = D3D = 1;
			break;
		case 4:
			D3F = D3G = D3B = D3C = 1;
			break;
		case 5:
			D3A = D3F = D3G = D3C = D3D = 1;
			break;
		case 6:
			D3A = D3F = D3E = D3D = D3C = D3G = 1;
			break;
		case 7:
			D3A = D3B = D3C = 1;
			break;
		case 8:
			D3A = D3B = D3C = D3D = D3E = D3F = D3G = 1;
			break;
		case 9:
			D3A = D3B = D3C = D3D = D3F = D3G = 1;
			break;
		case 0:
			D3A = D3B = D3C = D3D = D3E = D3F = 1;
			break;
		}
		break;
	case 4:
		D4A = D4B = D4C = D4D = D4E = D4F = D4G = 0; //intialize cleared
		switch(Number){
		case 1:
			D4B = D4C = 1;
			break;
		case 2:
			D4A = D4B = D4G = D4E = D4D = 1;
			break;
		case 3:
			D4A = D4B = D4G = D4C = D4D = 1;
			break;
		case 4:
			D4F = D4G = D4B = D4C = 1;
			break;
		case 5:
			D4A = D4F = D4G = D4C = D4D = 1;
			break;
		case 6:
			D4A = D4F = D4E = D4D = D4C = D4G = 1;
			break;
		case 7:
			D4A = D4B = D4C = 1;
			break;
		case 8:
			D4A = D4B = D4C = D4D = D4E = D4F = D4G = 1;
			break;
		case 9:
			D4A = D4B = D4C = D4D = D4F = D4G = 1;
			break;
		case 0:
			D4A = D4B = D4C = D4D = D4E = D4F = 1;
			break;
		}
		break;
	case 5:
		D5A = D5B = D5C = D5D = D5E = D5F = D5G = 0; //intialize cleared
		switch(Number){

		case 1:
			D5B = D5C = 1;
			break;
		case 2:
			D5A = D5B = D5G = D5E = D5D = 1;
			break;
		case 3:
			D5A = D5B = D5G = D5C = D5D = 1;
			break;
		case 4:
			D5F = D5G = D5B = D5C = 1;
			break;
		case 5:
			D5A = D5F = D5G = D5C = D5D = 1;
			break;
		case 6:
			D5A = D5F = D5E = D5D = D5C = D5G = 1;
			break;
		case 7:
			D5A = D5B = D5C = 1;
			break;
		case 8:
			D5A = D5B = D5C = D5D = D5E = D5F = D5G = 1;
			break;
		case 9:
			D5A = D5B = D5C = D5D = D5F = D5G = 1;
			break;
		case 0:
			D5A = D5B = D5C = D5D = D5E = D5F = 1;
			break;
		}
		break;
	case 6:
		D6A = D6B = D6C = D6D = D6E = D6F = D6G = 0; //intialize cleared
		switch(Number){

		case 1:
			D6B = D6C = 1;
			break;
		case 2:
			D6A = D6B = D6G = D6E = D6D = 1;
			break;
		case 3:
			D6A = D6B = D6G = D6C = D6D = 1;
			break;
		case 4:
			D6F = D6G = D6B = D6C = 1;
			break;
		case 5:
			D6A = D6F = D6G = D6C = D6D = 1;
			break;
		case 6:
			D6A = D6F = D6E = D6D = D6C = D6G = 1;
			break;
		case 7:
			D6A = D6B = D6C = 1;
			break;
		case 8:
			D6A = D6B = D6C = D6D = D6E = D6F = D6G = 1;
			break;
		case 9:
			D6A = D6B = D6C = D6D = D6F = D6G = 1;
			break;
		case 0:
			D6A = D6B = D6C = D6D = D6E = D6F = 1;
			break;
		}
		break;
	case 7:
		D7A = D7B = D7C = D7D = D7E = D7F = D7G = 0; //intialize cleared
		switch(Number){

		case 1:
			D7B = D7C = 1;
			break;
		case 2:
			D7A = D7B = D7G = D7E = D7D = 1;
			break;
		case 3:
			D7A = D7B = D7G = D7C = D7D = 1;
			break;
		case 4:
			D7F = D7G = D7B = D7C = 1;
			break;
		case 5:
			D7A = D7F = D7G = D7C = D7D = 1;
			break;
		case 6:
			D7A = D7F = D7E = D7D = D7C = D7G = 1;
			break;
		case 7:
			D7A = D7B = D7C = 1;
			break;
		case 8:
			D7A = D7B = D7C = D7D = D7E = D7F = D7G = 1;
			break;
		case 9:
			D7A = D7B = D7C = D7D = D7F = D7G = 1;
			break;
		case 0:
			D7A = D7B = D7C = D7D = D7E = D7F = 1;
			break;
		}
		break;
	case 8:
		D8A = D8B = D8C = D8D = D8E = D8F = D8G = 0; //intialize cleared
		switch(Number){

		case 1:
			D8B = D8C = 1;
			break;
		case 2:
			D8A = D8B = D8G = D8E = D8D = 1;
			break;
		case 3:
			D8A = D8B = D8G = D8C = D8D = 1;
			break;
		case 4:
			D8F = D8G = D8B = D8C = 1;
			break;
		case 5:
			D8A = D8F = D8G = D8C = D8D = 1;
			break;
		case 6:
			D8A = D8F = D8E = D8D = D8C = D8G = 1;
			break;
		case 7:
			D8A = D8B = D8C = 1;
			break;
		case 8:
			D8A = D8B = D8C = D8D = D8E = D8F = D8G = 1;
			break;
		case 9:
			D8A = D8B = D8C = D8D = D8F = D8G = 1;
			break;
		case 0:
			D8A = D8B = D8C = D8D = D8E = D8F = 1;
			break;
		}
		break;
	}
}

void numberBreak(unsigned char number){
	tens = 0;
	ones = 0;
	while(number >= 10){
		number = number - 10;
		tens ++;
	}
	
	ones=number;

}


void displayBars(unsigned char value){
	switch(value){
		case 0:
			X1=X2=X3=X4=X5=X6=X7=X8=X9=X10=X11=X12=X13=X14=X15=X16=X17=X18=X19=X20=0;
			break;
		case 1:
			X1=1;
			X2=X3=X4=X5=X6=X7=X8=X9=X10=X11=X12=X13=X14=X15=X16=X17=X18=X19=X20=0;
			break;
		case 2:
			X2=1;X1=1;
			X3=X4=X5=X6=X7=X8=X9=X10=X11=X12=X13=X14=X15=X16=X17=X18=X19=X20=0;
			break;
		case 3:
			X3=1;X2=1;X1=1;
			X4=X5=X6=X7=X8=X9=X10=X11=X12=X13=X14=X15=X16=X17=X18=X19=X20=0;
			break;
		case 4:
			X4=1;X3=1;X2=1;X1=1;
			X5=X6=X7=X8=X9=X10=X11=X12=X13=X14=X15=X16=X17=X18=X19=X20=0;
			break;
		case 5:
			X5=1;X4=1;X3=1;X2=1;X1=1;
			X6=X7=X8=X9=X10=X11=X12=X13=X14=X15=X16=X17=X18=X19=X20=0;
			break;
		case 6:
			X6=1;X5=1;X4=1;X3=1;X2=1;X1=1;
			X7=X8=X9=X10=X11=X12=X13=X14=X15=X16=X17=X18=X19=X20=0;
			break;
		case 7:
			X7=1;X6=1;X5=1;X4=1;X3=1;X2=1;X1=1;
			X8=X9=X10=X11=X12=X13=X14=X15=X16=X17=X18=X19=X20=0;
			break;
		case 8:
			X8=1;X7=1;X6=1;X5=1;X4=1;X3=1;X2=1;X1=1;
			X9=X10=X11=X12=X13=X14=X15=X16=X17=X18=X19=X20=0;
			break;
		case 9:
			X9=1;X8=1;X7=1;X6=1;X5=1;X4=1;X3=1;X2=1;X1=1;
			X10=X11=X12=X13=X14=X15=X16=X17=X18=X19=X20=0;
			break;
		case 10:
			X10=1;X9=1;X8=1;X7=1;X6=1;X5=1;X4=1;X3=1;X2=1;X1=1;
			X11=X12=X13=X14=X15=X16=X17=X18=X19=X20=0;
			break;
		case 11:
			X11=1;X10=1;X9=1;X8=1;X7=1;X6=1;X5=1;X4=1;X3=1;X2=1;X1=1;
			X12=X13=X14=X15=X16=X17=X18=X19=X20=0;
			break;
		case 12:
			X12=1;X11=1;X10=1;X9=1;X8=1;X7=1;X6=1;X5=1;X4=1;X3=1;X2=1;X1=1;
			X13=X14=X15=X16=X17=X18=X19=X20=0;
			break;
		case 13:
			X13=1;X12=1;X11=1;X10=1;X9=1;X8=1;X7=1;X6=1;X5=1;X4=1;X3=1;X2=1;X1=1;
			X14=X15=X16=X17=X18=X19=X20=0;
			break;
		case 14:
			X14=1;X13=1;X12=1;X11=1;X10=1;X9=1;X8=1;X7=1;X6=1;X5=1;X4=1;X3=1;X2=1;X1=1;
			X15=X16=X17=X18=X19=X20=0;
			break;
		case 15:
			X15=1;X14=1;X13=1;X12=1;X11=1;X10=1;X9=1;X8=1;X7=1;X6=1;X5=1;X4=1;X3=1;X2=1;X1=1;
			X16=X17=X18=X19=X20=0;
			break;
		case 16:
			X16=1;X15=1;X14=1;X13=1;X12=1;X11=1;X10=1;X9=1;X8=1;X7=1;X6=1;X5=1;X4=1;X3=1;X2=1;X1=1;
			X17=X18=X19=X20=0;
			break;
		case 17:
			X17=1;X16=1;X15=1;X14=1;X13=1;X12=1;X11=1;X10=1;X9=1;X8=1;X7=1;X6=1;X5=1;X4=1;X3=1;X2=1;X1=1;
			X18=X19=X20=0;
			break;
		case 18:
			X18=1;X17=1;X16=1;X15=1;X14=1;X13=1;X12=1;X11=1;X10=1;X9=1;X8=1;X7=1;X6=1;X5=1;X4=1;X3=1;X2=1;X1=1;
			X19=X20=0;
			break;
		case 19:
			X19=1;X18=1;X17=1;X16=1;X15=1;X14=1;X13=1;X12=1;X11=1;X10=1;X9=1;X8=1;X7=1;X6=1;X5=1;X4=1;X3=1;X2=1;X1=1;
			X20=0;
			break;
		case 20:
			X20=1;X19=1;X18=1;X17=1;X16=1;X15=1;X14=1;X13=1;X12=1;X11=1;X10=1;X9=1;X8=1;X7=1;X6=1;X5=1;X4=1;X3=1;X2=1;X1=1;
			break;		
		}	
}

void displayTime(){
	// Display Time in Mode 1 (F1)
	numberBreak(hours);
	if (hours > 9) {
		displayNumber(1,tens);	
		displayNumber(2,ones);
	}
	else{
		displayNumber(1,10);
		displayNumber(2,ones);
	}
	numberBreak(minutes);
	displayNumber(3,tens);
	displayNumber(4,ones);
	tickDisplay(seconds);
}

void displayVoltage(){

	// Configure ADC channel for POT
	CHS0 = 0;	//AN1
	
	delay();
	
	GODONE = 1;
	while (GODONE);	// wait for conversion to complete
	boardVoltage = (ADRESH << 8) + ADRESL;
	boardVoltage = boardVoltage * 30;
	boardVoltage = boardVoltage / 10;
	display4dig(boardVoltage);
	boardVoltage = boardVoltage / 150;
	displayBars(boardVoltage);
}


void display4dig(unsigned int number){
	unsigned char temp;
	temp = number / 1000;
	displayNumber(2, temp);
	number = number - temp * 1000;
	temp = number / 100;

	displayNumber(3, temp);
	number = number - temp * 100;
	temp = number / 10;

	displayNumber(4, temp);
	number = number - temp * 10;
	displayNumber(5, number);
}

void delay(){
	unsigned int i=16000;

	while(i--);

}

void getTemperature(void){
	// Configure ADC channel for POT
	CHS0 = 1;	//AN1
	
	delay();
	delay();

	GODONE = 1;
	while (GODONE);	// wait for conversion to complete
	boardTemp = (ADRESH << 8) + ADRESL;
	boardTemp = 36 * (boardTemp - 303) / 10;
	DP2 = 1;
	displayTemp(boardTemp);

}


void displayTemp(unsigned int number){
	unsigned char temp2;
	temp2 = number / 100;
	number = number - temp2 * 100;
	displayNumber(6, temp2);
	temp2 = number / 10;

	displayNumber(7, temp2);
	number = number - temp2 * 10;
	displayNumber(8, number);
	X26  = 1;
}

void tickDisplay(unsigned char value){
	//value will always be seconds
	unsigned char remainder = value; 
	if (value > 10){
		remainder = value % 10;
	}
	switch(remainder){
		case 0:
				X27 = X28 = X29 = X30 = X31 = X32 = X33 = X34 = X35 = X36 = 1;
			break;
		case 1:
				X35 = 1;
				X27 = X28 = X29 = X30 = X31 = X32 = X33 = X34 = X36 = 0;
			break;
		case 2:
				X35 = X34 = 1;
				X27 = X28 = X29 = X30 = X31 = X32 = X33 = X36 = 0;
			break;
		case 3:
				X35 = X34 = X33 = 1;
				X27 = X28 = X29 = X30 = X31 = X32 = X36 = 0;
			break;
		case 4:
				X35 = X34 = X33 = X32 = 1;
				X27 = X28 = X29 = X30 = X31 = X36 = 0;
			break;
		case 5:
				X35 = X34 = X33 = X32 = X31 = 1;
				X27 = X28 = X29 = X30 = X36 = 0;
			break;
		case 6:
				X35 = X34 = X33 = X32 = X31 = X30 = 1;
				X27 = X28 = X29 = X36 = 0;
			break;
		case 7:
				X35 = X34 = X33 = X32 = X31 = X30 = X29 = 1;
				X27 = X28 = X36 = 0;
			break;
		case 8:
				X35 = X34 = X33 = X32 = X31 = X30 = X29 = X28 = 1;
				X27 = X36 = 0;
			break;
		case 9:
				X35 = X34 = X33 = X32 = X31 = X30 = X29 = X28 = X36 = 1;
			    X27 = 0;
			break;
	}
}
