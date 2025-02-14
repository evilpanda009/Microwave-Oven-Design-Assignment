*        title    REXPRES.TXT
*        page
******************************************************************************
*                                                                            *
*               RUNTIME EXPRESSION EVALUATION SUBROUTINE                     *
*                                                                            *
******************************************************************************
*
*
DONEXP:	LDAA   #OPARNTOK    ; USE AN OPEN PAREN AS AN END OF EXPRESSION MARKER.
	JSR    PSHOP        ; PUSH OPEN PAREN ON THE STACK.
DONEXP1:	LDAA   0,Y          ; GET THE NEXT CHARACTER IN THE EXPRESSION.
	CMPA   #OPARNTOK    ; IS IT AN OPEN PAREN?
	BNE    DONEXP4      ; NO. CONTINUE.
	INY                 ; POINT TO NEXT TOKEN.
	BSR    DONEXP       ; GO DO A SUBEXPRESSION.
	INY                 ; MOVE THE IP PAST THE CLOSING PAREN.
	BRA    DONEXP1      ; GO GET THE NEXT CHARACTER.
DONEXP4:	TSTA                ; CHECK FOR OPERATOR OR OPERAND.
	BPL    DONEXP2      ; IF NOT VARIABLE OR CONSTANT, GO CHECK FOR FUNCT.
	BSR    PSHNUM       ; GO PUSH OPERAND ONTO STACK.
	BRA    DONEXP1      ; GO GET NEXT TOKEN.
DONEXP2:	JSR    CHKNFUN      ; GO CHECK FOR FUNCTION THAT RETURNS A NUMBER.
	JSR    CHCKEE       ; GO CHECK FOR END OF EXPRESSION.
	BCC    DONEXP3      ; IF NOT END OF EXPRESSION, GO PUSH OPERATOR.
	RTS                 ; IF AT END, RETURN.
DONEXP3:	INY                 ; POINT TO THE NEXT TOKEN.
	JSR    PSHOP        ; PUSH OPERATOR ONTO STACK.
	BRA    DONEXP1      ; GO GET NEXT TOKEN.
*
*
*
*        PSHNUM SUBROUTINE
*
*        PUSHES A NUMERIC OPERAND (CONSTANT OR VARIABLE) VALUE ONTO THE
*        OPERAND STACK.
*
*
PSHNUM:	CMPA   #IVARTOK     ; IS IT AN INTEGER SCALER VARIABLE?
	BNE    PSHNUM1      ; NO. GO CHECK FOR CONSTANT.
	LDD    1,Y          ; YES. GET THE "OFFSET" ADDRESS.
	ADDD   VARBEGIN     ; ADD IN THE START ADDRESS OF THE VARIABLE TABLE.
	XGDX                ; GET THE ADDRESS INTO X.
	LDAB   #$03         ; BUMP INTERPRETER POINTER PAST "VARIABLE".
	ABY
	LDD    3,X          ; GET THE VARIABLE VALUE.
	BRA    PSHNUM4      ; GO PUT IT ON THE STACK.
PSHNUM1:	CMPA   #ICONTOK     ; IS IT AN INTEGER CONSTANT?
	BNE    PSHNUM2      ; NO. GO CHECK FOR AN INTEGER ARRAY VARIABLE.
	LDX    1,Y          ; GET THE CONSTANT VALUE INTO X.
	LDAB   #$04
	ADDB   3,Y
	ABY
	XGDX                ; PUT THE CONSTANT VALUE INTO D.
	BRA    PSHNUM4      ; GO PUT IT ON THE STACK.
