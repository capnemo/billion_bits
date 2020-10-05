#include <iostream>
#include "base2.h"
#include "base10.h"

int main(int argc, char *argv[])
{
    if (argc != 2) {
        std::cout << "error -- args" << std::endl;
        return -1;
    }
 
    std::string arg(argv[1]);
    if (base2::validate(arg) == true) {
        base2 bin_num(arg);
        bin_num.print_bits();
    }   
}

