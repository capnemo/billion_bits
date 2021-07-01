INCL=incl/base10.h incl/base2.h incl/base63.h incl/util.h
SRC=src/base10.cpp src/base2.cpp src/base63.cpp src/util.cpp
OBJS=bin/base10.o bin/base2.o bin/base63.o bin/util.o bin/mul63.o
TEST_BINS=reg_tests/operator_drv reg_tests/conv_drv
CC=clang++ -std=c++11 -ggdb
#CC=clang++ -ggdb
#CC=g++
#TODO dependencies have to be added. 

MUL_ASM=src/mul63.asm
ASM_FLAGS=-felf64 -gdwarf2
os_name=$(shell uname -s)
ifeq ($(os_name),Darwin)
	MUL_ASM=src/mul63_darwin.asm
	ASM_FLAGS=-f macho64
endif

all:src_cmp test

release: CC:=clang++ -Ofast -ggdb
release: all

bin/b2.a:$(OBJS)
	ar r bin/b2.a $(OBJS)

src_cmp:bin
bin/%.o:src/%.cpp 
	$(CC) -c -Iincl $^ -o $@

bin:
	mkdir -p bin 

bin/mul63.o:src/mul63.asm 
	yasm $(ASM_FLAGS) $(MUL_ASM) -o bin/mul63.o

test:$(TEST_BINS)
reg_tests/%:reg_tests/%.cpp bin/b2.a 
	$(CC) -Iincl/ $^ -o $@

clean:
	rm -rf bin $(TEST_BINS)
