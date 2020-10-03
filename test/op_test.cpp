#include <iostream>
#include "base2.h"

bool is_less_than_zero(std::string& num);

int main(int argc, char *argv[])
{
    if (argc != 4) {
        std::cout << "error -- args" << std::endl;
        return -1;
    }
    
    std::string op = argv[1];
    std::string ns1(argv[2]), ns2(argv[3]);
    
    bool s1 = is_less_than_zero(ns1);
    bool s2 = is_less_than_zero(ns2);
    base2 a1(ns1, s1), a2(ns2, s2);
    bool res_valid = true;

    if (op == "-a") {
        a1.sum(a2);
    } else if (op == "-m") {
        a1.multiply_with(a2);
    } else if (op == "-s") {
        a1.difference(a2);
    } else if (op == "-d") {
        a1.divide_by(a2);
    } else if (op == "-r") {
        a1 = a1.get_modulo(a2);
    } else if (op == "-g") {
        std::cout << std::boolalpha << a1.is_greater_than(a2)
                  << std::endl;
        res_valid = false;
    } else {
        std::cout << "error -- unsupported op" << std::endl;
        return -1;
    } 

    if (res_valid == true) { 
        a1.print_base10();
        a1.print_bits();
    }
}

bool is_less_than_zero(std::string& num)
{
    bool s = false;
    if (num[0] == '-') {
        num = num.substr(1, num.size() - 1);
        s = true;
    }

    return s;
}
