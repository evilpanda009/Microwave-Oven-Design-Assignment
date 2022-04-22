/***********************************************************************
 *
 * PICDEM LCD2 Demo Program for 8490 family devices.
 *
 ***********************************************************************
 * FileName:            LCD2Demo_8490_R0.C
 * Dependencies:		LCD_8490.h
 * Processor:           PIC18F8490
 * Assembler:
 * Linker:
 * Compiler:			C18
 * Company:             Microchip Technology, Inc.
 *
 * Microchip Technology Incorporated ("Microchip") retains all ownership
 * and intellectual property rights in the code accompanying this message
 * and in all derivatives hereto.  You may use this code, and any
 * derivatives created by any person or entity by or on your behalf,
 * exclusively with Microchip’s proprietary products.  Your acceptance
 * and/or use of this code constitutes agreement to the terms and
 * conditions of this notice.
 *
 * CODE ACCOMPANYING THIS MESSAGE IS SUPPLIED BY MICROCHIP "AS IS".
 * NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING,
 * BUT NOT LIMITED TO, IMPLIED WARRANTIES OF NON-INFRINGEMENT,
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS CODE,
 * ITS INTERACTION WITH MICROCHIP’S PRODUCTS, COMBINATION WITH ANY OTHER
 * PRODUCTS, OR USE IN ANY APPLICATION.
 *
 * YOU ACKNOWLEDGE AND AGREE THAT, IN NO EVENT, SHALL MICROCHIP BE
 * LIABLE, WHETHER IN CONTRACT, WARRANTY, TORT (INCLUDING NEGLIGENCE OR
 * BREACH OF STATUTORY DUTY), STRICT LIABILITY, INDEMNITY, CONTRIBUTION,
 * OR OTHERWISE, FOR ANY INDIRECT, SPECIAL, PUNITIVE, EXEMPLARY, INCIDENTAL
 * OR CONSEQUENTIAL LOSS, DAMAGE, FOR COST OR EXPENSE OF ANY KIND WHATSOEVER
 * RELATED TO THE CODE, HOWSOEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED
 * OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT
 * ALLOWABLE BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY
 * RELATED TO THIS CODE, SHALL NOT EXCEED THE PRICE YOU PAID DIRECTLY TO
 * MICROCHIP SPECIFICALLY TO HAVE THIS CODE DEVELOPED.
 *
 * You agree that you are solely responsible for testing the code and
 * determining its suitability.  Microchip has no obligation to modify,
 * test, certify, or support the code.
 *
 * Refer to the PICDEM LCD 2 Users Guide for additional information
 **********************************************************************/
#include "p18f8490.h"
#include "LCD_8490.h"

//***********************************************************************
// Configuration bits
//***********************************************************************
#pragma config OSC  =INTIO67
#pragma config FCMEN=OFF
#pragma config IESO =OFF
#pragma config PWRT =OFF
#pragma config BOREN=OFF
#pragma config WDT  =OFF
#pragma config DEBUG=ON
#pragma config XINST=OFF

//**********************************************************************
// Structure Definitions
//**********************************************************************
typedef union
 {
  struct
  {
  unsigned b0:1;
  unsigned b1:1;
  unsigned b2:1;
  unsigned b3:1;
  unsigned b4:1;
  unsigned b5:1;
  unsigned b6:1;
  unsigned b7:1;
  }location;
  unsigned char ch;
 }Bytebits;

struct INT_nibble {
				unsigned a: 4;
				unsigned b: 4;
				unsigned c: 4;
				unsigned d: 4;
};

struct S_LONG_nibble {
				unsigned a: 4;
				unsigned b: 4;
				unsigned c: 4;
				unsigned d: 4;
				unsigned e: 4;
				unsigned f: 4;
};

union integer {
				struct INT_nibble Nib;
				unsigned char Char;
				unsigned int  Int;
};

union SL {
				  struct S_LONG_nibble Nib;
				  unsigned short long S_Long;
};
//**********************************************************************
// Function Prototypes
//**********************************************************************
void LCD_CONFIG_R (void);
void DisplayOn (void);
void DisplayOff (void);
void BarGraph (unsigned char);
void CirGraph (unsigned char);
void S1Num (unsigned char);
void S2Num (unsigned char);
void S3Num (unsigned char);
void S4Num (unsigned char);
void S5Num (unsigned char);
void S6Num (unsigned char);
void S7Num (unsigned char);
void S8Num (unsigned char);
void LLine (unsigned int);
void ULine (unsigned short long);

void Voltmeter (void);
void VoltM_Display (unsigned short long i);
void BttryIndic (unsigned char i);

void Thermometer (void);
void Temp_Display_C (unsigned short long i);
void Temp_Display_F (unsigned short long i);
void TempIndic (unsigned char i);

void RTC_Display(void);
void InitialDemo(void);

unsigned short long HEX2BIN (unsigned int);
unsigned short long AdjustBCD (unsigned short long);

void InterruptHandlerHigh (void);

void Delay3 (void);
void Delay_LP (void);
void Delay_LP1 (void);
//**********************************************************************
// Defines
//**********************************************************************
#define A 10
#define C 12
#define E 14
#define F 15
#define H 17
#define L 21
#define P 25



#define F_1 1
#define F_2 2
#define F_3 3
#define F_4 4
#define EnableAll 0xFF
#define No_Disp 255