PSHNUM2:	CMPA   #IAVARTOK    ; IS IT AN INTEGER ARRAY?
	BNE    PSHNUM3      ; NO. GO CHECK FOR A STRING VARIABLE.
	BSR    CALCSUB      ; GO GET BASE ADDR. & SUBSCRIPT OF ARRAY.
	PSHY                ; SAVE THE INTERPRETER POINTER.
	PSHX                ; PUT THE BASE ADDRESS OF THE ARRAY ON THE STACK.
	ASLD                ; MULTIPLY THE SUBSCRIPT BY THE # OF BYTES/ELEMENT.
	TSY                 ; POINT TO THE BASE ADDRESS.
	ADDD   0,Y          ; GET ADDRESS OF THE ELEMENT.
	PULX                ; RESTORE X.
	PULY                ; RESTORE Y
	XGDX                ; PUT ELEMENT ADDRESS INTO X.
	LDD    0,X          ; GET VALUE OF ELEMENT IN D.
	BRA    PSHNUM4 
PSHNUM3:	LDAA   #ILTOKERR
	JMP    RPTRERR
PSHNUM4:	LDX    NUMSTACK     ; GET THE OPERAND STACK POINTER.
	DEX                 ; MAKE ROOM ON THE STACK FOR NEW OPERAND.
	DEX
	CPX    ENUMSTK      ; HAS THE STACK OVERFLOWED?
	BHS    PSHNUM5      ; NO. GO STACK THE VALUE.
	LDAA   #MSTKOERR    ; YES.
	STAA   ERRCODE
	JMP    RPTRERR      ; GO REPORT THE ERROR.
PSHNUM5:	STX    NUMSTACK     ; SAVE THE STACK POINTER.
	STD    0,X          ; PUT THE VALUE ON THE STACK.
	RTS                 ; RETURN.
*
*
*        THIS SUBROUTINE CALCULATES BOTH THE BASE ADDRESS AND THE
*        SUBSCRIPT OF THE ARRAY VARIABLE THAT IS CURRENTLY POINTED TO BY
*        THE Y-REG. IT CHECKS TO SEE IF THE VARIABLE HAS BEEN DIMENTIONED
*        AND IF THE SUBSCRIPT IS IN RANGE. THE ROUTINE RETURNS WITH THE
*        ADDRESS OF THE ARRAY IN THE X-REG. & THE SUBSCRIPT IN THE D-REG.
*
CALCSUB:	LDD    1,Y          ; GET THE VARIABLE OFFSET ADDRESS.
	ADDD   VARBEGIN     ; ADD IN THE START OF THE VARIABLE AREA.
	XGDX                ; PUT ADDRESS INTO X.
	LDX    3,X          ; GET THE ACTUAL STORAGE ADDRESS.
*                             ; HAS THE ARRAY BEEN DIMENTIONED?
	BNE    CALCSUB2     ; YES. CONTINUE.
	LDAA   #UNDIMERR    ; NO. UNDIMENTIONED ARRAY REFERENCE.
CALCSUB1:	JMP    RPTRERR      ; GO REPORT THE ERROR.
CALCSUB2:	LDAB   #$4          ; SET POINTER TO START OF SUBSCRIPT EXPRESSION.
	ABY
	PSHX                ; SAVE THE POINTER TO THE ARRAY STORAGE AREA.
	JSR    DONEXP       ; GO GET THE SUBSCRIPT.
	INY                 ; BUMP IP PAST THE CLOSING PAREN OF THE SUBSCRIPT.
	PULX                ; RESTORE X.
	JSR    PULNUM       ; GET SUBSCRIPT FROM THE OPERAND STACK.
	CPD    0,X          ; IS THE SUBSCRIPT WITHIN RANGE?
	BLS    CALCSUB3     ; YES. CONTINUE.
	LDAA   #SUBORERR    ; NO. SUBSCRIPT OUT OF RANGE ERROR.
	BRA    CALCSUB1     ; GO REPORT IT.
CALCSUB3:	INX                 ; BYPASS THE SUBSCRIPT LIMIT.
	INX
	RTS
*
*
PULNUM:	PSHX                ; SAVE THE X-REG.
	LDX    NUMSTACK     ; GET THE OPERAND STACK POINTER.
	LDD    0,X          ; GET THE OPERAND.
	INX                 ; BUMP THE STACK POINTER.
	INX
	STX    NUMSTACK     ; SAVE THE STACK POINTER.
	PULX                ; RESTORE THE X-REG.
	CPD    #0           ; "TEST" THE OPERAND BEFORE WE RETURN.
	RTS                 ; RETURN.
