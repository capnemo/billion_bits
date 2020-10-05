INCL=incl/base10.h incl/base2.h incl/util.h
SRC=src/base10.cpp src/base2.cpp src/util.cpp
OBJS=base10.o base2.o util.o
TST_SRC=op_test.cpp
CC=g++ -ggdb

all:b2.a test

b2.a:$(SRC) $(INCL)
	$(CC) -c -Iincl/ $(SRC) 
	$(AR) r b2.a $(OBJS) 


test:tests/op_test tests/add_test tests/disp_test
tests/%:tests/%.cpp b2.a
	$(CC) -Iincl/ tests/$*.cpp b2.a -o tests/$*

clean:
	rm *.o b2.a tests/op_test tests/add_test tests/disp_test
