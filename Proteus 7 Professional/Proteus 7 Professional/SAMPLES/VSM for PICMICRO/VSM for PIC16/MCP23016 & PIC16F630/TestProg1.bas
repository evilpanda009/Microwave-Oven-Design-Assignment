'****************************************************************
'*  Name    : MCP23016.BAS                                      *
'*  Author  : Labcenter                                         *
'*  Notice  : Copyright (c) 2007 Labcenter Electronics Ltd      *
'*          : All Rights Reserved                               *
'*  Date    : 08/01/2007                                        *
'*  Version : 1.0                                               *
'*  Notes   : Demo programs for PIC16F630 driving an MCP23016   *
'*          :                                                   *
'****************************************************************

Device 16F630
Declare XTAL 8                      'USING 8 MHZ XTAL

Declare SCL_PIN     PORTC.0
Declare SDA_PIN     PORTC.1
 
Dim PP4             As Byte SYSTEM
Dim devAddr         As Byte
Dim regAddr         As Byte
Dim lValue          As Byte
Dim hValue          As Byte
Dim Err             As Byte
Dim forever         As Byte

Symbol GP0          0x00            ' I/O general purposes port 0
Symbol GP1          0x01            ' I/O general purposes port 1
Symbol OLAT0        0x02            ' Output latch register 0
Symbol OLAT1        0x03            ' Output latch register 1
Symbol IPOL0        0x04            ' Imput polarity port register 0
Symbol IPOL1        0x05            ' Input polarity port register 1
Symbol IODIR0       0x06            ' I/O direction register 0
Symbol IODIR1       0x07            ' I/O direction register 1
Symbol INTCAP0      0x08            ' Interrupt capture register 0
Symbol INTCAP1      0x09            ' Interrupt capture register 1
Symbol IOCON0       0x0A            ' I/O Control Register 0
Symbol IOCON1       0x0B            ' I/O Control Register 1

Symbol REDLED       PORTC.5
Symbol GREENLED     PORTC.4

Main:
    DelayMS 500                      ' Wait 500 ms start-up to allow RC clock to stabilize.
    ' Set Green Led on 
    Low     REDLED
    High    GREENLED
    ' Device Address. 
    ' We set this once, unless we need to address several devices on the same i2c bus.   
    devAddr = %01000000
    
    ' Set Fast Interrupt Sample Rate (200us)
    regAddr  = IOCON0
    lValue   = $01
    hValue   = $01
    GoSub WriteI2C 
    If Err = 1 Then exit

    ' programming GP0 as an output and GP1 as an input
    regAddr  = IODIR0
    lValue   = $00                  ' GP0 as an output
    hValue   = $FF                  ' GP1 as as input
    GoSub WriteI2C 
    If Err = 1 Then exit
    hValue   = 0 
    lValue   = 0
        
    forever  = 1                    ' Set the next loop being done forever !!
    ' Loop
    While forever = 1 
        ' Get value from read ports.
        regAddr  = GP0
        GoSub ReadI2C 
        If Err = 1 Then exit
        ' Write OLAT0. This writes in turn on GP0 output port with dip-switch value.
        hValue   = lValue
        regAddr  = OLAT0
        GoSub WriteI2C
        If Err = 1 Then exit        ' If something went wrong then exit gracefully
        ' Makes it responsive every 1 second, which seems quite reasonable to check
        ' interrupt-on-change effects. 
        DelayMS 1000
     Wend 

 exit:   
    ' Get error flagged with Red led on
    High REDLED
    Low  GREENLED
    End                             ' Stop program here !


' ********************************************************************************************
' Subroutines area

' Write lValue and hValue into the registers pair pointed by regAddr.
' The device responds to the hardware address in devAddr.  
' Packet is:  |S| devAddr|W| |ACK| regAddr |ACK| lValue |ACK| hValue |ACK| |P| 
' NOTE:
' The bus must remain free until after the ninth clock pulse for at least 12us. 
' Testing the ack bit if received ok would guarantee this.  

WriteI2C:
    Err = 0                         ' Clear error
    BStart                          ' Start
    BusOut devAddr                  ' Address the device to its write address  
    If PP4.0 = 1 Then Err = 1       ' Has ack been received ok ?
    BusOut regAddr                  ' Select register address pair we want write to 
    If PP4.0 = 1 Then Err = 1       ' Has ack been received ok ?
    BusOut lValue                   ' Write LSB or MSB byte of data value into the low register pair
    If PP4.0 = 1 Then Err = 1       ' Has ack been received ok ?
    BusOut hValue                   ' Write MSB Or LSB of data value into the high register pair
    If PP4.0 = 1 Then Err = 1       ' Has ack been received ok ?
    BStop
Return


' Read lValue and hValue from the registers pair pointed by regAddr.
' The device responds to the hardware address in devAddr.  
' Packet is:  |S| devAddr|W| |ACK| regAddr |RS| devAddr|R| |ACK| lValue |ACK| hValue |ACK| |P| 
ReadI2C:
    BStart                          ' Send a START condition
    BusOut devAddr                  ' Address the device to its write address  
    If PP4.0 = 1 Then Err = 1       ' Has ACK been received OK ?
    BusOut regAddr                  ' Select register address pair we want to read from
    If PP4.0 = 1 Then Err = 1       ' Has ACK been received OK ?
    BREStart                        ' Send RESTART
    BusOut devAddr+1                ' We now send again the Device Address but this time R/W bit is 1  
    If PP4.0 = 1 Then Err = 1       ' Has ACK been received OK ?
    BusIn  lValue                   ' Get data from LSB or MSB of register pair
    BusIn  hValue                   ' Get data from MSB or LSB of register pair
    BStop                           ' Send a STOP condition
Return 

