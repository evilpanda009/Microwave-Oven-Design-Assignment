The demo firmware is made up of three components, which are individually displayed on
the LCD. The PIC microcontroller’s internal HF INT oscillator at 1 MHz is used as the system clock
source.

1. Voltmeter (F1)
This mode uses the A/D module to measure the voltage of the R44 pot through
analog channel AN0, and displays a voltage between 0.00V and 3.30V on the
LCD using the LCD module. Voltage is continually updated until the mode is
exited by pressing either the S1 or S2 switch.

2. Thermometer (F2)
This mode uses the A/D module to measure the voltage of the thermistor R11
through analog channel AN1, and converts to temperature in Celsius and
displays the temperature on the LCD using the LCD module. Temperature is
continually updated until the mode is exited by pressing either the S1 or S2
switch.

3. Clock (F3)
This mode uses the Timer1 module and a 32 kHz clock crystal. Once this mode
is entered from the main menu, the Real-Time Clock will start counting from
00:00:00. The S3 switch is used to increment minutes and the S4 switch is used
to increment hours. Press and hold S3, S4 to perform the increment operation.
The clock keeps running continuously until the mode is exited
by pressing either the S1 or S2 switch.By exiting this mode, the Voltmeter mode
is entered.


Note:- 

After programming with ICD2 remove the RJ11 cable to execute the demo.


