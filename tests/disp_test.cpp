#include <iostream>
#include "base2.h"
#include "base10.h"

int main(int argc, char *argv[])
{
    if (argc != 2) {
        std::cout << "error -- args" << std::endl;
        return -1;
    }
 
    char *inp = argv[1];
    bool is_negative = false;
    if (inp[0] == '-') {
        inp++;
        is_negative = true;
    }

    std::string arg(inp);
    if (base2::validate(arg) == true) {
        base2 bin_num(arg, is_negative);
        bin_num.print_bits();
    }   
}

