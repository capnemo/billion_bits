#include <iostream>
#include <vector>
#include "base_types.h"
//#include "base2.h"

#ifndef UTIL_H
#define UTIL_H

namespace util 
{
    template <typename T> //Should be moved to the .cpp file.
    void add(std::vector<T>& n1, const std::vector<T>& n2, int base)
    {
        if (n1.size() < n2.size()) 
            n1.insert(n1.begin(), n2.size() - n1.size(), 0);
     
        int n1_pos = n1.size() - 1;
        int carry = 0;
        for (int i = n2.size() - 1; i >= 0; i--) {
            int t = n1[n1_pos] + n2[i] + carry;
            n1[n1_pos--] = t%base;
            carry = t/base;
        }
    
        while (n1_pos >= 0) {
            int t = n1[n1_pos] + carry;
            n1[n1_pos--] = t%2;
            carry = t/2;
        }
    
        if (carry != 0)
            n1.insert(n1.begin(), 1, carry);
    }

    vec_int convert_to_base10(const char* num_str);
    vec_int convert_to_base10(unsigned long num);
    vec_int convert_to_base10(const bool_vec& bin_num);
    void power_of_2(uint32_t exponent, vec_int& pow);
}

#endif
