; EEPROM - Read Write the EEPROM.
; Specify Device.
.device AT90S8515

; Variable Declarations
.def temp     = r16
.def isrsreg  = r17
.def isrtemp1 = r18     
.def isrtemp2 = r19    
.def isrflag  = r20     

.cseg					; CODE segment.

.org 0      
	rjmp init			; origin.

	    
;Main Routine                    
init:   ldi r16,$DF			; Initialize the stack.
	out SPL,r16
                    

; Write some data to the external memory at 0x0400
loop:   ldi r16,$A0			; Enable Sleep and SRAM - 0 ws
	out MCUCR,r16    
	ldi r16,$55
        sts $0400,r16           
        sts $0401,r16
        lds r0,$0400
        lds r0,$0401

	ldi r16,$E0			; Enable Sleep and SRAM - 1 ws
	out MCUCR,r16    
        ldi r16,$AA
        sts $0400,r16
        sts $0401,r16
        lds r0,$0400
        lds r0,$0401
        
	sleep	    