*
*
*        /***** chcknfun() *****/
*
*        /* checks for a numeric function and performs it if present */
*
CHKNFUN:	CMPA   #FUNCTFLG    ; IS THIS A FUNCTION CALL?
	BEQ    CHKNFUN1     ; YES. GO DO THE FUNCTION.
	RTS                 ; NO. JUST RETURN.
CHKNFUN1:	LDAA   1,Y          ; GET THE FUNCTION CODE BYTE IN B.
	DECA                ; SUBTRACT 1 FOR INDEXING.
	LDAB   #3           ; BUMP THE IP.
	ABY                 ; POINT TO THE FIRST ELEMENT IN THE EXPRESSION.
	TAB                 ; PUT THE FUNCTION NUMBER INTO B.
	ASLB                ; MULT BY THE NUMBER OF BYTES/ADDRESS.
	LDX    #RNFUNCT     ; POINT TO THE FUNCTION ADDRESS TABLE.
	ABX                 ; POINT TO THE PROPER FUNCTION.
	LDX    0,X          ; GET THE ADDRESS INTO X.
	JSR    0,X          ; GO DO THE FUNCTION.
	INY                 ; PUT IP PAST THE CLOSING PAREN.
	LDAA   0,Y          ; GET NEXT CHARACTER.
	RTS                 ; RETURN.
*
*
RNFUNCT:	EQU    *
	FDB    RFDIV
	FDB    ICHRS        ; "ICHRS" BECAUSE IT'S ILLEGAL IN AN EXPRESSION.
	FDB    RADC
	FDB    RABS
	FDB    RRND
	FDB    RSGN
	FDB    ITAB         ; "ITAB" BECAUSE IT'S ILLEGAL IN AN EXPRESSION.
	FDB    RCALL
	FDB    RPEEK
	FDB    RFEEP        ; "EEP" AS A FUNCTION.
	FDB    IHEX         ; "IHEX" BECAUSE IT'S ILLEGAL IN AN EXPRESSION.
	FDB    RFPORTA
	FDB    RFPORTB
	FDB    RFPORTC
	FDB    RFPORTD
	FDB    RFPORTE
	FDB    RFTIME
	FDB    IHEX2        ; "IHEX2" BECAUSE IT'S ILLEGAL IN AN EXPRESSION.
	FDB    RFPACC
*
*
*        /***** chckee() *****/
*
*        /* if the current token is a semicolin, comma, colin, or space */
*        /* all pending operations on the math stack are performed and */
*        /* we return with the carry set */
*
CHCKEE:	EQU    *
	CMPA   #CPARNTOK    ; IS IT A CLOSED PAREN?
	BEQ    CHCKEE2      ; YES.
	CMPA   #MEOLTOK     ; IS IT ONE OF THE "EXPRESSION END" TOKENS?
	BHS    CHCKEE1      ; YES.
	CLC                 ; FLAG "NOT AT THE END OF EXPRESSION".
	RTS                 ; RETURN.
CHCKEE1:	LDAA   #CPARNTOK    ; END OF EXPRESSION FOUND. PERFORM ALL PENDING
CHCKEE2:	BSR    PSHOP        ; OPERATIONS.
	SEC                 ; FLAG END OF EXPRESSION.
	RTS
*
*
PSHOP:	LDX    OPSTACK      ; GET THE OPERATOR STACK POINTER.
	DEX                 ; DECREMENT THE STACK POINTER.
	CPX    EOPSTK       ; DID THE STACK OVERFLOW?
	BNE    PSHOP1       ; NO. CONTINUE.
	LDAA   #MSTKOERR    ; YES.
	JMP    RPTRERR      ; GO REPORT THE ERROR.
PSHOP1:	STX    OPSTACK
	STAA   0,X          ; PUT IT ON THE STACK.
