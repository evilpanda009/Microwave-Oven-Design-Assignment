set targetcpu=LM3S308

set dlibconfig="DLib_Config_Normal.h"

set iarpath="m:\tools\arm IAR 6.10.2 kickstart\arm"
set linkercfg="M:\tools\arm IAR 6.10.2 kickstart\arm\CONFIG\generic_cortex.icf"
set include="M:\model\cortex-m3\conformance\include"

del *.elf

rem COMPILE

%iarpath%\bin\iccarm.exe include\core_cm3.c     -D ewarm -D PART_%targetcpu% --diag_suppress Pa050 --debug --endian=little --cpu=Cortex-M3 -e --fpu=None --dlib_config %dlibconfig% -I \"%include%\" --use_c++_inline
%iarpath%\bin\iccarm.exe include\system_lm3s.c  -D ewarm -D PART_%targetcpu% --diag_suppress Pa050 --debug --endian=little --cpu=Cortex-M3 -e --fpu=None --dlib_config %dlibconfig% -I \"%include%\" --use_c++_inline
%iarpath%\bin\iccarm.exe startup.c              -D ewarm -D PART_%targetcpu% --diag_suppress Pa050 --debug --endian=little --cpu=Cortex-M3 -e --fpu=None --dlib_config %dlibconfig% -I \"%include%\" --use_c++_inline
%iarpath%\bin\iccarm.exe evalboard.c            -D ewarm -D PART_%targetcpu% --diag_suppress Pa050 --debug --endian=little --cpu=Cortex-M3 -e --fpu=None --dlib_config %dlibconfig% -I \"%include%\" --use_c++_inline

rem LINK

%iarpath%\bin\ilinkarm.exe --redirect _Printf=_PrintfTiny --redirect _Scanf=_ScanfSmall -o evalboard.elf --config %linkercfg% --semihosting --entry __iar_program_start       startup.o evalboard.o system_lm3s.o core_cm3.o


del *.map
del *.asm
del *.sdf
del *.pwi
del *.dbk
del *.o