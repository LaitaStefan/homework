#Papa Florin, 314CA

CC = g++
CCFLAGS = -Wall
SRC = main.cc skip.cc
PROGRAM = skiplist

#Regula build
build:
	$(CC) -o $(PROGRAM) $(SRC) $(CCFLAGS)

#Regulile de "curatenie"
.PHONY : clean
clean:
	rm -f $(PROGRAM) core *~
