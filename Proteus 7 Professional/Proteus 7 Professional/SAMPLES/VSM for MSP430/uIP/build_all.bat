@echo on
SET CCEPATH=M:/tools/msp430 TI CCE v3.1/tools/compiler/MSP430/


rem *** UNVERSAL MAKE ***

SET PATH=%CCEPATH%bin
SET CMD=lnk_msp430f2419.cmd

cl430 --fp_reassoc=off --include_path="%CCEPATH%include" --sat_reassoc=off --symdebug:dwarf --keep_unneeded_statics --plain_char=unsigned --silicon_version=msp --printf_support=minimal terminal.c
cl430 --fp_reassoc=off --include_path="%CCEPATH%include" --sat_reassoc=off --symdebug:dwarf --keep_unneeded_statics --plain_char=unsigned --silicon_version=msp --printf_support=minimal RTL8019.c
cl430 --fp_reassoc=off --include_path="%CCEPATH%include" --sat_reassoc=off --symdebug:dwarf --keep_unneeded_statics --plain_char=unsigned --silicon_version=msp --printf_support=minimal nic.c
cl430 --fp_reassoc=off --include_path="%CCEPATH%include" --sat_reassoc=off --symdebug:dwarf --keep_unneeded_statics --plain_char=unsigned --silicon_version=msp --printf_support=minimal cgi.c
cl430 --fp_reassoc=off --include_path="%CCEPATH%include" --sat_reassoc=off --symdebug:dwarf --keep_unneeded_statics --plain_char=unsigned --silicon_version=msp --printf_support=minimal fs.c
cl430 --fp_reassoc=off --include_path="%CCEPATH%include" --sat_reassoc=off --symdebug:dwarf --keep_unneeded_statics --plain_char=unsigned --silicon_version=msp --printf_support=minimal app.c
cl430 --fp_reassoc=off --include_path="%CCEPATH%include" --sat_reassoc=off --symdebug:dwarf --keep_unneeded_statics --plain_char=unsigned --silicon_version=msp --printf_support=minimal httpd.c
cl430 --fp_reassoc=off --include_path="%CCEPATH%include" --sat_reassoc=off --symdebug:dwarf --keep_unneeded_statics --plain_char=unsigned --silicon_version=msp --printf_support=minimal memb.c
cl430 --fp_reassoc=off --include_path="%CCEPATH%include" --sat_reassoc=off --symdebug:dwarf --keep_unneeded_statics --plain_char=unsigned --silicon_version=msp --printf_support=minimal telnetd.c
cl430 --fp_reassoc=off --include_path="%CCEPATH%include" --sat_reassoc=off --symdebug:dwarf --keep_unneeded_statics --plain_char=unsigned --silicon_version=msp --printf_support=minimal telnetd_shell.c
cl430 --fp_reassoc=off --include_path="%CCEPATH%include" --sat_reassoc=off --symdebug:dwarf --keep_unneeded_statics --plain_char=unsigned --silicon_version=msp --printf_support=minimal uip.c
cl430 --fp_reassoc=off --include_path="%CCEPATH%include" --sat_reassoc=off --symdebug:dwarf --keep_unneeded_statics --plain_char=unsigned --silicon_version=msp --printf_support=minimal uip_arch.c
cl430 --fp_reassoc=off --include_path="%CCEPATH%include" --sat_reassoc=off --symdebug:dwarf --keep_unneeded_statics --plain_char=unsigned --silicon_version=msp --printf_support=minimal uip_arp.c

cl430 --fp_reassoc=off --include_path="%CCEPATH%include" --sat_reassoc=off --symdebug:dwarf --keep_unneeded_statics --plain_char=unsigned --silicon_version=msp --printf_support=minimal main.c
cl430 -z main.obj terminal.obj app.obj RTL8019.obj nic.obj cgi.obj fs.obj httpd.obj telnetd.obj memb.obj telnetd_shell.obj uip.obj uip_arch.obj uip_arp.obj --reread_libs --warn_sections --search_path="%CCEPATH%lib" --search_path="%CCEPATH%include"  --library=%CMD% --library="rts430.lib" --rom_model --heap_size=0 --stack_size=50 -o uip.cof




rem hex430.exe -i %FILE%.cof -o %FILE%.hex -order MS -romwidth 16

rem del *.asm
del *.obj
del *.pwi
del *.dbk