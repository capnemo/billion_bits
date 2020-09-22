
INCL=incl/base10.h incl/base2.h incl/util.h
SRC=src/base10.cpp src/base2.cpp src/util.cpp
OBJS=base10.o base2.o util.o
TST_SRC=op_test.cpp
CC=g++ -ggdb

all:b2.a op_test add_test disp_test

#If one file changes, all files will be built.
b2.a:$(SRC) $(INCL)
	$(CC) -c -Iincl/ $(SRC) 
	$(AR) r b2.a $(OBJS) 

#No need for a line for every test file. 
op_test: b2.a test/op_test.cpp
	$(CC) -Iincl/ test/op_test.cpp b2.a -o op_test

add_test: b2.a test/add_test.cpp
	$(CC) -Iincl/ test/add_test.cpp b2.a -o add_test
	
disp_test: b2.a test/disp_test.cpp
	$(CC) -Iincl/ test/disp_test.cpp b2.a -o disp_test

clean:
	rm *.o b2.a op_test add_test disp_test
