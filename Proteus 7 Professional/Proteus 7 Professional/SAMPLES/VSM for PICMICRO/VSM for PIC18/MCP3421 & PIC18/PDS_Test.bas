'****************************************************************
'*  Name    : PDS_Test                                          *
'*  Author  : EA - Labcenter                                    *
'*  Notice  : Copyright (c) 2009                                *
'*          : All Rights Reserved                               *
'*  Date    : 28/01/2009                                        *
'*  Version : 1.0                                               *
'*  Notes   :                                                   *
'*          :                                                   *
'****************************************************************
   
   Device = 18F452
   XTAL = 4 
   
   CONFIG_START
      OSC = RC         ' Oscillator Selection RC
      BORV = 25        ' Brown-out Voltage 2.5V
      WDT = OFF        ' Watchdog Timer Disabled
   CONFIG_END
   
   INTERNAL_FONT = On 
   
   LCD_DTPORT = PORTD
   LCD_RSPIN  = PORTB.0
   LCD_ENPIN  = PORTB.1
   LCD_RWPIN  = PORTB.2
   LCD_CS1PIN = PORTB.3
   LCD_CS2PIN = PORTB.4
   LCD_TYPE   = GRAPHIC
   
   SCL_PIN    = PORTC.3
   SDA_PIN    = PORTC.4
   
   ' We want no pin with analogue features.
   ALL_DIGITAL = TRUE
   
   Dim devAddr       As Byte
   Dim Value_1st     As Byte
   Dim Value_2nd     As Byte
   Dim Value_3rd     As Byte
   Dim ConfReg       As Byte

   Dim rawMeasure    As DWord     
   Dim inpMeasure    As Float    ' Primary measure - voltage in.
   Dim Resistance    As Float    ' Secondary measure - PT100 resistance.
   Dim Temperature   As Float    ' Computed measure - temperature from PT100 resistance.
   
   ' PGA gain
   Dim Gain          As Float    
   
   ' Standard PT100 coefficients
   Dim R0            As Float
   Dim coeffA        As Float
   Dim coeffB        As Float
   
   Dim trialRes      As Float
   Dim err           As Float
   Dim absError      As Float
   Dim TempHigh      As Float
   Dim TempLow       As Float
   
   
   ' Device Address. 
   devAddr  = %11010000
   ' Constants.
   Gain     = 4.0 
   R0       = 100.00
   coeffA   = 3.9083 / 1000.0 
   coeffB   = -5.775 / 10000000.0
 
   DelayMS 100
   Cls
   Print At 0,2,  " PT100 Precision "
   Print At 1,2,  "Temperature Meter"
   Print At 12,1, "Temp : "
   Print At 30,1, "Rtd R: "
   Print At 12,72, " 0.00" , " "
   Print At 12,80, "degC"
   Print At 30,72, " 0.000", " Ohms "
  
   ' Select 18-bit resolution (3.75 SPS) and PGA Gain=4.
   ConfReg = %10011110
   GoSub WriteI2C   
   DelayMS 50
   
   ' Main loop
   While 1=1
      ' Get raw data from the MCP3421.
      GoSub ReadI2C 
      ' Get the value of the resistance (in Ohms) at unknow temperature.
      GoSub measureRes
      ' Compute the linearized temperature value (in degC).
      GoSub computeTemp
      
      ' Display updating. 
      Print At 12,72, DEC2 Temperature, " "
      Print At 12,80, "degC"
      Print At 30,72, DEC3 Resistance,  " Ohms "
   Wend 



' ********************************************************************************************
' Subroutines area
' ********************************************************************************************

' Value of the Platinum sensor at given temperature value.
' Given the resolution of 18-bit, the Gain=4 and the excitation current of 1mA then
' the resistance value is scaled directly from the raw data (3 bytes from the MCP3421).   
measureRes:
   rawMeasure = Value_3rd 
   rawMeasure = rawMeasure + (Value_2nd << 8)
   rawMeasure = rawMeasure + (Value_1st << 16)
   inpMeasure = rawMeasure / 131071.0  ' 131071=2^18-1
   inpMeasure = inpMeasure * 2048.0    ' 2048=Vref/Iexc=2.048V/1.0mA 
   Resistance = inpMeasure / Gain
Return


' Given the resistance we compute the linearized value of the Temperature.
' The calculation of the linearized temperature value would require solving a 2nd degree
' equation in order to find the correct result. (by using a (VERY!!) slow SQR instruction)
' 
' An iteractive bisection method is used (a root-finding algorithm). 
' The temperature value is that the whose theoretical sensor resistance approximates the 
' measured one with a known tolerance (1e-3 Ohms in this case).

computeTemp:
   ' Full initial temperature interval. 
   TempHigh = 850.0
   TempLow  = -200.0
  
TrialValue:
   ' Bisection method.  
   Temperature = (TempLow + TempHigh) * 0.5
   
   ' Callendar-Van Dusen Temperature/Resistance relation. 
   trialRes = Temperature * coeffB + coeffA 
   trialRes = trialRes * Temperature + 1.0
   trialRes = trialRes * R0     
   
   ' Find opposite sign. Also restricts the interval of the temperature.
   err = Resistance - trialRes 
   If err > 0.0 Then
     TempLow = Temperature
   Else
     TempHigh = Temperature   
   End If

   absError = Abs err   
   ' Re-try a new step if the absolute error is above the tolerance.  
   If absError > 0.001 Then TrialValue
   
   ' Here the temperature value converges and is made available. 
   ' The total accuracy should be within 0.01°C for positive values of the temperature.
 Return


' Write the configuration register.
' The device responds to the hardware address in devAddr.  
' Packet is:  |S| devAddr|W| |ACK| ConfReg |ACK| |P| 
WriteI2C:
   HBusOut  devAddr, [ConfReg]
Return


' Read the 1st, 2nd, 3rd and config byte.
' The device responds to the hardware address in devAddr.  
' Packet is:  |S| devAddr|W| |ACK| 1st_val |ACK| 2nd_val |ACK| 3rd_val |ACK| ConfReg |ACK| ... |ACK| ... |NAK| |P| 
ReadI2C:
   HBStart  
   HBusOut devAddr+1
   HBusIn  Value_1st
   HBusAck
   HBusIn  Value_2nd
   HBusAck
   HBusIn  Value_3rd
   HBusAck
   HBusIn  ConfReg
   ' Repeatedly test the $RDY bit until it is cleared (End Of Conversion).
   While  ConfReg And $80
      HBusAck
      HBusIn  ConfReg
      DelayMS 10
   Wend
   HBusOut 0xff    ' send a NAK
   HBStop
Return 


   Include "FONT.INC"
   End
