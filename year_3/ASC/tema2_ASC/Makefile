CC=gcc
SRC=using_cblas.c
LDFLAGS=-lcblas -lm -latlas

OPTDIR=/opt/tools/libraries/atlas/3.10.1-opteron-gcc-4.4.6
NEHDIR=/opt/tools/libraries/atlas/3.10.1-nehalem-gcc-4.4.6
QUADDIR=/opt/tools/libraries/atlas/3.10.1-quad-gcc-4.4.6

build: dtrmv_o dtrmv_n dtrmv_q basic improved

basic:
	$(CC) -o basic dummy_dtrmv.c -O3
dtrmv_o:
	$(CC) -o dtrmv_o $(SRC) -L$(OPTDIR)/lib -I$(OPTDIR)/include $(LDFLAGS)
dtrmv_n:
	$(CC) -o dtrmv_n $(SRC) -L$(NEHDIR)/lib -I$(NEHDIR)/include $(LDFLAGS)
dtrmv_q:
	$(CC) -o dtrmv_q $(SRC) -L$(QUADDIR)/lib -I$(QUADDIR)/include $(LDFLAGS)
improved:
	$(CC) -o improved improved_dtrmv.c -O3 -funroll-loops

clean:
	rm -rf basic improved dtrmv_o dtrmv_q dtrmv_n