PSHOP2:	LDX    OPSTACK
	LDAA   0,X          ; GET THE NEW OPERATOR OFF THE TOP OF STACK.
	CMPA   #OPARNTOK    ; IS IT AN OPEN PAREN?
	BEQ    PSHOP5       ; YES. GO PUSH IT.
	LDAB   1,X          ; GET THE PREVIOUS OPERATOR OFF THE STACK.
	ANDB   #$F0         ; MASK ALL BUT THE PRECIDENCE VALUE.
	ANDA   #$F0         ; MASK ALL BUT THE OPERATOR PRECIDENCE.
	CBA                 ; IS THE PRECIDENCE OF THE CURRENT OPERATOR >=
*                             ; THE OPERATOR ON THE TOP OF THE STACK?
	BHI    PSHOP5       ; NO. JUST GO PUSH IT ON THE STACK.
	LDAA   1,X          ; YES. GET THE PREVIOUS OPERATOR FROM THE STACK.
	LDAB   0,X          ; GET THE CURRENT OPERATOR FROM THE STACK.
	CMPB   #CPARNTOK    ; IS THE CURRENT OPERATOR A CLOSED PAREN?
	BNE    PSHOP3       ; NO. CONTINUE.
	CMPA   #OPARNTOK    ; YES. IS THE PREVIOUS OPERATOR AN OPEN PAREN?
	BNE    PSHOP3       ; NO. CONTINUE.
	INX                 ; YES. KNOCK BOTH OPERATORS OFF THE STACK.
	INX
	STX    OPSTACK      ; SAVE THE STACK POINTER.
PSHOP5:	RTS                 ; RETURN.
PSHOP3:	STAB   1,X          ; PUT IT ON THE STACK.
	INX                 ; UPDATE THE STACK POINTER.
	STX    OPSTACK
	BSR    DOOP         ; GO DO THE OPERATION.
	BRA    PSHOP2       ; GO TRY FOR ANOTHER OPERATION.
*
*
DOOP:	CMPA   #$70         ; IS IT A UINARY OPERATOR?
	BLO    DOOP1        ; NO. GO CHECK THE NEXT GROUP.
	SUBA   #$70         ; YES. SUBTRACT THE BASE VALUE OF THE GROUP.
	LDX    #HEIR7       ; POINT TO THE EXECUTION ADDRESS TABLE.
	BRA    DOOP7        ; GO DO THE OPERATION.
DOOP1:	CMPA   #$60         ; IS IT THE "^" OPERATOR?
	BLO    DOOP2        ; NO. GO CHECK THE NEXT GROUP.
	SUBA   #$60         ; YES. SUBTRACT THE BASE VALUE OF THE GROUP.
	LDX    #HEIR6       ; POINT TO THE EXECUTION ADDRESS TABLE.
	BRA    DOOP7        ; GO DO THE OPERATION.
DOOP2:	CMPA   #$50         ; IS IT MULTIPLY, DIVIDE, OR MOD?
	BLO    DOOP3        ; NO. GO CHECK THE NEXT GROUP.
	SUBA   #$50         ; YES. SUBTRACT THE BASE VALUE OF THE GROUP.
	LDX    #HEIR5       ; POINT TO THE EXECUTION ADDRESS TABLE.
	BRA    DOOP7        ; GO DO THE OPERATION.
DOOP3:	CMPA   #$40         ; IS IT ADD OR SUBTRACT?
	BLO    DOOP4        ; NO. GO CHECK THE NEXT GROUP.
	SUBA   #$40         ; YES. SUBTRACT THE BASE VALUE OF THE GROUP.
	LDX    #HEIR4       ; POINT TO THE EXECUTION ADDRESS TABLE.
	BRA    DOOP7        ; GO DO THE OPERATION.
