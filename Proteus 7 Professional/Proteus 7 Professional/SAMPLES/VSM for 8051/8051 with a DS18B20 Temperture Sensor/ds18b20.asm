   DQ       BIT P3.7 ; 1 wire line
   swpH     equ  0d2H
   swpL     equ  0ffH
   WDLSB    DATA 30H ;
   WDMSB    DATA 31H ;
;***************************************************************
   ORG 0000H
   LJMP MAIN
   ORG 000BH
   LJMP TMR0   ; Timer0 isr

;********************************************************
; Timer0 Interrupt Service Routine
TMR0:    MOV   TH0,#swpH 
         MOV   TL0,#swpL
         JB    21H,DSL
         MOV   P0,42H
         ORL   P0,#00100000B
         SJMP  EXIT
DSL:  
         MOV   P0,43H
         ORL   P0,#00010000B
EXIT: 
         CPL   21H
         RETI

; Main program
MAIN:    
TOINIT: 
         CLR   EA
         MOV   TMOD,#01H
         MOV   TH0,#swpH
         MOV   TL0,#swpL
         SETB  EA
         SETB  ET0
         SETB  TR0

;***********************************************************       
;
         MOV   R2,#2
         MOV   R0,#42H ;
OVER:
         MOV   @R0,#00H; 
         INC   R0
         DJNZ  R2,OVER
   
LOOP: 
         LCALL DSWD ;      
         SJMP  LOOP

;**********************************************************
; Read a temperature from the DS18B20
DSWD:
         LCALL RSTSNR      ; Init of the DS18B20 
         JNB   F0,KEND     
         MOV   R0,#0CCH
         LCALL SEND_BYTE    
         MOV   R0,#44H     
         LCALL SEND_BYTE   ; Send a Convert Command   
         SETB  EA
         MOV   48H,#1      
SS2: 
         MOV   49H,#255
SS1:
         MOV   4AH,#255
SS0: 
         DJNZ  4AH,SS0
         DJNZ  49H,SS1
         DJNZ  48H,SS2
         CLR   EA
         LCALL RSTSNR
         JNB   F0,KEND
         MOV   R0,#0CCH       
         LCALL SEND_BYTE
         MOV   R0,#0BEH         
         LCALL SEND_BYTE      ; Send Read Scratchpad command 
         LCALL READ_BYTE      ; Read the low byte from scratchpad 
         MOV   WDLSB,A        ; Save the temperature (low byte)
         LCALL READ_BYTE      ; Read the high byte from scratchpad
         MOV   WDMSB,A        ; Save the temperature (high byte)
         LCALL TRANS12
KEND:    
         SETB  EA
         RET
;**********************************************************
;
TRANS12:
         MOV   A,30H
         ANL   A,#0F0H
         MOV   3AH,A
         MOV   A,31H
         ANL   A,#0FH
         ORL   A,3AH
         SWAP  A
         MOV   B,#10
         DIV   AB
         ;MOV 42H,A
         MOV   43H,B ;
         MOV   b,#10
         DIV   ab
         MOV   42H,B
         MOV   41H,A
         RET
;*************************************************
; Send a byte to the 1 wire line
SEND_BYTE: ;
         MOV   A,R0
         MOV   R5,#8
SEN3:    CLR   C
         RRC   A
         JC    SEN1
         LCALL WRITE_0
         SJMP  SEN2
SEN1:    LCALL WRITE_1
SEN2:    DJNZ  R5,SEN3 ; 
         RET
;*************************************************
; Read a byte from the 1 wire line
READ_BYTE: 
         MOV   R5,#8
READ1:   LCALL READ
         RRC   A
         DJNZ  R5,READ1 ; 
         MOV   R0,A
         RET
;*************************************************
; Reset 1 wire line
RSTSNR:  SETB  DQ
         NOP
         NOP
         CLR   DQ
         MOV   R6,#250 ;
         DJNZ  R6,$
         MOV   R6,#50
         DJNZ  R6,$
         SETB  DQ ;
         MOV   R6,#15
         DJNZ  R6,$
         CALL  CHCK ;
         MOV   R6,#60
         DJNZ  R6,$
         SETB  DQ
         RET


;*************************************************
; low level subroutines
CHCK:    MOV   C,DQ
         JC    RST0
         SETB  F0 ;
         SJMP  CHCK0
RST0:    CLR   F0 ;
CHCK0:   RET

;*************************************************
WRITE_0: 
         CLR   DQ
         MOV   R6,#30
         DJNZ  R6,$
         SETB  DQ
         RET
;*************************************************
WRITE_1: 
         CLR   DQ 
         NOP
         NOP
         NOP
         NOP
         NOP
         SETB  DQ
         MOV   R6,#30
         DJNZ  R6,$
         RET

;*************************************************
READ:    SETB  DQ ;
         NOP
         NOP
         CLR   DQ
         NOP
         NOP
         SETB  DQ ;
         NOP
         NOP
         NOP
         NOP
         NOP
         NOP
         NOP
         MOV   C,DQ
         MOV   R6,#23
         DJNZ  R6,$
         RET

;**********************************************
DELAY10: MOV   R4,#20
D2:      MOV   R5,#30
         DJNZ  R5,$
         DJNZ  R4,D2
         RET

         end
