/*********************************************************************
* Software License Agreement:
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
*********************************************************************/

#include <pic.h>
#include <stdlib.h>
#include <time.h>

#define FOSC 16000000UL

#include "GenericTypeDefs.h"
#include "lcd.h"
#include "rtcc.h"
#include "input.h"
#include "i2c.h"
#include "mcp9800.h"
#include "mchp_support.h"

__CONFIG(0x3FE4);
__CONFIG(0x1FFF);

void display_time(void);
void display_temp(int t);
void display_int(int t);
void display_rpm(int r);
void display_pot(int p);

volatile char blink;

typedef enum { MODE_TIME, MODE_TEMPERATURE, MODE_POT} mode_t;

int main()
{
    int tmp;
    event_t btn_event;
    time_t btn_press_time;
    char time_set;
    time_t t;
    mode_t display_mode;
        
    T0CS = 0;
    PSA = 1;
    
    blink = 0;
    
    OSCCONbits.IRCF = 15;
        
    // peripheral configuration
    PORTC = 0;
    PORTD = 0;
    PORTE = 0;
    TRISC2 = 0;
    TRISE0 = 0;
    TRISE1 = 0;
    TRISE2 = 0;
    TRISD5 = 0;
    TRISD6 = 0;
    
    ANSA1 = 1;
    ANSA3 = 1;
    ANSB3 = 1;
    ANSB1 = 1;
    
    rtcc_init();
    lcd_init();
    input_init();
        
    i2c_init();
    
    PEIE = 1;
    GIE = 1;

    // external HW configuration
    mcp9800_init();
        
        // light all segments
        AMPS = 1;
        VOLT = 1;
        KILO = 1;
        OHMS = 1;
        S1 = 1;
        S2 = 1;
        MILLI = 1;
        MEGA = 1;
        B1 = G1 = C1 = 1;
        A1 = F1 = E1 = D1 = 1;
        B2 = G2 = C2 = DP2 = 1;
        D2 = E2 = F2 = A2 = 1;
        DP3 = C3 = G3 = B3 = 1;
        D3 = E3 = F3 = A3 = 1;
        DP4 = BC4 = RH = DH = 1;
        AC = MINUS = BATT = RC = 1;
/*        
	while(1)
	{
    	
    }
*/
    while(1)
     {   	
    	TMR0 = 0;
    	TMR0IF = 0;
    	
    	btn_event = input_event();

    	switch(btn_event)
    	{
        	case BUTTON_UP: break;
        	case BUTTON_DOWN:
        	    if(display_mode == MODE_TIME && ((time(0) - btn_press_time) > 1))
        	    {
            	    time_set = 1;
            	    // watch the pot and adjust the time.
            	    tmp = input_pot();
            	    t = time(0);
            	    if(tmp > (1023 - (1023/4))) // increment the time if the pot is > 75%;
            	    {
                	    t += 1;
                	    if(t > 0)
                	    {
                    	    rtcc_set(t+1);
                    	}    
                	}
                	else if(tmp < 1023/4) // decrement the time if the pot is < 25%
                	{
                    	t -= 1;
                	    if(t >= 0)
                	    {
                    	    rtcc_set(t);
                    	}                        	    
                    }
                    else // deadband in the center of the pot
                    {
                    }
            	}    
        	    break;
        	case BUTTON_PRESSED:
        	    btn_press_time = time(0);
        	    break;
        	case BUTTON_RELEASED: // on release, next display mode (unless we were setting the time)
        	    if(time_set)
        	    {
        	        time_set = 0;
        	    }
        	    else
        	    {
            	    switch(display_mode)
            	    {
                	    case MODE_TIME: display_mode = MODE_TEMPERATURE;
                	        break;
                	    case MODE_TEMPERATURE: display_mode = MODE_POT;
                	        break;
                	    case MODE_POT: display_mode = MODE_TIME;
                	        break;
                	    default:
                            display_mode = MODE_TIME;
                	        break;
                	}
            	}    
        	    break;
        	default: break;
        }
            	 
        switch(display_mode)
        {
            case MODE_TIME: 
                display_time(); 
                break;
            case MODE_TEMPERATURE: 
                display_temp(mcp9800_get_temp()); 
                break;
            case MODE_POT:
                display_pot(input_pot());
                break;
            default: display_mode = MODE_TIME; break;
        }
    	while(!TMR0IF); // slow down the main loop
    }   	    
	return 0;
}

void interrupt isr()
{
    // Timer 1
    if(TMR1IF && TMR1IE)
    {
        TMR1IF = 0;
        rtcc_handler();
        S2 = ~S2;

        if(blink && RE0)
            lcd_display_off();
        else
            lcd_display_on();    
            
    }
   i2c_handler();
}

void display_temp(int t)
{
    static int counter = 0;
    static int integrator=0;
    int average;
    static int v = 0;
    
    // low pass filter
    integrator += t;
    average = integrator / 16;
    integrator -= average;
    
    // decimate
    if(counter-- == 0)
    {
        counter = 100;
        v = average;
    }

    // display
    display_int(v);
        
    DP3 = 0;
    DP2 = 1;
    S1 = 0;
    S2 = 0;
    AMPS = 1;
    VOLT = 0;
    KILO = 0;
    OHMS = 0;
    MINUS = 0;
}
    
void display_rpm(int r)
{
    display_int(r);
    DP3 = 0;
    DP2 = 0;
    S1 = 0;
    S2 = 0;
    AMPS = 0;
    VOLT = 1;
    KILO = 0;
    OHMS = 0;
    MINUS = 0;
}
    
void display_pot(int p)
{
    display_int(p);
    DP3 = 0;
    DP2 = 0;
    S1 = 0;
    S2 = 1;
    AMPS = 0;
    VOLT = 0;
    KILO = 0;
    OHMS = 0;
    MINUS = 0;
}

void display_int(int t)
{
    BCD_TYPE bcd;
    
    // assumes t is the temperature in degrees C * 10
    bcd.digit0 = t %10;
    t /= 10;
    bcd.digit1 = t % 10;
    t /= 10;
    bcd.digit2 = t % 10;
    t /= 10;
    bcd.digit3 = t%10;
    lcd_display_digits(bcd);   
}    

void display_time(void)
{
    time_t t;
    struct tm *d;
    BCD_TYPE bcd;
    
    time(&t);
    d = gmtime(&t);
    
    bcd.digit0= d->tm_min % 10;
    bcd.digit1= d->tm_min / 10;
    
    if(d->tm_hour > 11)
    {
        d->tm_hour -= 12;
        MINUS = 1;
    }    
    else
    {
        MINUS = 0;
    }
    
    if(d->tm_hour == 0) d->tm_hour = 12;
    
    bcd.digit2= d->tm_hour % 10;
    bcd.digit3= d->tm_hour/10;

    lcd_display_digits(bcd);
    DP3 = (t&1)?1:0;
    DP2 = 0;
    S1 = 0;
    AMPS = 0;
    VOLT = 0;
    KILO = 0;
    OHMS = 1;
}    