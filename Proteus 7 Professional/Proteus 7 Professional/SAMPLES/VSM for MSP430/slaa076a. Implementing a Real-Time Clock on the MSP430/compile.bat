@echo on
SET IARPATH=m:/tools/msp430 IAR 4.11a kickstart/430/
SET PATH=%IARPATH%bin

SET FILE=RTC11xTA
del %FILE%.obj
del %FILE%.cof
a430 -I"%IARPATH%inc"   %FILE%.s43
xlink -Fintel-standard  -xh %FILE%.r43  -f lnk430f1111  -o %FILE%.hex

SET FILE=RTC11xWD
del %FILE%.obj
del %FILE%.cof
a430 -I"%IARPATH%inc"   %FILE%.s43
xlink -Fintel-standard  -xh %FILE%.r43  -f lnk430f1111  -o %FILE%.hex

del *.asm
del *.r43
del *.obj
del *.pwi
del *.dbk