#define Pressed 0
#define Default 1
#define SW3 	PORTAbits.RA6
#define SW4 	PORTAbits.RA7

//**********************************************************************
// Variable declaration
//**********************************************************************

unsigned char i, cnt, Temp;
unsigned int j;
unsigned short long k;

struct INT_nibble Vn;
union integer In;

struct S_LONG_nibble ST_Ln;
union SL BCD;

unsigned int an0voltage, an2voltage;
unsigned int Reading;

unsigned char BD_VLTG_x10_InHex;

unsigned char SEC, MIN, HR;

unsigned char BGCNT, SEC_FLAG, SEC_FG, TMR3FG, TMP_CNT;

unsigned char DemoCode, S_2, S_1, BiasBits;

unsigned char i0,i1,i2;
Bytebits Port_bits, Temp_bits;
//**********************************************************************
// Main Program
//**********************************************************************

void main (void)
{
    OSCCON  = 0b01000000;  //1 MHZ
    OSCTUNE = 0b01100000;

	BD_VLTG_x10_InHex = 0x32; // 0x21 for 3.3 V  0x33 for 5.1V.
	BGCNT = 0;
	SEC_FLAG = 0;

	LCD_CONFIG_R(); 		// Configure LCD module to use internal Charge Pump.

	// Hardware Set UP

	TRISBbits.TRISB0 = 0; 	// Control Power for RS-232, Thermistor, Voltmeter & EEPROM

	INTCONbits.RBIE = 1;
	INTCONbits.GIE  = 1;
	INTCONbits.PEIE = 1;

	LATBbits.LATB0 = 0;   	// Power OFF for AUX.
	DisplayOn();
	Delay3();
	DisplayOff();
	MCHPON; VaritronixON; 	// Display MCHP & Varitronix Symbol.

	Sleep(); 				// Low Power Mode.

	S_1 = Default;
	S_2 = Default;
	while (S_2 == Default)
	{
	   S1Num(H);S2Num(E);S3Num(L);S4Num(L);S5Num(0);
 	   DemoCode = F_1;
	}

	DisplayOff();

	//Timer1 Initialization
	TMR1H = 0x80;
	T1CONbits.T1RUN = 1;
	T1CONbits.T1CKPS0 = 0;
	T1CONbits.T1OSCEN = 1;
	T1CONbits.T1SYNC = 1;
	T1CONbits.TMR1CS = 1;
	T1CONbits.TMR1ON = 1;
	PIE1bits.TMR1IE = 0;

	while(1)
	{

		while (DemoCode == F_1)
		{
			ADCON1 = 0x0D;			//RA0-RA2 Analog inputs.
			S_1 = Default;
			S_2 = Default;
		 	F1ON; MCHPON; VaritronixON;
			LATBbits.LATB0 = 1;   	// Power ON, for jumper on I/O & RB0.
			TRISAbits.TRISA0 = 1; 	// AN0
			ADCON0bits.CHS3 = 0;  	// channel 0
			ADCON0bits.CHS2 = 0;
			ADCON0bits.CHS1 = 0;
			ADCON0bits.CHS0 = 0;
			ADCON2 = 0xBF;		  	// right justify, A/D RC Osc
			ADCON0bits.ADON = 1;  	// ADC on


			while (S_2 == Default)
			{
				PIR1bits.ADIF = 0;
				PIE1bits.ADIE = 1;
				ADCON0bits.GO_DONE = 1;
				Sleep();
				Reading = an0voltage;
				Voltmeter();
			}

			DemoCode = F_2;
			DisplayOff();
			ADCON0bits.ADON = 0;  	// ADC Off
		}
		while (DemoCode == F_2)
		{
			S_1 = Default;
			S_2 = Default;
			F2ON; MCHPON; VaritronixON;
			TRISAbits.TRISA1 = 1; 	//AN2
			ADCON0bits.CHS3 = 0;  	// channel 2
			ADCON0bits.CHS2 = 0;
			ADCON0bits.CHS1 = 0;
			ADCON0bits.CHS0 = 1;
			ADCON2 = 0xBF;			// right justify, A/D RC Osc
			ADCON0bits.ADON = 1;	// ADC on
			cnt = 4; //n samples
			SEC_FG = 1;
			TMP_CNT = 2;

			while (S_2 == Default)
			{
				Reading = 0;
				for ( i=0; i<cnt; i++)
				{
					PIR1bits.ADIF = 0;
					PIE1bits.ADIE = 1;
					ADCON0bits.GO_DONE = 1;
					Sleep();
					Reading += an2voltage;
				}
				Reading /= cnt;
				if (SEC_FG == 1) { SEC_FG = 0;TMP_CNT++;}
				if (TMP_CNT == 3)
				{
					TMP_CNT = 0;
					Thermometer();
				}
			}
			DemoCode = F_3;
			DisplayOff();
			ADCON0bits.ADON = 0;  // ADC Off
			LATBbits.LATB0 = 0;   // Power Off for AUX

		}
		while (DemoCode == F_3)
		{
			S_1 = Default;
			S_2 = Default;

			F3ON; MCHPON; VaritronixON;
			PIE1bits.TMR1IE = 1;
			MIN = 0; HR = 0; SEC = 0;

			while (S_2 == Default)
			{
				RTC_Display();
				Sleep();
				if (SW4 == Pressed)
				{
				  while(SW4 == Pressed)
				  {
					  Delay_LP1();	//Delay for Key press
					  if (SW3 == Pressed) MIN++;
					  else if (SW4 == Pressed) HR++;
					  RTC_Display();
				   }
				}
			}
			DemoCode = F_1;
			DisplayOff();
			PIE1bits.TMR1IE = 0;
		}
	}

}//END Main

