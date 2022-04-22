        LIST

;==========================================================================
;  MPASM PIC12F617 processor include
; 
;  (c) Copyright 1999-2009 Microchip Technology, All rights reserved
;==========================================================================

        NOLIST

;==========================================================================
;  This header file defines configurations, registers, and other useful
;  bits of information for the PIC12F617 microcontroller.  These names
;  are taken to match the data sheets as closely as possible.
;
;  Note that the processor must be selected before this file is included.
;  The processor may be selected the following ways:
;
;       1. Command line switch:
;               C:\MPASM MYFILE.ASM /PIC12F617
;       2. LIST directive in the source file
;               LIST   P=PIC12F617
;       3. Processor Type entry in the MPASM full-screen interface
;       4. Setting the processor in the MPLAB Project Dialog
;==========================================================================

;==========================================================================
;
;       Verify Processor
;
;==========================================================================
        IFNDEF __12F617
           MESSG "Processor-header file mismatch.  Verify selected processor."
        ENDIF



;==========================================================================
;
;       Register Definitions
;
;==========================================================================

W                EQU  H'0000'
F                EQU  H'0001'

;----- Register Files -----------------------------------------------------

;-----Bank0------------------
INDF             EQU  H'0000'
TMR0             EQU  H'0001'
PCL              EQU  H'0002'
STATUS           EQU  H'0003'
FSR              EQU  H'0004'
GPIO             EQU  H'0005'
PORTA            EQU  H'0005'
PCLATH           EQU  H'000A'
INTCON           EQU  H'000B'
PIR1             EQU  H'000C'
TMR1             EQU  H'000E'
TMR1L            EQU  H'000E'
TMR1H            EQU  H'000F'
T1CON            EQU  H'0010'
TMR2             EQU  H'0011'
T2CON            EQU  H'0012'
CCPR1            EQU  H'0013'
CCPR1L           EQU  H'0013'
CCPR1H           EQU  H'0014'
CCP1CON          EQU  H'0015'
PWM1CON          EQU  H'0016'
ECCPAS           EQU  H'0017'
VRCON            EQU  H'0019'
CMCON0           EQU  H'001A'
CMCON1           EQU  H'001C'
ADRESH           EQU  H'001E'
ADCON0           EQU  H'001F'

;-----Bank1------------------
OPTION_REG       EQU  H'0081'
TRISA            EQU  H'0085'
TRISIO           EQU  H'0085'
PIE1             EQU  H'008C'
PCON             EQU  H'008E'
OSCTUNE          EQU  H'0090'
PR2              EQU  H'0092'
APFCON           EQU  H'0093'
WPU              EQU  H'0095'
IOC              EQU  H'0096'
PMCON1           EQU  H'0098'
PMCON2           EQU  H'0099'
PMADRL           EQU  H'009A'
PMADRH           EQU  H'009B'
PMDATL           EQU  H'009C'
PMDATH           EQU  H'009D'
ADRESL           EQU  H'009E'
ANSEL            EQU  H'009F'

;----- STATUS Bits -----------------------------------------------------
C                EQU  H'0000'
DC               EQU  H'0001'
Z                EQU  H'0002'
NOT_PD           EQU  H'0003'
NOT_TO           EQU  H'0004'
IRP              EQU  H'0007'


;----- GPIO Bits -----------------------------------------------------
GP0              EQU  H'0000'
GP1              EQU  H'0001'
GP2              EQU  H'0002'
GP3              EQU  H'0003'
GP4              EQU  H'0004'
GP5              EQU  H'0005'

RA0              EQU  H'0000'
RA1              EQU  H'0001'
RA2              EQU  H'0002'
RA3              EQU  H'0003'
RA4              EQU  H'0004'
RA5              EQU  H'0005'


;----- PORTA Bits -----------------------------------------------------
GP0              EQU  H'0000'
GP1              EQU  H'0001'
GP2              EQU  H'0002'
GP3              EQU  H'0003'
GP4              EQU  H'0004'
GP5              EQU  H'0005'

RA0              EQU  H'0000'
RA1              EQU  H'0001'
RA2              EQU  H'0002'
RA3              EQU  H'0003'
RA4              EQU  H'0004'
RA5              EQU  H'0005'


;----- INTCON Bits -----------------------------------------------------
GPIF             EQU  H'0000'
INTF             EQU  H'0001'
TMR0IF           EQU  H'0002'
GPIE             EQU  H'0003'
INTE             EQU  H'0004'
TMR0IE           EQU  H'0005'
PEIE             EQU  H'0006'
GIE              EQU  H'0007'


;----- PIR1 Bits -----------------------------------------------------
TMR1IF           EQU  H'0000'
TMR2IF           EQU  H'0001'
C1IF             EQU  H'0003'
ECCPIF           EQU  H'0005'
ADIF             EQU  H'0006'


