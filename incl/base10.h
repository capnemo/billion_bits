/*
 *  Class to print in base 10 format.
 */

#include <iostream>
#include <vector>
#include "util.h"
#include "base_types.h"

#ifndef BASE10_H
#define BASE10_H
class base10 {

    public:
    base10() {};
    base10(unsigned long num, bool sign = false) : 
                            digits(util::convert_to_base10(num)),
                            is_negative(sign) {}
    base10(const char* n_str, bool sign = false) : 
                            digits(util::convert_to_base10(n_str)),
                            is_negative(sign) {}
    base10(const bool_vec& b_vec, bool sign = false) : 
                            digits(util::convert_to_base10(b_vec)),
                            is_negative(sign) {}

    void print();
    private:
    vec_int digits = {0}; 
    bool is_negative;
};
#endif
