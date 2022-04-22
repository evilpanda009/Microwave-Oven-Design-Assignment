set targetcpu=LM3S315

set dlibconfig="DLib_Config_Normal.h"

set iarpath="m:\tools\arm IAR 6.10.2 kickstart\arm"
set linkercfg="M:\tools\arm IAR 6.10.2 kickstart\arm\CONFIG\generic_cortex.icf"
set include="m:\model\cortex-m3\demos\filter\inc"

rem set iarpath="d:\users\um\cortex-m3\IAR 6.10.2 ARM\arm"
rem set linkercfg="d:\users\um\cortex-m3\IAR 6.10.2 ARM\arm\config\generic_cortex.icf"
rem set include="d:\users\um\cortex-m3\conformance\include"

del *.cof

rem COMPILE

%iarpath%\bin\iccarm.exe driverlib\sysctl.c     -D ewarm -D PART_%targetcpu% --diag_suppress Pa050 --debug --endian=little --cpu=Cortex-M3 -e --fpu=None --dlib_config %dlibconfig% -I \"%include%\" --use_c++_inline
%iarpath%\bin\iccarm.exe driverlib\adc.c        -D ewarm -D PART_%targetcpu% --diag_suppress Pa050 --debug --endian=little --cpu=Cortex-M3 -e --fpu=None --dlib_config %dlibconfig% -I \"%include%\" --use_c++_inline
%iarpath%\bin\iccarm.exe driverlib\timer.c      -D ewarm -D PART_%targetcpu% --diag_suppress Pa050 --debug --endian=little --cpu=Cortex-M3 -e --fpu=None --dlib_config %dlibconfig% -I \"%include%\" --use_c++_inline
%iarpath%\bin\iccarm.exe driverlib\interrupt.c  -D ewarm -D PART_%targetcpu% --diag_suppress Pa050 --debug --endian=little --cpu=Cortex-M3 -e --fpu=None --dlib_config %dlibconfig% -I \"%include%\" --use_c++_inline
%iarpath%\bin\iccarm.exe driverlib\gpio.c       -D ewarm -D PART_%targetcpu% --diag_suppress Pa050 --debug --endian=little --cpu=Cortex-M3 -e --fpu=None --dlib_config %dlibconfig% -I \"%include%\" --use_c++_inline
%iarpath%\bin\iccarm.exe driverlib\cpu.c        -D ewarm -D PART_%targetcpu% --diag_suppress Pa050 --debug --endian=little --cpu=Cortex-M3 -e --fpu=None --dlib_config %dlibconfig% -I \"%include%\" --use_c++_inline
%iarpath%\bin\iccarm.exe startup.c              -D ewarm -D PART_%targetcpu% --diag_suppress Pa050 --debug --endian=little --cpu=Cortex-M3 -e --fpu=None --dlib_config %dlibconfig% -I \"%include%\" --use_c++_inline
%iarpath%\bin\iccarm.exe filter.c               -D ewarm -D PART_%targetcpu% --diag_suppress Pa050 --debug --endian=little --cpu=Cortex-M3 -e --fpu=None --dlib_config %dlibconfig% -I \"%include%\" --use_c++_inline

rem LINK

%iarpath%\bin\ilinkarm.exe --redirect _Printf=_PrintfTiny --redirect _Scanf=_ScanfSmall -o filter.elf --config %linkercfg% --semihosting --entry __iar_program_start       startup.o sysctl.o timer.o interrupt.o gpio.o filter.o cpu.o adc.o


del *.map
del *.asm
del *.sdf
del *.pwi
del *.dbk
del *.o
