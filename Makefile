
INCL=incl/base10.h incl/base2.h incl/util.h
SRC=src/base10.cpp src/base2.cpp src/util.cpp
OBJS=base10.o base2.o util.o
TST_SRC=op_test.cpp
CC=g++ -c 

b2.a:$(SRC) $(INCL)
	$(CC) -Iincl/ $(SRC) 
	$(AR) r b2.a $(OBJS) 