//**********************************************************************
// End of Main Program
//**********************************************************************
//----------------------------------------------------------------------------
// High priority interrupt vector

#pragma code InterruptVectorHigh = 0x08
void
InterruptVectorHigh (void)
{
  _asm
    goto InterruptHandlerHigh //jump to interrupt routine
  _endasm
}

//----------------------------------------------------------------------------
// High priority interrupt routine

#pragma code
#pragma interrupt InterruptHandlerHigh

void
InterruptHandlerHigh ()
{

    if (PIR1bits.TMR1IF == 1)
    {
		PIR1bits.TMR1IF = 0;
		SEC++;
		if (SEC > 59){ SEC = 0; MIN++;}
		if (MIN > 59){ MIN = 0; HR++;}
		if (HR > 23 ) { HR = 0;}

		TMR1H = 0x80;
    	SEC_FLAG = ~SEC_FLAG;
		SEC_FG = 1;
	}

	if (PIR1bits.ADIF == 1)
	{
		PIR1bits.ADIF = 0;
		Temp = ADCON0 & 0x3C;
		if ( Temp == 0){an0voltage = ((unsigned int)(ADRESH) << 8) + ADRESL;}
		if ( Temp == 4){an2voltage = ((unsigned int)(ADRESH) << 8) + ADRESL;}
	}

	if (INTCONbits.RBIF == 1)
	{
		INTCONbits.RBIF = 0;
		Port_bits.ch = PORTB;
		if (Port_bits.location.b7 == 1){S_2 = Pressed;}
		if (Port_bits.location.b6 == 1){S_1 = Pressed;}
	}

	if (PIR2bits.TMR3IF == 1)
    {
		PIR2bits.TMR3IF = 0;
		TMR3FG = 1;
    }

}
//**********************************************************************
void Thermometer (void)
{
	if (SEC_FLAG == 0)
	{
	j= 2*(Reading - 392);
	k= HEX2BIN (j); //to display in C
	Temp_Display_C (k);
	}
	else
	{
	j = 36*(Reading - 303);
	k = HEX2BIN (j); //to display in F
	Temp_Display_F (k);
	}
}
//**********************************************************************
void Temp_Display_C(unsigned short long i)
{
  BCD.S_Long = i;
  S5Num (No_Disp);
  S6Num (BCD.Nib.c);
  S7Num (BCD.Nib.b);
  S8Num (BCD.Nib.a);
  DP2ON;X25ON;X26OFF;
  TempIndic (BCD.Nib.c);
}
//**********************************************************************
void Temp_Display_F(unsigned short long i)
{
  BCD.S_Long = i;
  if (BCD.Nib.e == 0) S5Num(No_Disp);
  else S5Num (BCD.Nib.e);
  S6Num (BCD.Nib.d);
  S7Num (BCD.Nib.c);
  S8Num (BCD.Nib.b);
  DP2ON;X26ON;X25OFF;
}
//**********************************************************************
void Voltmeter (void)
{
	j= an0voltage * BD_VLTG_x10_InHex;
	k= HEX2BIN (j);
	VoltM_Display (k);
	BarGraph(an0voltage/50);
}
//**********************************************************************
void VoltM_Display (unsigned short long i)
{
  BCD.S_Long = i;
  S4Num (BCD.Nib.e);
  S5Num (BCD.Nib.d);
  //S6Num (BCD.Nib.c);  //Optional for LSBs
  //S7Num (BCD.Nib.b);
  //S8Num (BCD.Nib.a);
  DP1ON;
  //BttryIndic (BCD.Nib.e);
  //Unused
}
//**********************************************************************
unsigned short long HEX2BIN (unsigned int Binary)
{
unsigned short long Result ;

Result = 0;

for (i=0; i<16; i++)
 {
   Result = Result << 1;
   if (Binary >= 32768) Result++;
   if (i < 15){ Result = AdjustBCD(Result);}
   Binary = Binary << 1;
 }

return Result ;
}
//**********************************************************************
unsigned short long AdjustBCD (unsigned short long R)
{
unsigned char R0,R1,R2;

BCD.S_Long = R;

if ((BCD.Nib.a + 3) > 7) BCD.Nib.a += 3;
if ((BCD.Nib.b + 3) > 7) BCD.Nib.b += 3;
if ((BCD.Nib.c + 3) > 7) BCD.Nib.c += 3;
if ((BCD.Nib.d + 3) > 7) BCD.Nib.d += 3;
if ((BCD.Nib.e + 3) > 7) BCD.Nib.e += 3;
if ((BCD.Nib.f + 3) > 7) BCD.Nib.f += 3;

R = BCD.S_Long;

return R;
}
//**********************************************************************
void LLine (unsigned int i)
{
  In.Int = i;
  S6Num (In.Nib.c);
  S7Num (In.Nib.b);
  S8Num (In.Nib.a);
}
//**********************************************************************
void ULine (unsigned short long i)
{
  BCD.S_Long = i;
  if (i < 0x10000) S1Num(No_Disp);
  else S1Num (BCD.Nib.e);
  if (i < 0x1000) S2Num(No_Disp);
  else S2Num (BCD.Nib.d);
  if (i < 0x100) S3Num(No_Disp);
  else S3Num (BCD.Nib.c);
  S4Num (BCD.Nib.b);
  S5Num (BCD.Nib.a);
}
//**********************************************************************
void TempIndic (unsigned char i)
{
  S5ON;
  if (i >= 5)
	{ S6ON; S7ON; S8ON;}
  else if (i >= 2)
	{ S6OFF; S7ON; S8ON;}
  else if (i >= 1)
	{ S6OFF; S7OFF; S8ON;}
}
//**********************************************************************
void BttryIndic (unsigned char i)
{
  S9ON;
  if (i >= 3)
	{ S10ON; S12ON; S11ON;}
  if (i == 2)
	{ S10OFF; S12ON; S11ON;}
  if (i == 1)
	{ S10OFF; S12OFF; S11ON;}
}


