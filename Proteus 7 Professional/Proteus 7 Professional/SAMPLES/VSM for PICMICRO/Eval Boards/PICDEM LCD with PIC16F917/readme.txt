PIC16F946/PIC16F917/PIC16F914 PIM DEMO FIRMWARE OPERATION
The demo firmware is made up of four components, which are individually displayed on
the LCD. The PIC microcontroller’s internal RC oscillator is used as the system clock
source.

1. Clock (F1)
This mode uses the Timer1 module and a 32 kHz clock crystal. Once this mode
is entered from the main menu, the Real-Time Clock will start counting from
12:00:00. The S2 switch is used to increment minutes The clock keeps running 
continuously until the mode is exited by pressing either the S1 switch.

2. Voltmeter (F2)
This mode uses the A/D module to measure the voltage of the R44 pot through
analog channel AN0, and displays a voltage between in millivolts from 0 - 3000mV on the
LCD using the LCD module. Voltage is continually updated until the mode is
exited by pressing the S1 switch.

3. Thermometer (F3)
This mode uses the A/D module to measure the voltage of the thermistor R11
through analog channel AN1, and converts to temperature in Fahrenheit and
displays the temperature on the LCD using the LCD module. Temperature is
continually updated until the mode is exited by pressing the S1
switch.

