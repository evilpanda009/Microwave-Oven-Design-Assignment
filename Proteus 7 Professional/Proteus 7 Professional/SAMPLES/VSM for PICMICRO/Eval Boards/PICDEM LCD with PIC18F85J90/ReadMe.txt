The demo firmware is made up of four components, which are individually displayed on
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
to increment hours.The clock keeps running continuously until the mode is exited
by pressing either the S1 or S2 switch.

Press and hold S3, S4 to perform the increment operation.


4. Charge Pump and Software Contrast (F4)
This mode is available only for the PIC18F85J90 family of devices and uses the
on-board charge pump and LCD voltage regulator. The LCD module is configured
in charge pump mode and software contrast is achieved by either pressing
S3 to increase the contrast or S4 to decrease the contrast until the mode is exited
by pressing either the S1 or S2 switch. By exiting this mode, the Voltmeter mode
is entered and retains the selected contrast.

Note:- 

1. Use MPLAB 7.52 or later.
2. After programming with ICD2, remove the RJ11 cable for the demo to execute.