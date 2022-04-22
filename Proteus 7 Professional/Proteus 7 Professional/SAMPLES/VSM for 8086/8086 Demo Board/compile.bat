@echo on
del *.obj
del *.exe
del *.map
ml /Zm /Zd /Zi /Zf -c RTL.ASM
dmc -0 -g -ms -0 -c -Ic:\compile\dm\INCLUDE  -oboard.obj board.c
dmc -0 -g -ms -0 -c -Ic:\compile\dm\INCLUDE  -ocalc.obj calc.c
link /CO /NOD /DEB /DEBUGB  /STACK:1024 calc.obj+board.obj+rtl.obj+math.lib
