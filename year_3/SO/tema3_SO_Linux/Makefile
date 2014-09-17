# Papa Florin, 334CA
# Tema 3 SO

build: vmsim.o
	gcc -shared vmsim.o -o libvmsim.so

vmsim.o:
	gcc -fPIC -c vmsim.c
	
clean:
	rm -rf *.o *.so
