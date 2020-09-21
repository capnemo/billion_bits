#include <iostream>
#include <vector>
#include "util.h"
#include "base_types.h"

#ifndef BASE10_H
#define BASE10_H
class base10 {

    public:

    static vec_int convert_to_base10(const char* num_str);
    static vec_int convert_to_base10(unsigned long num);
    static vec_int convert_to_base10(const bool_vec& bin_num);
    static void power_of_2(uint32_t exponent, vec_int& pow);

    base10() {};
    base10(unsigned long num):digits(convert_to_base10(num)) {}
    base10(const char* n_str):digits(convert_to_base10(n_str)) {}
    base10(const bool_vec& b_vec):digits(convert_to_base10(b_vec)) {}

    void print();
    private:
    std::vector<int> digits = {0};
};
#endif