;----- T1CON Bits -----------------------------------------------------
TMR1ON           EQU  H'0000'
TMR1CS           EQU  H'0001'
NOT_T1SYNC       EQU  H'0002'
T1OSCEN          EQU  H'0003'
TMR1GE           EQU  H'0006'
T1GINV           EQU  H'0007'

T1CKPS0          EQU  H'0004'
T1CKPS1          EQU  H'0005'


;----- T2CON Bits -----------------------------------------------------
TMR2ON           EQU  H'0002'

T2CKPS0          EQU  H'0000'
T2CKPS1          EQU  H'0001'
TOUTPS0          EQU  H'0003'
TOUTPS1          EQU  H'0004'
TOUTPS2          EQU  H'0005'
TOUTPS3          EQU  H'0006'


;----- CCP1CON Bits -----------------------------------------------------
P1M              EQU  H'0007'

CCP1M0           EQU  H'0000'
CCP1M1           EQU  H'0001'
CCP1M2           EQU  H'0002'
CCP1M3           EQU  H'0003'
DCB0             EQU  H'0004'
DCB1             EQU  H'0005'


;----- PWM1CON Bits -----------------------------------------------------
PRSEN            EQU  H'0007'

PDC0             EQU  H'0000'
PDC1             EQU  H'0001'
PDC2             EQU  H'0002'
PDC3             EQU  H'0003'
PDC4             EQU  H'0004'
PDC5             EQU  H'0005'
PDC6             EQU  H'0006'


;----- ECCPAS Bits -----------------------------------------------------
ECCPASE          EQU  H'0007'

PSSBD0           EQU  H'0000'
PSSBD1           EQU  H'0001'
PSSAC0           EQU  H'0002'
PSSAC1           EQU  H'0003'
ECCPAS0          EQU  H'0004'
ECCPAS1          EQU  H'0005'
ECCPAS2          EQU  H'0006'


;----- VRCON Bits -----------------------------------------------------
FBREN            EQU  H'0004'
VRR              EQU  H'0005'
C1VREN           EQU  H'0007'

VR0              EQU  H'0000'
VR1              EQU  H'0001'
VR2              EQU  H'0002'
VR3              EQU  H'0003'


;----- CMCON0 Bits -----------------------------------------------------
C1CH             EQU  H'0000'
C1R              EQU  H'0002'
C1POL            EQU  H'0004'
C1OE             EQU  H'0005'
C1OUT            EQU  H'0006'
C1ON             EQU  H'0007'


;----- CMCON1 Bits -----------------------------------------------------
C1SYNC           EQU  H'0000'
T1GSS            EQU  H'0001'
C1HYS            EQU  H'0003'
T1ACS            EQU  H'0004'


;----- ADCON0 Bits -----------------------------------------------------
ADON             EQU  H'0000'
GO_NOT_DONE      EQU  H'0001'
VCFG             EQU  H'0006'
ADFM             EQU  H'0007'

CHS0             EQU  H'0002'
CHS1             EQU  H'0003'
CHS2             EQU  H'0004'


;----- OPTION_REG Bits -----------------------------------------------------
PSA              EQU  H'0003'
T0SE             EQU  H'0004'
T0CS             EQU  H'0005'
INTEDG           EQU  H'0006'
NOT_GPPU         EQU  H'0007'

PS0              EQU  H'0000'
PS1              EQU  H'0001'
PS2              EQU  H'0002'


;----- TRISA Bits -----------------------------------------------------
TRISIO0          EQU  H'0000'
TRISIO1          EQU  H'0001'
TRISIO2          EQU  H'0002'
TRISIO3          EQU  H'0003'
TRISIO4          EQU  H'0004'
TRISIO5          EQU  H'0005'

TRISA0           EQU  H'0000'
TRISA1           EQU  H'0001'
TRISA2           EQU  H'0002'
TRISA3           EQU  H'0003'
TRISA4           EQU  H'0004'
TRISA5           EQU  H'0005'


;----- TRISIO Bits -----------------------------------------------------
TRISIO0          EQU  H'0000'
TRISIO1          EQU  H'0001'
TRISIO2          EQU  H'0002'
TRISIO3          EQU  H'0003'
TRISIO4          EQU  H'0004'
TRISIO5          EQU  H'0005'

TRISA0           EQU  H'0000'
TRISA1           EQU  H'0001'
TRISA2           EQU  H'0002'
TRISA3           EQU  H'0003'
TRISA4           EQU  H'0004'
TRISA5           EQU  H'0005'


