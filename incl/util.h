#include <iostream>
#include <vector>
#include "base_types.h"
//#include "base2.h"

#ifndef UTIL_H
#define UTIL_H

namespace util 
{
template <typename T>
    void add(std::vector<T>& n1, const std::vector<T>& n2, int base);

    vec_int convert_to_base10(const char* num_str);
    vec_int convert_to_base10(unsigned long num);
    vec_int convert_to_base10(const bool_vec& bin_num);
    void power_of_2(uint32_t exponent, vec_int& pow);
}

#endif
