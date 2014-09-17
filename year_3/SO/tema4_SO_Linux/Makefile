# Papa Florin, 334CA
# Tema 4 SO

build: utils.o so_scheduler.o
	gcc -shared utils.o so_scheduler.o -o libscheduler.so
	cp libscheduler.so ../tema4-checker-lin/_test/
	cp libscheduler.so ../tema4-checker-lin/

utils.o:
	gcc -fPIC -c utils.c

so_scheduler.o:
	gcc -fPIC -c so_scheduler.c

clean:
	rm -rf *.o *.so