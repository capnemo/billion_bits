# Billion Bits
Arbitrary precision library. Implements base 2 and base 2^63 arithmetic.  
The following operations are supported  
Addition  
Subtraction  
Multiplication  
Division  
Modulo  
A base2 object has a range of +-2^9223372036854775744 and a base63 object has a range of +-(2^63)^2305843009213693951 on a 64 bit linux/unix machine.
### Building the software
git clone the software to your machine.  
In the billion_bits directory, run make.  
The file bin/b2.a is the library archive.  
The base 2 api is in the file incl/base2.h and the base 2^63 api is in the file incl/base63.h

### Tests
reg_tests/test_all.sh will run tests on base 2 and base 2^63 for all the operators.  
The file reg_tests/operator_drv.cpp is instructive in the usage of this library.

### Dependencies
a) C++ 11 or greater.  
b) Linux or Macos.  
c) Clang version 8 or greater.  
d) Yasm 1.3.0 or greater.  
