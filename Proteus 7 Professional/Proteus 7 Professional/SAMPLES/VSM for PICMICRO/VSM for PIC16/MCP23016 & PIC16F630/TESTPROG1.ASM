;----------------------------------------------------------
; Code Produced by the PROTON+ Compiler. Version 3.2.5.2
; Copyright Rosetta Technologies/Crownhill Associates
; Written by Les Johnson. October 2006
;----------------------------------------------------------
 NOLIST
 #include "K:\PROLIBS\MICROCHIP\VSM\MCP23016\PROTON BASIC\TESTPROG1.PBP"
 LIST

 DEVADDR = 41
 REGADDR = 42
 LVALUE = 43
 HVALUE = 44
 ERR = 45
 FOREVER = 46
	#Define __SCL_PORT PORTC		;7
	#Define __SDA_PORT PORTC		;7
	#Define GP0 0
	#Define GP1 1
	#Define OLAT0 2
	#Define OLAT1 3
	#Define IPOL0 4
	#Define IPOL1 5
	#Define IODIR0 6
	#Define IODIR1 7
	#Define INTCAP0 8
	#Define INTCAP1 9
	#Define IOCON0 10
	#Define IOCON1 11
	#Define REDLED PORTC,5		;7
	#Define GREENLED PORTC,4		;7
F2_SOF equ $ ; TESTPROG1.PRP
F2_EOF equ $ ; TESTPROG1.PRP
F1_SOF equ $ ; TESTPROG1.BAS
MAIN
F1_000043 equ $ ; in [TESTPROG1.BAS] DelayMS 500                      ' Wait 500 ms start-up to allow RC clock to stabilize.
	Movlw 1
	Movwf PP1H
	Movlw 244
	Call dly@w
F1_000045 equ $ ; in [TESTPROG1.BAS] Low     REDLED
	Bsf STATUS,5
ram_bank = 1
	Bcf TRISC,5
	Bcf STATUS,5
ram_bank = 0
	Bcf PORTC,5
F1_000046 equ $ ; in [TESTPROG1.BAS] High    GREENLED
	Bsf STATUS,5
ram_bank = 1
	Bcf TRISC,4
	Bcf STATUS,5
ram_bank = 0
	Bsf PORTC,4
F1_000049 equ $ ; in [TESTPROG1.BAS] devAddr = %01000000
	Movlw 64
	Movwf DEVADDR
F1_000052 equ $ ; in [TESTPROG1.BAS] regAddr  = IOCON0
	Movlw 10
	Movwf REGADDR
F1_000053 equ $ ; in [TESTPROG1.BAS] lValue   = $01
	Movlw 1
	Movwf LVALUE
F1_000054 equ $ ; in [TESTPROG1.BAS] hValue   = $01
	Movlw 1
	Movwf HVALUE
F1_000055 equ $ ; in [TESTPROG1.BAS] GoSub WriteI2C
	Call WRITEI2C
F1_000056 equ $ ; in [TESTPROG1.BAS] If Err = 1 Then exit
	Movlw 1
	Subwf ERR,W
	Btfss STATUS,2
	Goto bc@LL2
	Goto EXIT
bc@LL2
F1_000059 equ $ ; in [TESTPROG1.BAS] regAddr  = IODIR0
	Movlw 6
	Movwf REGADDR
F1_000060 equ $ ; in [TESTPROG1.BAS] lValue   = $00                  ' GP0 as an output
	Clrf LVALUE
F1_000061 equ $ ; in [TESTPROG1.BAS] hValue   = $FF                  ' GP1 as as input
	Movlw 255
	Movwf HVALUE
F1_000062 equ $ ; in [TESTPROG1.BAS] GoSub WriteI2C
	Call WRITEI2C
F1_000063 equ $ ; in [TESTPROG1.BAS] If Err = 1 Then exit
	Movlw 1
	Subwf ERR,W
	Btfss STATUS,2
	Goto bc@LL4
	Goto EXIT
bc@LL4
F1_000064 equ $ ; in [TESTPROG1.BAS] hValue   = 0
	Clrf HVALUE
F1_000065 equ $ ; in [TESTPROG1.BAS] lValue   = 0
	Clrf LVALUE
F1_000067 equ $ ; in [TESTPROG1.BAS] forever  = 1                    ' Set the next loop being done forever !!
	Movlw 1
	Movwf FOREVER
F1_000069 equ $ ; in [TESTPROG1.BAS] While forever = 1
bc@LL5
	Movlw 1
	Subwf FOREVER,W
	Btfss STATUS,2
	Goto bc@LL6
F1_000071 equ $ ; in [TESTPROG1.BAS] regAddr  = GP0
	Clrf REGADDR
F1_000072 equ $ ; in [TESTPROG1.BAS] GoSub ReadI2C
	Call READI2C
F1_000073 equ $ ; in [TESTPROG1.BAS] If Err = 1 Then exit
	Movlw 1
	Subwf ERR,W
	Btfss STATUS,2
	Goto bc@LL8
	Goto EXIT
bc@LL8
F1_000075 equ $ ; in [TESTPROG1.BAS] hValue   = lValue
	Movf LVALUE,W
	Movwf HVALUE
F1_000076 equ $ ; in [TESTPROG1.BAS] regAddr  = OLAT0
	Movlw 2
	Movwf REGADDR
F1_000077 equ $ ; in [TESTPROG1.BAS] GoSub WriteI2C
	Call WRITEI2C
F1_000078 equ $ ; in [TESTPROG1.BAS] If Err = 1 Then exit        ' If something went wrong then exit gracefully
	Movlw 1
	Subwf ERR,W
	Btfss STATUS,2
	Goto bc@LL10
	Goto EXIT