DOOP4:	CMPA   #$30         ; IS IT A LOGICAL OPERATOR?
	BLO    DOOP5        ; NO. GO CHECK THE NEXT GROUP.
	SUBA   #$30         ; YES. SUBTRACT THE BASE VALUE OF THE GROUP.
	LDX    #HEIR3       ; POINT TO THE EXECUTION ADDRESS TABLE.
	BRA    DOOP7        ; GO DO THE OPERATION.
DOOP5:	CMPA   #$20         ; IS IT AND, OR, OR EOR?
	BLO    DOOP6        ; NO. ERROR.
	SUBA   #$20         ; YES. SUBTRACT THE BASE VALUE OF THE GROUP.
	LDX    #HEIR2       ; POINT TO THE EXECUTION ADDRESS TABLE.
	BRA    DOOP7        ; GO DO THE OPERATION.
DOOP6:	LDAA   #ILTOKERR    ; ILLEGAL OPERATOR TOKEN ENCOUNTERED.
	JMP    RPTRERR      ; GO REPORT THE ERROR.
DOOP7:	TAB                 ; PUT THE OFFSET IN B.
	ASLB                ; MULTIPLY THE OFFSET BY 2.
	ABX                 ; POINT TO THE ROUTINE ADDRESS.
	LDX    0,X          ; GET THE ADDRESS.
	JMP    0,X          ; GO DO THE OPERATION & RETURN.
*
*
HEIR7:	EQU    *
	FDB    RINDIR
	FDB    RNOT
	FDB    RNEG
HEIR6:	EQU    *
	FDB    RPWR
HEIR5:	EQU    *
	FDB    RMULT
	FDB    RDIV
	FDB    RMOD
HEIR4:	EQU    *
	FDB    RPLUS
	FDB    RMINUS
HEIR3:	EQU    *
	FDB    RLT
	FDB    RGT
	FDB    RLTEQ
	FDB    RGTEQ
	FDB    REQ
	FDB    RNOTEQ
HEIR2:	EQU    *
	FDB    RAND
	FDB    RORV
	FDB    REOR
*
*
REOR:	JSR    PULNUM
	LDX    NUMSTACK
	EORA   0,X
	EORB   1,X
REOR1:	STD    0,X
	RTS
*
*
RORV:	JSR    PULNUM
	LDX    NUMSTACK
	ORAA   0,X
	ORAB   1,X
	BRA    REOR1
*
*
RAND:	JSR    PULNUM
	LDX    NUMSTACK
	ANDA   0,X
	ANDB   1,X
	BRA    REOR1
*
*
RPLUS:	JSR    PULNUM
	LDX    NUMSTACK
	ADDD   0,X
	BRA    REOR1
*
*
RMINUS:	LDX    NUMSTACK
	LDD    2,X
	SUBD   0,X
	INX
	INX
	STD    0,X
	STX    NUMSTACK
	RTS
*
*
RDIV:	BSR    RDIVS        ; GO DO A SIGNED DIVIDE.
	JSR    PULNUM       ; GET INTEGER RESULT OFF STACK.
	LDX    NUMSTACK     ; POINT TO NUMERIC STACK.
	STD    0,X          ; OVERWRITE REMAINDER.
	RTS                 ; RETURN.
*
*
*
RDIVS:	LDX    NUMSTACK     ; POINT TO NUMERIC STACK.
	LDAA   0,X          ; GET UPPER BYTE OF DIVISOR.
	EORA   2,X          ; GET SIGN OF THE RESULT.
	PSHA                ; SAVE RESULT.
	LDD    0,X          ; GET DIVISOR OFF NUMERIC STACK. IS IT ZERO?
	BNE    RDIV1        ; NO. CONTINUE.
RDIV2:	LDAA   #ZDIVERR     ; YES. GET DIVIDE BY ZERO ERROR.
	JMP    RPTRERR      ; GO REPORT IT.
RDIV1:	BPL    RDIV3        ; IF POSITIVE IT'S OK.
	JSR    RNEG         ; IF NOT MAKE IT POSITIVE.
