# Papa Florin, 334CA
# Tema 4 SO

build: utils.obj so_scheduler.obj
	link /nologo /dll /out:libscheduler.dll /implib:libscheduler.lib utils.obj so_scheduler.obj
	
utils.obj:
	cl /c utils.c

so_scheduler.obj:
	cl /c so_scheduler.c

clean:
	del *.obj *.dll *.lib *.exp