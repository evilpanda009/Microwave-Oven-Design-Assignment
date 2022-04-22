
	; HI-TECH C PRO for the PIC10/12/16 MCU family V9.60PL5
	; Copyright (C) 1984-2009 HI-TECH Software
	;Serial no. HCPICP-36668
	;This licence will expire on Tue, 11 Dec 2029

	; Auto-generated runtime startup code for final link stage.

	;
	; Compiler options:
	;
	; -o946PIM2.cof -m946PIM2.map --summary=default --output=default \
	; main.p1 --chip=16F946 -P --runtime=default --opt=default -g --asmlist \
	; --errformat=Error   [%n] %f; %l.%c %s --msgformat=Advisory[%n] %s \
	; --warnformat=Warning [%n] %f; %l.%c %s
	;


	processor	16F946

	global	_main,start,_exit,reset_vec
	fnroot	_main
	psect	config,class=CONFIG,delta=2
	psect	idloc,class=IDLOC,delta=2
	psect	rbss_0,class=BANK0,space=1
	psect	rbss_1,class=BANK1,space=1
	psect	rbss_2,class=BANK2,space=1
	psect	rbss_3,class=BANK3,space=1
	psect	rdata_0,class=BANK0,space=1
	psect	rdata_1,class=BANK1,space=1
	psect	rdata_2,class=BANK2,space=1
	psect	rdata_3,class=BANK3,space=1
	psect	nvram,class=BANK0,space=1
	psect	nvram_1,class=BANK1,space=1
	psect	nvram_2,class=BANK2,space=1
	psect	nvram_3,class=BANK3,space=1
	psect	nvbit_0,class=BANK0,bit,space=1
	psect	nvbit_1,class=BANK1,bit,space=1
	psect	nvbit_2,class=BANK2,bit,space=1
	psect	nvbit_3,class=BANK3,bit,space=1
	psect	temp,ovrld,class=BANK0,space=1
	psect	struct,ovrld,class=BANK0,space=1
	psect	code,class=CODE,delta=2
	psect	rbit_0,class=BANK0,bit,space=1
	psect	ptbit_0,class=BANK0,bit,space=1
	psect	rbit_1,class=BANK1,bit,space=1
	psect	rbit_2,class=BANK2,bit,space=1
	psect	rbit_3,class=BANK3,bit,space=1
	psect	pstrings,class=CODE,delta=2
	psect	powerup,class=CODE,delta=2
	psect	reset_vec,class=CODE,delta=2
	psect	maintext,class=CODE,delta=2
	C	set	0
	Z	set	2
	PCL	set	2
	INDF	set	0

	psect	fnautoc,class=COMMON,space=1
	psect	common,class=COMMON,space=1
	psect	fnauto0,class=BANK0,space=1
	psect	fnauto1,class=BANK1,space=1
	psect	fnauto2,class=BANK2,space=1
	psect	fnauto3,class=BANK3,space=1
	STATUS	equ	3
	PCLATH	equ	0Ah

	psect	eeprom_data,class=EEDATA,delta=2,space=2
	psect	idata,class=CODE,delta=2
	psect	idata_0,class=CODE,delta=2
	psect	idata_1,class=CODE,delta=2
	psect	idata_2,class=CODE,delta=2
	psect	idata_3,class=CODE,delta=2
	psect	intcode,class=CODE,delta=2
	psect	intret,class=CODE,delta=2
	psect	intentry,class=CODE,delta=2
	global	intlevel0,intlevel1,intlevel2, intlevel3, intlevel4, intlevel5
intlevel0:
intlevel1:
intlevel2:
intlevel3:
intlevel4:
intlevel5:
	psect	intsave,class=BANK0,space=1
	psect	intsave_1,class=BANK1,space=1
	psect	intsave_2,class=BANK2,space=1
	psect	intsave_3,class=BANK3,space=1
	psect	init,class=CODE,delta=2
	psect	init23,class=CODE,delta=2
	psect	text,class=CODE,delta=2
	psect	end_init,class=CODE,delta=2
	psect	clrtext,class=CODE,delta=2
	psect	float_text0,class=CODE,delta=2,size=2048
	psect	float_text1,class=CODE,delta=2,size=2048
	psect	float_text2,class=CODE,delta=2,size=2048
	psect	float_text3,class=CODE,delta=2,size=2048
	psect	float_text4,class=CODE,delta=2,size=2048
	FSR	set	4
	psect	strings,class=CODE,delta=2,reloc=256

	psect	reset_vec
reset_vec:
	; No powerup routine
	global start

; jump to start
	movlw	start >>8
	movwf	PCLATH
	goto	start & 0x7FF | (reset_vec & not 0x7FF)



	psect	init
start
_exit

;-------------------------------------------------------------------------------
;		Clear (zero) uninitialized global variables

;	11 bytes of RAM objects in bank 0 to zero

	psect	init
	global	__Lrbss_0
;	Sequence of clrf's more optimal than using clear routine
	clrf	(__Lrbss_0+0)^0x0		;clear byte at RAM address 0x65
	clrf	(__Lrbss_0+1)^0x0		;clear byte at RAM address 0x66
	clrf	(__Lrbss_0+2)^0x0		;clear byte at RAM address 0x67
	clrf	(__Lrbss_0+3)^0x0		;clear byte at RAM address 0x68
	clrf	(__Lrbss_0+4)^0x0		;clear byte at RAM address 0x69
	clrf	(__Lrbss_0+5)^0x0		;clear byte at RAM address 0x6A
	clrf	(__Lrbss_0+6)^0x0		;clear byte at RAM address 0x6B
	clrf	(__Lrbss_0+7)^0x0		;clear byte at RAM address 0x6C
	clrf	(__Lrbss_0+8)^0x0		;clear byte at RAM address 0x6D
	clrf	(__Lrbss_0+9)^0x0		;clear byte at RAM address 0x6E
	clrf	(__Lrbss_0+10)^0x0		;clear byte at RAM address 0x6F

;	No RAM objects to clear in bank 1
;	No RAM objects to clear in bank 2
;	No RAM objects to clear in bank 3
;	No RAM objects to clear in common bank

;-------------------------------------------------------------------------------
	psect	end_init
	ljmp _main
	end	start
