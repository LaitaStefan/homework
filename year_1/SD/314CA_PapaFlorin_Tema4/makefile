#Papa Florin, 314CA

CC = g++
CCFLAGS = -Wall
SRC = main.cc graph.cc scheduler.cc
PROGRAM = tema4

#Regula build
build:
	$(CC) -o $(PROGRAM) $(SRC) $(CCFLAGS)

#Regulile de "curatenie"
.PHONY : clean
clean:
	rm -f $(PROGRAM) core *~