bc@LL10
F1_000081 equ $ ; in [TESTPROG1.BAS] DelayMS 1000
	Movlw 3
	Movwf PP1H
	Movlw 232
	Call dly@w
F1_000082 equ $ ; in [TESTPROG1.BAS] Wend
	Goto bc@LL5
bc@LL6
EXIT
F1_000086 equ $ ; in [TESTPROG1.BAS] High REDLED
	Bsf STATUS,5
ram_bank = 1
	Bcf TRISC,5
	Bcf STATUS,5
ram_bank = 0
	Bsf PORTC,5
F1_000087 equ $ ; in [TESTPROG1.BAS] Low  GREENLED
	Bsf STATUS,5
ram_bank = 1
	Bcf TRISC,4
	Bcf STATUS,5
ram_bank = 0
	Bcf PORTC,4
F1_000088 equ $ ; in [TESTPROG1.BAS] End                             ' Stop program here !
Pb@lb12
	Sleep
	Goto Pb@lb12
WRITEI2C
F1_000102 equ $ ; in [TESTPROG1.BAS] Err = 0                         ' Clear error
	Clrf ERR
F1_000103 equ $ ; in [TESTPROG1.BAS] BStart                          ' Start
	Call bus@strt
F1_000104 equ $ ; in [TESTPROG1.BAS] BusOut devAddr                  ' Address the device to its write address
	Movf DEVADDR,W
	Call Busoutb
F1_000105 equ $ ; in [TESTPROG1.BAS] If PP4.0 = 1 Then Err = 1       ' Has ack been received ok ?
	Btfss PP4,0
	Goto bc@LL13
	Movlw 1
	Movwf ERR
bc@LL13
F1_000106 equ $ ; in [TESTPROG1.BAS] BusOut regAddr                  ' Select register address pair we want write to
	Movf REGADDR,W
	Call Busoutb
F1_000107 equ $ ; in [TESTPROG1.BAS] If PP4.0 = 1 Then Err = 1       ' Has ack been received ok ?
	Btfss PP4,0
	Goto bc@LL15
	Movlw 1
	Movwf ERR
bc@LL15
F1_000108 equ $ ; in [TESTPROG1.BAS] BusOut lValue                   ' Write LSB or MSB byte of data value into the low register pair
	Movf LVALUE,W
	Call Busoutb
F1_000109 equ $ ; in [TESTPROG1.BAS] If PP4.0 = 1 Then Err = 1       ' Has ack been received ok ?
	Btfss PP4,0
	Goto bc@LL17
	Movlw 1
	Movwf ERR
bc@LL17
F1_000110 equ $ ; in [TESTPROG1.BAS] BusOut hValue                   ' Write MSB Or LSB of data value into the high register pair
	Movf HVALUE,W
	Call Busoutb
F1_000111 equ $ ; in [TESTPROG1.BAS] If PP4.0 = 1 Then Err = 1       ' Has ack been received ok ?
	Btfss PP4,0
	Goto bc@LL19
	Movlw 1
	Movwf ERR
bc@LL19
F1_000112 equ $ ; in [TESTPROG1.BAS] BStop
	Call bus@stp
F1_000113 equ $ ; in [TESTPROG1.BAS] Return
	Return
READI2C
F1_000120 equ $ ; in [TESTPROG1.BAS] BStart                          ' Send a START condition
	Call bus@strt
F1_000121 equ $ ; in [TESTPROG1.BAS] BusOut devAddr                  ' Address the device to its write address
	Movf DEVADDR,W
	Call Busoutb
F1_000122 equ $ ; in [TESTPROG1.BAS] If PP4.0 = 1 Then Err = 1       ' Has ACK been received OK ?
	Btfss PP4,0
	Goto bc@LL21
	Movlw 1
	Movwf ERR
bc@LL21
F1_000123 equ $ ; in [TESTPROG1.BAS] BusOut regAddr                  ' Select register address pair we want to read from
	Movf REGADDR,W
	Call Busoutb
F1_000124 equ $ ; in [TESTPROG1.BAS] If PP4.0 = 1 Then Err = 1       ' Has ACK been received OK ?
	Btfss PP4,0
	Goto bc@LL23
	Movlw 1
	Movwf ERR
bc@LL23
F1_000125 equ $ ; in [TESTPROG1.BAS] BREStart                        ' Send RESTART
	Call bus@strt
F1_000126 equ $ ; in [TESTPROG1.BAS] BusOut devAddr+1                ' We now send again the Device Address but this time R/W bit is 1
	Incf DEVADDR,W
	Call Busoutb
F1_000127 equ $ ; in [TESTPROG1.BAS] If PP4.0 = 1 Then Err = 1       ' Has ACK been received OK ?
	Btfss PP4,0
	Goto bc@LL25
	Movlw 1
	Movwf ERR
bc@LL25
F1_000128 equ $ ; in [TESTPROG1.BAS] BusIn  lValue                   ' Get data from LSB or MSB of register pair
	Call Businb
	Movwf LVALUE
F1_000129 equ $ ; in [TESTPROG1.BAS] BusIn  hValue                   ' Get data from MSB or LSB of register pair
	Call Businb
	Movwf HVALUE
F1_000130 equ $ ; in [TESTPROG1.BAS] BStop                           ' Send a STOP condition
	Call bus@stp
F1_000131 equ $ ; in [TESTPROG1.BAS] Return
	Return
F1_EOF equ $ ; TESTPROG1.BAS
 LIST
	END