;----- PIE1 Bits -----------------------------------------------------
TMR1IE           EQU  H'0000'
TMR2IE           EQU  H'0001'
C1IE             EQU  H'0003'
ECCPIE           EQU  H'0005'
ADIE             EQU  H'0006'


;----- PCON Bits -----------------------------------------------------
NOT_BOR          EQU  H'0000'
NOT_POR          EQU  H'0001'


;----- OSCTUNE Bits -----------------------------------------------------
TUN0             EQU  H'0000'
TUN1             EQU  H'0001'
TUN2             EQU  H'0002'
TUN3             EQU  H'0003'
TUN4             EQU  H'0004'


;----- APFCON Bits -----------------------------------------------------
P1ASEL           EQU  H'0000'
P1BSEL           EQU  H'0001'
T1GSEL           EQU  H'0004'


;----- WPU Bits -----------------------------------------------------
WPUA0            EQU  H'0000'
WPUA1            EQU  H'0001'
WPUA2            EQU  H'0002'
WPUA4            EQU  H'0004'
WPUA5            EQU  H'0005'


;----- IOC Bits -----------------------------------------------------
IOC0             EQU  H'0000'
IOC1             EQU  H'0001'
IOC2             EQU  H'0002'
IOC3             EQU  H'0003'
IOC4             EQU  H'0004'
IOC5             EQU  H'0005'


;----- PMCON1 Bits -----------------------------------------------------
RD               EQU  H'0000'
WR               EQU  H'0001'
WREN             EQU  H'0002'


;----- PMADRH Bits -----------------------------------------------------
PMADRH0          EQU  H'0000'
PMADRH1          EQU  H'0001'
PMADRH2          EQU  H'0002'


;----- ANSEL Bits -----------------------------------------------------
ANS0             EQU  H'0000'
ANS1             EQU  H'0001'
ANS2             EQU  H'0002'
ANS3             EQU  H'0003'
ADCS0            EQU  H'0004'
ADCS1            EQU  H'0005'
ADCS2            EQU  H'0006'



;==========================================================================
;
;       RAM Definitions
;
;==========================================================================
       __MAXRAM  H'00FF'
       __BADRAM  H'0006'-H'0009'
       __BADRAM  H'000D'
       __BADRAM  H'0018'
       __BADRAM  H'001B'
       __BADRAM  H'001D'
       __BADRAM  H'0086'-H'0089'
       __BADRAM  H'008D'
       __BADRAM  H'008F'
       __BADRAM  H'0091'
       __BADRAM  H'0094'
       __BADRAM  H'0097'
       __BADRAM  H'00C0'-H'00EF'

;==========================================================================
;
;       Configuration Bits
;
;   NAME            Address
;   CONFIG            2007h
;
;==========================================================================

; The following is an assignment of address values for all of the
; configuration registers for the purpose of table reads
_CONFIG          EQU  H'2007'

;----- CONFIG Options --------------------------------------------------
_LP_OSC              EQU  H'FFF8'    ; LP
_XT_OSC              EQU  H'FFF9'    ; XT
_HS_OSC              EQU  H'FFFA'    ; HS
_EC_OSC              EQU  H'FFFB'    ; EC
_INTRC_OSC_NOCLKOUT  EQU  H'FFFC'    ; Internal RC No Clock
_INTRC_OSC_CLKOUT    EQU  H'FFFD'    ; Internal RC Clockout
_EXTRC_OSC_NOCLKOUT  EQU  H'FFFE'    ; External RC No Clock
_EXTRC_OSC_CLKOUT    EQU  H'FFFF'    ; External RC Clockout

_WDT_OFF             EQU  H'FFF7'    ; Off
_WDT_ON              EQU  H'FFFF'    ; On

_PWRTE_ON            EQU  H'FFEF'    ; On
_PWRTE_OFF           EQU  H'FFFF'    ; Off

_MCLRE_OFF           EQU  H'FFDF'    ; Internal
_MCLRE_ON            EQU  H'FFFF'    ; External

_CP_ON               EQU  H'FFBF'    ; On
_CP_OFF              EQU  H'FFFF'    ; Off

_IOSCFS_4MHZ         EQU  H'FF7F'    ; 4 MHz
_IOSCFS_8MHZ         EQU  H'FFFF'    ; 8 MHz

_BOR_OFF             EQU  H'FCFF'    ; BOR disabled
_BOR_NSLEEP          EQU  H'FEFF'    ; BOR enabled in run, disabled in sleep
_BOR_ON              EQU  H'FFFF'    ; BOR Enabled

_DEVID1          EQU  H'2006'

_IDLOC0          EQU  H'2000'
_IDLOC1          EQU  H'2001'
_IDLOC2          EQU  H'2002'
_IDLOC3          EQU  H'2003'

        LIST
