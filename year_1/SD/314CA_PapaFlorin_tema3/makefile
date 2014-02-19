#Papa Florin, 314CA

CC = g++
CCFLAGS = -Wall
SRC = tema3.cc huff.cc heap.cc
PROGRAM = mycompbmp

#Regula build
build:
	$(CC) -o $(PROGRAM) $(SRC) $(CCFLAGS)

#Regulile de "curatenie"
.PHONY : clean
clean:
	rm -f $(PROGRAM) core *~