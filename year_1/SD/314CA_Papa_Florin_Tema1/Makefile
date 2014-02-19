#Papa Florin, 314CA

CC = g++
CCFLAGS = -Wall
SRC = main.cc list.cc list1.cc queue.cc queue1.cc
PROGRAM = sched

#Regula build
build:
	$(CC) -o $(PROGRAM) $(SRC) $(CCFLAGS)

#Regulile de "curatenie"
.PHONY : clean
clean:
	rm -f $(PROGRAM) core *~
