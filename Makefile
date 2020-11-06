INCL=incl/base10.h incl/base2.h incl/base63.h incl/util.h
SRC=src/base10.cpp src/base2.cpp src/base63.cpp src/util.cpp
OBJS=bin/base10.o bin/base2.o bin/base63.o bin/util.o bin/mul63.o
TEST_BINS=tests/op_test tests/add_test tests/disp_test tests/b63_test tests/b63_test_str_disp
CC=g++ -ggdb
#CC=clang++ -ggdb

all:bin/b2.a test

bin/b2.a:$(OBJS)
	ar r bin/b2.a $(OBJS)

bin/%.o:bin src/%.cpp $(INCL)
	$(CC) -c -Iincl src/$*.cpp  -o bin/$*.o

bin:
	mkdir -p bin

bin/mul63.o:src/mul63.asm
	yasm -felf64 -gdwarf2 src/mul63.asm -o bin/mul63.o

test:$(TEST_BINS)
tests/%:tests/%.cpp bin/b2.a $(INCL)
	$(CC) -Iincl/ tests/$*.cpp bin/b2.a -o tests/$*

clean:
	rm -rf bin *.o b2.a tests/op_test tests/add_test tests/disp_test
