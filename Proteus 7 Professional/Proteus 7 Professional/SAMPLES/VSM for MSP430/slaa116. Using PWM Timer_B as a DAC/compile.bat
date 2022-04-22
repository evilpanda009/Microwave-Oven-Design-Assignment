@echo on
SET IARPATH=m:/tools/msp430 IAR 4.11a kickstart/430/
SET PATH=%IARPATH%bin

SET FILE=TBPWMDACcode
SET XCL=lnk430f2471

del %FILE%.obj
del %FILE%.cof
a430 -I"%IARPATH%inc"   %FILE%.s43
xlink -Fintel-standard  -xh %FILE%.r43  -f %XCL%  -o %FILE%.hex

del *.asm
del *.r43
del *.obj
del *.pwi
del *.dbk