#include <iostream>
#include <vector>
#include "util.h"

template <typename T>
void util::add(std::vector<T>& n1, const std::vector<T>& n2, int base)
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

vec_int util::convert_to_base10(const char* num_str)
{
    vec_int b10;
    for (int i = 0; num_str[i] != '\0'; i++) 
        b10.push_back(num_str[i] - '0');

    return b10;
}

vec_int util::convert_to_base10(unsigned long num)
{
    int loc = num;
    vec_int b10;

    while(loc > 0) {
        b10.insert(b10.begin(), 1, loc%10);
        loc = loc/10;
    }

    return b10;
}

vec_int util::convert_to_base10(const std::vector<bool>& bin_num)
{
    vec_int res;
    int sz = bin_num.size();
    
    for (int i = sz - 1; i >= 0; i--) {
        if (bin_num[i] == 1) {
            int power = sz - 1 - i;
            vec_int p2;
            power_of_2(power, p2); 
            util::add(res, p2, 10);
        }
    }

    if (res.size() == 0)
        return {0};

    return res;
}

void util::power_of_2(uint32_t exponent, vec_int& pow)
{
    pow = {1};
    vec_int tmp;

    if (exponent == 0)
        return;

    for (uint32_t j = 0; j < exponent; j++) {
        uint8_t carry = 0;
        for (int32_t i = pow.size() - 1; i >= 0; i--) {
            uint8_t t = pow[i] * 2 + carry;
            tmp.insert(tmp.begin(), t%10);
            carry = t/10;
        }
        if (carry != 0)
            tmp.insert(tmp.begin(), carry);
        pow = tmp;
        tmp.clear();
    }
}

