#include <iostream>
#include <vector>
#include "base_types.h"

#ifndef UTIL_H
#define UTIL_H

/*
 * Utility functions for base2 and base10 classes.
 */ 
namespace util 
{
    template <typename T> 
    void add(std::vector<T>& n1, const std::vector<T>& n2, int base);

    vec_int convert_to_base10(const char* num_str);
    vec_int convert_to_base10(unsigned long num);
    void multiply_with_2_base10(const vec_int& num, vec_int& out);
    vec_int convert_to_base10(const bool_vec& bin_num);
    bool_vec convert_to_bits(unsigned char digit);
    void power_of_2(uint32_t exponent, vec_int& pow);
    bool is_valid_num(const std::string& num);
    uint8_t get_trailing_zeros(uint64_t);

    template <typename T>
    void raise_to(T& num, uint64_t exponent);
}

#endif
