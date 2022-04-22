.MODEL	SMALL
.8086

.code

extrn _main:proc

.startup

	call  near ptr _main
endless:
   jmp   endless

.data

public	__acrtused		; trick to force in startup
	__acrtused = 9876h	; funny value not easily matched in SYMDEB

.stack

END