RDIV3:	TST    2,X          ; IS THE DIVIDEND NEGATIVE?
	BPL    RDIV4        ; NO. CONTINUE.
	LDD    2,X          ; YES. GET THE NUMBER.
	COMA                ; NEGATE IT.
	COMB
	ADDD   #1
	STD    2,X          ; SAVE THE RESULT.
RDIV4:	LDD    0,X          ; GET THE DIVISOR.
	LDX    2,X          ; GET THE DIVIDEND.
	XGDX                ; PUT THEM IN THE PROPER REGISTERS.
	IDIV                ; DO AN UNSIGNED DIVIDE.
	PSHX                ; SAVE THE QUOTIENT.
	LDX    NUMSTACK     ; POINT TO THE NUMERIC STACK.
	STD    2,X          ; SAVE THE REMAINDER.
*        PULD                 ; GET THE QUOTIENT.
	PULA
	PULB
	STD    0,X          ; PUT IT ON THE NUMERIC STACK.
	PULA                ; GET THE SIGN OF THE RESULT.
	TSTA                ; SET THE CONDITION CODES.
	BPL    RDIV5        ; IF PLUS, RESULT OK AS IS.
	JSR    RNEG         ; MAKE THE QUOTIENT NEGATIVE.
	LDD    2,X          ; GET THE REMAINDER.
	COMA                ; MAKE IT NEGATIVE.
	COMB
	ADDD   #1
	STD    2,X          ; SAVE THE RESULT.
RDIV5:	RTS                 ; RETURN.
*
*
RMOD:	BSR    RDIVS        ; GO GET QUOTIENT & REMAINDER.
	JSR    PULNUM       ; REMOVE INTEGER RESULT & LEAVE REMAINDER.
	RTS                 ; RETURN.
*
*
RMULT:	PSHY
	LDX    NUMSTACK
	LDAA   1,X
	LDAB   3,X
	MUL
*        PSHD
	PSHB
	PSHA
	TSY
	LDAA   1,X
	LDAB   2,X
	MUL
	ADDB   0,Y
	STAB   0,Y
	LDAA   0,X
	LDAB   3,X
	MUL
	ADDB   0,Y
	STAB   0,Y
	INX
	INX
*        PULD
	PULA
	PULB
	STD    0,X
	STX    NUMSTACK
	PULY
	RTS
*
*
RINDIR:	EQU    *
	RTS
*
*
RNOT:	EQU    *
	LDX    NUMSTACK
	LDD    0,X
	COMA
	COMB
	STD    0,X
	RTS
*
*
RNEG:	EQU    *
	BSR    RNOT
	ADDD   #1
	STD    0,X
	RTS
*
*
RLT:	EQU    *
	BSR    CMPNUM
	BGE    RLT1
RLT2:	INC    3,X
RLT1:	INX
	INX
	STX    NUMSTACK
	RTS
*
*
RGT:	EQU    *
	BSR    CMPNUM
	BLE    RLT1
	BRA    RLT2
*
*
RLTEQ:	EQU    *
	BSR    CMPNUM
	BGT    RLT1
	BRA    RLT2
*
*
RGTEQ:	EQU    *
	BSR    CMPNUM
	BLT    RLT1
	BRA    RLT2
*
*
REQ:	EQU    *
	BSR    CMPNUM
	BNE    RLT1
	BRA    RLT2
*
*
RNOTEQ:	EQU    *
	BSR    CMPNUM
	BEQ    RLT1
	BRA    RLT2
*
*
CMPNUM:	EQU    *
	LDX    NUMSTACK
	LDD    2,X
	CLR    2,X
	CLR    3,X
	CPD    0,X
	RTS
*
*
RPWR:	EQU    *
	RTS
*
*
RABS:	EQU    *
	JSR    DONEXP
	LDX    NUMSTACK
	LDD    0,X
	BPL    RABS1
RABS2:	COMA
	COMB
	ADDD   #1
RABS1:	STD    0,X
	RTS
*
*
RSGN:	EQU    *
	JSR    DONEXP
	LDX    NUMSTACK
	LDD    0,X
	BEQ    RABS1
	LDD    #1
	TST    0,X
	BPL    RABS1
	BRA    RABS2
