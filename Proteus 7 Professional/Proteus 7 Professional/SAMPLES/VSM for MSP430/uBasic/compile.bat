@echo on
SET CCEPATH=M:/tools/msp430 TI CCE v3.1/tools/compiler/MSP430/


rem *** UNVERSAL MAKE ***

SET PATH=%CCEPATH%bin
SET CMD=lnk_msp430f2419.cmd
del %FILE%.obj
del %FILE%.cof

cl430 --fp_reassoc=off --include_path="%CCEPATH%include" --sat_reassoc=off --symdebug:dwarf --keep_unneeded_statics --plain_char=unsigned --silicon_version=msp --printf_support=full terminal.c
cl430 --fp_reassoc=off --include_path="%CCEPATH%include" --sat_reassoc=off --symdebug:dwarf --keep_unneeded_statics --plain_char=unsigned --silicon_version=msp --printf_support=minimal tokenizer.c
cl430 --fp_reassoc=off --include_path="%CCEPATH%include" --sat_reassoc=off --symdebug:dwarf --keep_unneeded_statics --plain_char=unsigned --silicon_version=msp --printf_support=minimal ubasic.c
cl430 --fp_reassoc=off --include_path="%CCEPATH%include" --sat_reassoc=off --symdebug:dwarf --keep_unneeded_statics --plain_char=unsigned --silicon_version=msp --printf_support=minimal main.c
cl430 -z main.obj terminal.obj ubasic.obj tokenizer.obj  --reread_libs --warn_sections --search_path="%CCEPATH%lib" --search_path="%CCEPATH%include"  --library=%CMD% --library="rts430.lib" --rom_model --heap_size=0 --stack_size=50 -o basic.cof
hex430.exe -i basic.cof -o basic.hex -order MS -romwidth 16

rem del *.asm
del *.obj
del *.pwi
del *.dbk