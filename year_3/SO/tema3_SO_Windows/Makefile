# Papa Florin, 334CA
# Tema 3 SO

build: vmsimx.obj common_winx.obj
	link /nologo /dll /out:libvmsim.dll /implib:libvmsim.lib vmsim.obj common_win.obj

vmsimx.obj: 
	cl /c vmsim.c

common_winx.obj:
	cl /c common_win.c
	
clean:
	del *.obj *.dll *.lib *.exp