*
*
RCALL:	EQU    *
	JSR    DONEXP
	LDX    NUMSTACK
	LDX    0,X
	JSR    0,X
	BRA    RPEEK1
*
*
RPEEK:	EQU    *
	JSR    DONEXP
	LDX    NUMSTACK
	LDX    0,X
	LDAB   0,X
	CLRA
RPEEK1:	LDX    NUMSTACK
	STD    0,X
	RTS
*
*
RFEEP:	EQU    *
	JSR    DONEXP       ; GO GET SUBSCRIPT OF EEPROM ARRAY.
	LDX    NUMSTACK     ; POINT TO THE OPERAND STACK.
	LDD    0,X          ; GET THE SUBSCRIPT OFF THE STACK.
	CPD    #MAXEESUB    ; IS IT WITHIN THE LIMIT?
	BLS    RFEEP1       ; YES. GO GET THE VALUE.
	LDAA   #EESUBERR    ; NO. SUBSCRIPT ERROR.
RFEEP2:	JMP    RPTRERR      ; REPORT THE ERROR.
RFEEP1:	LSLD                ; MULT THE SUBSCRIPT BY 2.
	ADDD   #EEPBASAD    ; ADD IN THE BASE ADDRESS OF THE EEPROM ADDRESS.
	XGDX                ; PUT THE ADDRESS IN X.
	LDD    0,X          ; GET THE DATA.
	BRA    RPEEK1       ; GO STEAL SOME CODE.
*
*
RFDIV:	EQU    *
	JSR    DONEXP       ; GO EVALUATE THE DIVIDEND EXPRESSION.
	JSR    RSKIPSPC     ; SKIP SPACES.
	INY                 ; PASS UP THE COMMA.
	JSR    RSKIPSPC     ; SKIP SPACES AFTER THE COMMA.
	JSR    DONEXP       ; EVALUATE THE DIVISOR EXPRESSION.
	LDX    NUMSTACK     ; POINT TO OPERAND STACK.
	LDD    2,X          ; GET THE DIVIDEND.
	LDX    0,X          ; GET THE DIVISOR.
	FDIV                ; DO THE FRACTIONAL DIVIDE.
	BVC    RFDIV1       ; ALL IS OK IF V=0. (IX > D).
	LDAA   #OVDV0ERR    ; ERROR. EITHER OVERFLOW OR /0 ERROR.
RFDIV2:	BRA    RFEEP2       ; GO REPORT IT.
RFDIV1:	XGDX                ; PUT QUOTIENT IN D.
	LDX    NUMSTACK     ; POINT TO OPERAND STACK.
	INX                 ; REMOVE DIVISOR FROM STACK.
	INX
	STD    0,X          ; PUT QUITIENT ON OPERAND STACK.
	STX    NUMSTACK     ; SAVE NEW VALUE OF STACK POINTER.
	RTS                 ; RETURN.
*
*
RADC:	EQU    *
	JSR    DONEXP       ; GO GET THE CHANNEL NUMBER TO CONVERT.
	LDX    NUMSTACK     ; POINT TO THE RESULT.
	LDD    0,X          ; GET THE CHANNEL NUMBER.
	BMI    RADC4        ; NEGATIVE CHANNEL NUMBERS ARE ILLEGAL.
	CPD    #7           ; IS IT A VALID CHANNEL NUMBER?
	BLS    RADC1        ; YES. GO CONVERT IT.
RADC4:	LDAA   #INVCHERR    ; NO. INVALID CHANNEL NUMBER.
	BRA    RFDIV2       ; GO REPORT THE ERROR.
RADC1:	LDX    IOBaseV
	STAB   ADCTL,X      ; START THE CONVERSION ON THE SELECTED.