//**********************************************************************
void RTC_Display(void)
{
	BCD.S_Long = HEX2BIN (SEC);
	S6Num (BCD.Nib.b);
	S7Num (BCD.Nib.a);
	CirGraph(BCD.Nib.a);
	BCD.S_Long = HEX2BIN (MIN);
	S3Num (BCD.Nib.b);
	S4Num (BCD.Nib.a);
	BCD.S_Long = HEX2BIN (HR);
	S1Num (BCD.Nib.b);
	S2Num (BCD.Nib.a);
	COLON;
}
//**********************************************************************
void S1Num (unsigned char num)
{
	switch (num)
    {
		case 0: S1AON; S1BON; S1CON; S1DON; S1EON; S1FON; S1GOFF;break;
		case 1: S1AOFF;S1BON; S1CON; S1DOFF;S1EOFF;S1FOFF;S1GOFF;break;
		case 2: S1AON; S1BON; S1COFF;S1DON; S1EON; S1FOFF;S1GON; break;
		case 3: S1AON; S1BON; S1CON; S1DON;	S1EOFF;S1FOFF;S1GON; break;
		case 4: S1AOFF;S1BON; S1CON; S1DOFF;S1EOFF;S1FON; S1GON; break;
		case 5: S1AON; S1BOFF;S1CON; S1DON; S1EOFF;S1FON; S1GON; break;
		case 6: S1AON; S1BOFF;S1CON; S1DON; S1EON; S1FON; S1GON; break;
		case 7: S1AON; S1BON; S1CON; S1DOFF;S1EOFF;S1FOFF;S1GOFF;break;
		case 8: S1AON; S1BON; S1CON; S1DON; S1EON; S1FON; S1GON; break;
		case 9: S1AON; S1BON; S1CON; S1DON; S1EOFF;S1FON; S1GON; break;
		case A: S1AON; S1BON; S1CON; S1DOFF;S1EON; S1FON; S1GON; break;
		case C: S1AON; S1BOFF;S1COFF;S1DON; S1EON; S1FON; S1GOFF;break;
		case E: S1AON; S1BOFF;S1COFF;S1DON; S1EON; S1FON; S1GON; break;
		case F: S1AON; S1BOFF;S1COFF;S1DOFF;S1EON; S1FON; S1GON; break;
		case H: S1AOFF;S1BON; S1CON; S1DOFF;S1EON; S1FON; S1GON; break;
		case L: S1AOFF;S1BOFF;S1COFF;S1DON; S1EON; S1FON; S1GOFF;break;
		case P: S1AON; S1BON; S1COFF;S1DOFF;S1EON; S1FON; S1GON; break;

		case No_Disp: S1NumOFF;
    }
}
//**********************************************************************
void S2Num (unsigned char num)
{
	switch (num)
    {
		case 0: S2AON; S2BON; S2CON; S2DON; S2EON; S2FON; S2GOFF;break;
		case 1: S2AOFF;S2BON; S2CON; S2DOFF;S2EOFF;S2FOFF;S2GOFF;break;
		case 2: S2AON; S2BON; S2COFF;S2DON; S2EON; S2FOFF;S2GON; break;
		case 3: S2AON; S2BON; S2CON; S2DON;	S2EOFF;S2FOFF;S2GON; break;
		case 4: S2AOFF;S2BON; S2CON; S2DOFF;S2EOFF;S2FON; S2GON; break;
		case 5: S2AON; S2BOFF;S2CON; S2DON; S2EOFF;S2FON; S2GON; break;
		case 6: S2AON; S2BOFF;S2CON; S2DON; S2EON; S2FON; S2GON; break;
		case 7: S2AON; S2BON; S2CON; S2DOFF;S2EOFF;S2FOFF;S2GOFF;break;
		case 8: S2AON; S2BON; S2CON; S2DON; S2EON; S2FON; S2GON; break;
		case 9: S2AON; S2BON; S2CON; S2DON; S2EOFF;S2FON; S2GON; break;
		case A: S2AON; S2BON; S2CON; S2DOFF;S2EON; S2FON; S2GON; break;
		case C: S2AON; S2BOFF;S2COFF;S2DON; S2EON; S2FON; S2GOFF;break;
		case E: S2AON; S2BOFF;S2COFF;S2DON; S2EON; S2FON; S2GON; break;
		case F: S2AON; S2BOFF;S2COFF;S2DOFF;S2EON; S2FON; S2GON; break;
		case H: S2AOFF;S2BON; S2CON; S2DOFF;S2EON; S2FON; S2GON; break;
		case L: S2AOFF;S2BOFF;S2COFF;S2DON; S2EON; S2FON; S2GOFF;break;
		case P: S2AON; S2BON; S2COFF;S2DOFF;S2EON; S2FON; S2GON; break;

		case No_Disp: S2NumOFF;
    }
}
//**********************************************************************
void S3Num (unsigned char num)
{
	switch (num)
    {
		case 0: S3AON; S3BON; S3CON; S3DON; S3EON; S3FON; S3GOFF;break;
		case 1: S3AOFF;S3BON; S3CON; S3DOFF;S3EOFF;S3FOFF;S3GOFF;break;
		case 2: S3AON; S3BON; S3COFF;S3DON; S3EON; S3FOFF;S3GON; break;
		case 3: S3AON; S3BON; S3CON; S3DON;	S3EOFF;S3FOFF;S3GON; break;
		case 4: S3AOFF;S3BON; S3CON; S3DOFF;S3EOFF;S3FON; S3GON; break;
		case 5: S3AON; S3BOFF;S3CON; S3DON; S3EOFF;S3FON; S3GON; break;
		case 6: S3AON; S3BOFF;S3CON; S3DON; S3EON; S3FON; S3GON; break;
		case 7: S3AON; S3BON; S3CON; S3DOFF;S3EOFF;S3FOFF;S3GOFF;break;
		case 8: S3AON; S3BON; S3CON; S3DON; S3EON; S3FON; S3GON; break;
		case 9: S3AON; S3BON; S3CON; S3DON; S3EOFF;S3FON; S3GON; break;
		case A: S3AON; S3BON; S3CON; S3DOFF;S3EON; S3FON; S3GON; break;
		case C: S3AON; S3BOFF;S3COFF;S3DON; S3EON; S3FON; S3GOFF;break;
		case E: S3AON; S3BOFF;S3COFF;S3DON; S3EON; S3FON; S3GON; break;
		case F: S3AON; S3BOFF;S3COFF;S3DOFF;S3EON; S3FON; S3GON; break;
		case H: S3AOFF;S3BON; S3CON; S3DOFF;S3EON; S3FON; S3GON; break;
		case L: S3AOFF;S3BOFF;S3COFF;S3DON; S3EON; S3FON; S3GOFF;break;
		case P: S3AON; S3BON; S3COFF;S3DOFF;S3EON; S3FON; S3GON; break;

		case No_Disp: S3NumOFF;
    }
}
//**********************************************************************
void S4Num (unsigned char num)
{
	switch (num)
    {
		case 0: S4AON; S4BON; S4CON; S4DON; S4EON; S4FON; S4GOFF;break;
		case 1: S4AOFF;S4BON; S4CON; S4DOFF;S4EOFF;S4FOFF;S4GOFF;break;
		case 2: S4AON; S4BON; S4COFF;S4DON; S4EON; S4FOFF;S4GON; break;
		case 3: S4AON; S4BON; S4CON; S4DON;	S4EOFF;S4FOFF;S4GON; break;
		case 4: S4AOFF;S4BON; S4CON; S4DOFF;S4EOFF;S4FON; S4GON; break;
		case 5: S4AON; S4BOFF;S4CON; S4DON; S4EOFF;S4FON; S4GON; break;
		case 6: S4AON; S4BOFF;S4CON; S4DON; S4EON; S4FON; S4GON; break;
		case 7: S4AON; S4BON; S4CON; S4DOFF;S4EOFF;S4FOFF;S4GOFF;break;
		case 8: S4AON; S4BON; S4CON; S4DON; S4EON; S4FON; S4GON; break;
		case 9: S4AON; S4BON; S4CON; S4DON; S4EOFF;S4FON; S4GON; break;
		case A: S4AON; S4BON; S4CON; S4DOFF;S4EON; S4FON; S4GON; break;
		case C: S4AON; S4BOFF;S4COFF;S4DON; S4EON; S4FON; S4GOFF;break;
		case E: S4AON; S4BOFF;S4COFF;S4DON; S4EON; S4FON; S4GON; break;
		case F: S4AON; S4BOFF;S4COFF;S4DOFF;S4EON; S4FON; S4GON; break;
		case H: S4AOFF;S4BON; S4CON; S4DOFF;S4EON; S4FON; S4GON; break;
		case L: S4AOFF;S4BOFF;S4COFF;S4DON; S4EON; S4FON; S4GOFF;break;
		case P: S4AON; S4BON; S4COFF;S4DOFF;S4EON; S4FON; S4GON; break;

		case No_Disp: S4NumOFF;
    }
}
//**********************************************************************
void S5Num (unsigned char num)
{
	switch (num)
    {
		case 0: S5AON; S5BON; S5CON; S5DON; S5EON; S5FON; S5GOFF;break;
		case 1: S5AOFF;S5BON; S5CON; S5DOFF;S5EOFF;S5FOFF;S5GOFF;break;
		case 2: S5AON; S5BON; S5COFF;S5DON; S5EON; S5FOFF;S5GON; break;
		case 3: S5AON; S5BON; S5CON; S5DON;	S5EOFF;S5FOFF;S5GON; break;
		case 4: S5AOFF;S5BON; S5CON; S5DOFF;S5EOFF;S5FON; S5GON; break;
		case 5: S5AON; S5BOFF;S5CON; S5DON; S5EOFF;S5FON; S5GON; break;
		case 6: S5AON; S5BOFF;S5CON; S5DON; S5EON; S5FON; S5GON; break;
		case 7: S5AON; S5BON; S5CON; S5DOFF;S5EOFF;S5FOFF;S5GOFF;break;
		case 8: S5AON; S5BON; S5CON; S5DON; S5EON; S5FON; S5GON; break;
		case 9: S5AON; S5BON; S5CON; S5DON; S5EOFF;S5FON; S5GON; break;
		case A: S5AON; S5BON; S5CON; S5DOFF;S5EON; S5FON; S5GON; break;
		case C: S5AON; S5BOFF;S5COFF;S5DON; S5EON; S5FON; S5GOFF;break;
		case E: S5AON; S5BOFF;S5COFF;S5DON; S5EON; S5FON; S5GON; break;
		case F: S5AON; S5BOFF;S5COFF;S5DOFF;S5EON; S5FON; S5GON; break;
		case H: S5AOFF;S5BON; S5CON; S5DOFF;S5EON; S5FON; S5GON; break;
		case L: S5AOFF;S5BOFF;S5COFF;S5DON; S5EON; S5FON; S5GOFF;break;
		case P: S5AON; S5BON; S5COFF;S5DOFF;S5EON; S5FON; S5GON; break;

		case No_Disp: S5NumOFF;
    }
}
//**********************************************************************
void S6Num (unsigned char num)
{
	switch (num)
    {
		case 0: S6AON; S6BON; S6CON; S6DON; S6EON; S6FON; S6GOFF;break;
		case 1: S6AOFF;S6BON; S6CON; S6DOFF;S6EOFF;S6FOFF;S6GOFF;break;
		case 2: S6AON; S6BON; S6COFF;S6DON; S6EON; S6FOFF;S6GON; break;
		case 3: S6AON; S6BON; S6CON; S6DON;	S6EOFF;S6FOFF;S6GON; break;
		case 4: S6AOFF;S6BON; S6CON; S6DOFF;S6EOFF;S6FON; S6GON; break;
		case 5: S6AON; S6BOFF;S6CON; S6DON; S6EOFF;S6FON; S6GON; break;
		case 6: S6AON; S6BOFF;S6CON; S6DON; S6EON; S6FON; S6GON; break;
		case 7: S6AON; S6BON; S6CON; S6DOFF;S6EOFF;S6FOFF;S6GOFF;break;
		case 8: S6AON; S6BON; S6CON; S6DON; S6EON; S6FON; S6GON; break;
		case 9: S6AON; S6BON; S6CON; S6DON; S6EOFF;S6FON; S6GON; break;
		case A: S6AON; S6BON; S6CON; S6DOFF;S6EON; S6FON; S6GON; break;
		case C: S6AON; S6BOFF;S6COFF;S6DON; S6EON; S6FON; S6GOFF;break;
		case E: S6AON; S6BOFF;S6COFF;S6DON; S6EON; S6FON; S6GON; break;
		case F: S6AON; S6BOFF;S6COFF;S6DOFF;S6EON; S6FON; S6GON; break;
		case H: S6AOFF;S6BON; S6CON; S6DOFF;S6EON; S6FON; S6GON; break;
		case L: S6AOFF;S6BOFF;S6COFF;S6DON; S6EON; S6FON; S6GOFF;break;
		case P: S6AON; S6BON; S6COFF;S6DOFF;S6EON; S6FON; S6GON; break;

		case No_Disp: S6NumOFF;
    }
}
//**********************************************************************
void S7Num (unsigned char num)
{
	switch (num)
    {
		case 0: S7AON; S7BON; S7CON; S7DON; S7EON; S7FON; S7GOFF;break;
		case 1: S7AOFF;S7BON; S7CON; S7DOFF;S7EOFF;S7FOFF;S7GOFF;break;
		case 2: S7AON; S7BON; S7COFF;S7DON; S7EON; S7FOFF;S7GON; break;
		case 3: S7AON; S7BON; S7CON; S7DON;	S7EOFF;S7FOFF;S7GON; break;
		case 4: S7AOFF;S7BON; S7CON; S7DOFF;S7EOFF;S7FON; S7GON; break;
		case 5: S7AON; S7BOFF;S7CON; S7DON; S7EOFF;S7FON; S7GON; break;
		case 6: S7AON; S7BOFF;S7CON; S7DON; S7EON; S7FON; S7GON; break;
		case 7: S7AON; S7BON; S7CON; S7DOFF;S7EOFF;S7FOFF;S7GOFF;break;
		case 8: S7AON; S7BON; S7CON; S7DON; S7EON; S7FON; S7GON; break;
		case 9: S7AON; S7BON; S7CON; S7DON; S7EOFF;S7FON; S7GON; break;
		case A: S7AON; S7BON; S7CON; S7DOFF;S7EON; S7FON; S7GON; break;
		case C: S7AON; S7BOFF;S7COFF;S7DON; S7EON; S7FON; S7GOFF;break;
		case E: S7AON; S7BOFF;S7COFF;S7DON; S7EON; S7FON; S7GON; break;
		case F: S7AON; S7BOFF;S7COFF;S7DOFF;S7EON; S7FON; S7GON; break;
		case H: S7AOFF;S7BON; S7CON; S7DOFF;S7EON; S7FON; S7GON; break;
		case L: S7AOFF;S7BOFF;S7COFF;S7DON; S7EON; S7FON; S7GOFF;break;
		case P: S7AON; S7BON; S7COFF;S7DOFF;S7EON; S7FON; S7GON; break;

		case No_Disp: S7NumOFF;
    }
}
//**********************************************************************
void S8Num (unsigned char num)
{
	switch (num)
    {
		case 0: S8AON; S8BON; S8CON; S8DON; S8EON; S8FON; S8GOFF;break;
		case 1: S8AOFF;S8BON; S8CON; S8DOFF;S8EOFF;S8FOFF;S8GOFF;break;
		case 2: S8AON; S8BON; S8COFF;S8DON; S8EON; S8FOFF;S8GON; break;
		case 3: S8AON; S8BON; S8CON; S8DON;	S8EOFF;S8FOFF;S8GON; break;
		case 4: S8AOFF;S8BON; S8CON; S8DOFF;S8EOFF;S8FON; S8GON; break;
		case 5: S8AON; S8BOFF;S8CON; S8DON; S8EOFF;S8FON; S8GON; break;
		case 6: S8AON; S8BOFF;S8CON; S8DON; S8EON; S8FON; S8GON; break;
		case 7: S8AON; S8BON; S8CON; S8DOFF;S8EOFF;S8FOFF;S8GOFF;break;
		case 8: S8AON; S8BON; S8CON; S8DON; S8EON; S8FON; S8GON; break;
		case 9: S8AON; S8BON; S8CON; S8DON; S8EOFF;S8FON; S8GON; break;
		case A: S8AON; S8BON; S8CON; S8DOFF;S8EON; S8FON; S8GON; break;
		case C: S8AON; S8BOFF;S8COFF;S8DON; S8EON; S8FON; S8GOFF;break;
		case E: S8AON; S8BOFF;S8COFF;S8DON; S8EON; S8FON; S8GON; break;
		case F: S8AON; S8BOFF;S8COFF;S8DOFF;S8EON; S8FON; S8GON; break;
		case H: S8AOFF;S8BON; S8CON; S8DOFF;S8EON; S8FON; S8GON; break;
		case L: S8AOFF;S8BOFF;S8COFF;S8DON; S8EON; S8FON; S8GOFF;break;
		case P: S8AON; S8BON; S8COFF;S8DOFF;S8EON; S8FON; S8GON; break;

		case No_Disp: S8NumOFF;
    }
}
//**********************************************************************
void BarGraph (unsigned char Count)
{
	if (BGCNT > Count)
	{
		switch (Count)
		{
			case 1: X2OFF;
			case 2: X3OFF;
			case 3: X4OFF;
			case 4: X5OFF;
			case 5: X6OFF;
			case 6: X7OFF;
			case 7: X8OFF;
			case 8: X9OFF;
			case 9: X10OFF;
			case 10: X11OFF;
			case 11: X12OFF;
			case 12: X13OFF;
			case 13: X14OFF;
			case 14: X15OFF;
			case 15: X16OFF;
			case 16: X17OFF;
			case 17: X18OFF;
			case 18: X19OFF;
			case 19: X20OFF;
		}
	}

if (BGCNT <= Count)
  {
	switch (Count)
	{
		case 20: X20ON;
		case 19: X19ON;
		case 18: X18ON;
		case 17: X17ON;
		case 16: X16ON;
		case 15: X15ON;
		case 14: X14ON;
		case 13: X13ON;
		case 12: X12ON;
		case 11: X11ON;
		case 10: X10ON;
		case  9: X9ON;
		case  8: X8ON;
		case  7: X7ON;
		case  6: X6ON;
		case  5: X5ON;
		case  4: X4ON;
		case  3: X3ON;
		case  2: X2ON;
		case  1: X1ON;
	 }
   }
BGCNT = Count;
}
//**********************************************************************
void CirGraph (unsigned char Count)
{
	CGOFF;
	switch (Count)
	{
		case 9: X28ON;
		case 8: X29ON;
		case 7: X30ON;
		case 6: X31ON;
		case 5: X32ON;
		case 4: X33ON;
		case 3: X34ON;
		case 2: X35ON;
		case 1: X36ON;
		case 0: X27ON;
	}

}
//**********************************************************************
void LCD_CONFIG_R (void)
{
	// Select the frame clock prescaler using bits LP3:LP0
	 LCDPSbits.WFT     = 0;   //0 - TypeA
	 LCDPSbits.BIASMD  = 0;
	 LCDPSbits.LP3 = 0;
	 LCDPSbits.LP2 = 1;
	 LCDPSbits.LP1 = 0;
	 LCDPSbits.LP0 = 0;

	// Configure the appropriate pins to function as segment drivers
	// using the LCDSEx registers
	 LCDSE0 = 0xFF;
	 LCDSE1 = 0xFF;
	 LCDSE2 = 0xFF;
	 LCDSE3 = 0xFF;
	 LCDSE4 = 0xFF;
	 LCDSE5 = 0xFF;

	// Configure the LCD module for the following using the LCDCON reg
	 LCDCONbits.CS1   = 1; // 00 Sys Clock, 01 TMR1 Clk
	 LCDCONbits.CS0   = 1; // 1x INTRC
	 LCDCONbits.LMUX1 = 1;
	 LCDCONbits.LMUX0 = 1;

	// Enable the LCD module by setting bit LCDEN
	 LCDCONbits.LCDEN = 1;

}
//**********************************************************************
void DisplayOn (void)
{
	S1ON; S2ON; S3ON; S4ON; S5ON; S6ON; S7ON; S8ON; S9ON; S10ON;
	S11ON; S12ON; S13ON; X26ON; X25ON; X24ON; X23ON; X22ON; X21ON;
	X20ON; X19ON; X18ON; X17ON; X16ON; X15ON; X14ON; X13ON;	X12ON;
	X11ON; X10ON; X9ON; X8ON; X7ON; X6ON; X5ON; X4ON; X3ON; X2ON;
	X1ON; F1ON; F2ON; F3ON; F4ON; X27ON; X28ON; X29ON; X30ON; X31ON;
	X32ON; X33ON; X34ON; X35ON; X36ON; S1AON; S1BON; S1CON; S1DON;
	S1EON; S1FON; S1GON; S2AON; S2BON; S2CON; S2DON; S2EON; S2FON;
	S2GON; COLON; S3AON; S3BON; S3CON; S3DON; S3EON; S3FON; S3GON;
	S4AON; S4BON; S4CON; S4DON; S4EON; S4FON; S4GON; DP1ON; S5AON;
	S5BON; S5CON; S5DON; S5EON; S5FON; S5GON; S6AON; S6BON; S6CON;
	S6DON; S6EON; S6FON; S6GON; S7AON; S7BON; S7CON; S7DON; S7EON;
	S7FON; S7GON; DP2ON; S8AON; S8BON; S8CON; S8DON; S8EON; S8FON;
	S8GON;

}
//**********************************************************************
void DisplayOff (void)
{
	S1OFF; S13OFF;
	S2OFF; S3OFF; S4OFF; S5OFF; S6OFF; S7OFF; S8OFF; S9OFF; S10OFF;
	S11OFF; S12OFF; X26OFF; X25OFF; X24OFF; X23OFF; X22OFF; X21OFF;
	X20OFF; X19OFF; X18OFF; X17OFF; X16OFF; X15OFF; X14OFF; X13OFF;
	X12OFF; X11OFF; X10OFF; X9OFF; X8OFF; X7OFF; X6OFF; X5OFF; X4OFF;
	X3OFF; X2OFF; X1OFF; F1OFF; F2OFF; F3OFF; F4OFF; X27OFF; X28OFF;
	X29OFF; X30OFF; X31OFF; X32OFF; X33OFF; X34OFF; X35OFF; X36OFF;
	S1AOFF;	S1BOFF;	S1COFF;	S1DOFF;	S1EOFF;	S1FOFF;	S1GOFF;	S2AOFF;
	S2BOFF;	S2COFF;	S2DOFF;	S2EOFF;	S2FOFF;	S2GOFF;	COLOFF;	S3AOFF;
	S3BOFF;	S3COFF;	S3DOFF;	S3EOFF;	S3FOFF;	S3GOFF;	S4AOFF;	S4BOFF;
	S4COFF;	S4DOFF;	S4EOFF;	S4FOFF;	S4GOFF;	DP1OFF;	S5AOFF;	S5BOFF;
	S5COFF;	S5DOFF;	S5EOFF;	S5FOFF;	S5GOFF;	S6AOFF;	S6BOFF;	S6COFF;
	S6DOFF;	S6EOFF;	S6FOFF;	S6GOFF;	S7AOFF;	S7BOFF;	S7COFF;	S7DOFF;
	S7EOFF;	S7FOFF;	S7GOFF;	DP2OFF;	S8AOFF;	S8BOFF;	S8COFF;	S8DOFF;
	S8EOFF;	S8FOFF;	S8GOFF;

}
//**********************************************************************

void Delay3 (void)
{
  unsigned int temp1, temp2;

 for (temp2=0;temp2<15000;temp2++);
}

void Delay_LP (void)
{
	TMR3H = 0xE8;
	TMR3L = 0xFF;
	T3CONbits.TMR3CS = 1; //Ext Timer1 Osc
	T3CONbits.T3SYNC = 1;
	T3CONbits.TMR3ON = 1;
	PIR2bits.TMR3IF = 0;
	PIE2bits.TMR3IE = 1;  //Enable Interrupt
	Sleep();
	PIE2bits.TMR3IE = 0;
	T3CONbits.TMR3ON = 0;
}

void Delay_LP1 (void)
{
	TMR3H = 0xE0;
	TMR3L = 0xFF;
	T3CONbits.TMR3CS = 1; //Ext Timer1 Osc
	T3CONbits.T3SYNC = 1;
	T3CONbits.TMR3ON = 1;
	PIR2bits.TMR3IF = 0;
	PIE2bits.TMR3IE = 1;  //Enable Interrupt
	Sleep();
	PIE2bits.TMR3IE = 0;
	T3CONbits.TMR3ON = 0;
}