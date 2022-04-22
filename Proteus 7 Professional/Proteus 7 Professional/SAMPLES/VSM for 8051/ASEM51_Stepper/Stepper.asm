      ORG      00H
START:
      MOV   DPTR,#TAB1
      MOV   R0,#3
      MOV   R4,#0
      MOV   P2,R0        

WAIT: MOV   P0,#0FFH
      JNB   P0.0,POS    ; Wait for a key pressed
      JNB   P0.1,NEG
      MOV   P2,#00H                
      SJMP  WAIT

POS:  MOV   R4,#1
      MOV   A,R4        ; Forward direction
      MOVC  A,@A+DPTR
      MOV   P2,A
      ACALL DELAY
      AJMP  KEY

NEG:  MOV   R4,#7       ; Reverse direction 
      MOV   A,R4
      MOVC  A,@A+DPTR
      MOV   P2,A
      ACALL DELAY
      AJMP  KEY

KEY:  MOV   P0,#03H
      JB    P0.0,NR1
      INC   R4
      CJNE  R4,#9,LOOPP
      MOV   R4,#1

LOOPP:   
      MOV   A,R4
      MOVC  A,@A+DPTR
      MOV   P2,A
      ACALL DELAY
      AJMP  KEY

NR1:  
      JB    P0.1,START
      DEC   R4
      CJNE  R4,#0,LOOPN
      MOV   R4,#8

LOOPN:   
      MOV   A,R4
      MOVC  A,@A+DPTR
      MOV   P2,A
      ACALL DELAY
      AJMP  KEY

DELAY:   
      MOV   R6,#1
DD1:  MOV   R5,#80H
DD2:  MOV   R7,#0
DD3:  DJNZ  R7,DD3
      DJNZ  R5,DD2
      DJNZ  R6,DD1
      RET

      ; Table of Stepping Sequences 
TAB1: DB    00H,02H,06H,04H
      DB    0CH,08H,09H,01H,03H
      END