RADC2:	TST    ADCTL,X      ; IS THE CONVERSION COMPLETE?
	BPL    RADC2        ; NO. WAIT FOR 4 CONVERSIONS ON 1 CHANNEL.
	CLRA                ; YES. NOW AVERAGE THE 4 CONVERSIONS.
	LDAB   ADR1,X       ; GET 1ST RESULT.
	ADDB   ADR2,X       ; ADD IN THE SECOND.
	ADCA   #0           ; ADD IN CARRY.
	ADDB   ADR3,X       ; ADD IN THE THIRD.
	ADCA   #0           ; ADD IN CARRY.
	ADDB   ADR4,X       ; ADD IN THE FOURTH.
	ADCA   #0           ; ADD IN CARRY.
	LSRD                ; DIVIDE RESULT BY 4.
	LSRD
	LDX    NUMSTACK     ; POINT TO THE RESULT.
	STD    0,X          ; PUT THE RESULT ON THE OPERAND STACK.
	RTS                 ; RETURN.
*
*
RRND:	EQU    *
	JSR    DONEXP       ; GO GET FUNCTION ARGUMENT.
	LDX    NUMSTACK     ; GET ARGUMENT OFF STACK. GET NEW RANDOM NUMBER?
	LDD    0,X
	BEQ    RRND2        ; YES. GO GET NEXT RANDOM NUMBER IN THE SERIES.
	BMI    RRND1        ; IF NEG., START A NEW SERIES.
	LDD    RANDOM       ; IF POSITIVE, GET LAST RANDOM NUMBER.
	BRA    RRND3        ; RETURN.
RRND1:	LDX    IOBaseV
	LDD    TCNT,X       ; USE THE TIMER VALUE AS THE NEW SEED.
	STD    RANDOM       ; SAVE IT.
RRND2:	LDD    RANDOM       ; GET PREVIOUS RANDOM NUMBER (USE AS SEED).
	ASLB                ; DO SOME OPERATIONS.
	ABA
	LDAB   RANDOM+1
	ASLD
	ASLD
	ADDD   RANDOM
	ADDD   #$3619
	STD    RANDOM
RRND3:	LSRD                ; MAKE THE NUMBER POSITIVE.
	STD    0,X          ; PUT THE NUMBER ON THE STACK.
	RTS                 ; RETURN.
*
*
ITAB:	EQU    *
ICHRS:	EQU    *
IHEX:	EQU    *
IHEX2:	EQU    *
	LDAA   #PRFUNERR	; THESE FUNCTIONS MUST BE USED ONLY IN
	JMP    RPTRERR	; PRINT STATEMENTS.
*
*
RFTIME:	LDD    TIMEREG      ; GET THE TIME IN SECONDS.
	BRA    RFPORTA2     ; GO PUT NUMBER ON THE STACK.
*
*
RFPACC:	LDX    IOBaseV
	LDAB   PACNT,X      ; GET THE CURRENT VALUE OF THE PULSE ACCUMULATOR.
	CLRA
	BRA    RFPORTA2     ; GO PUT THE NUMBER ON THE STACK.
*
*
RFPORTA:	EQU    *
	LDAB   #PORTAIO      ; GET DATA FROM PORTA.
RFPORTA1:	LDX    IOBaseV
	ABX
	LDAB   0,X
	CLRA                ; CLEAR UPPER BYTE OF WORD.
RFPORTA2:	DEY                 ; DECREMENT IP BECAUSE CALLING ROUTINE WILL TRY
	DEY                 ; TO BUMP IT PAST AN OPENING & CLOSING PAREN
*                             ; WHICH ISN'T THERE.
	JMP    PSHNUM4      ; GO PUSH VALUE ON OPERAND STACK & RETURN.
*
RFPORTB:	EQU    *
	LDAB   #PORTBIO
	BRA    RFPORTA1
*
RFPORTC:	EQU    *
	LDAB   #PORTCIO
	BRA    RFPORTA1
*
RFPORTD:	EQU    *
	LDAB   #PORTDIO
	BRA    RFPORTA1
*
RFPORTE:	EQU    *
	LDAB   #PORTEIO
	BRA    RFPORTA1
*
*
