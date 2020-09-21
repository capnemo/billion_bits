#include <iostream>
#include <vector>
#include "util.h"
#include "base10.h"


using vec_int = std::vector<int>;

vec_int base10::convert_to_base10(const char* num_str)
{
    vec_int b10;
    for (int i = 0; num_str[i] != '\0'; i++) 
        b10.push_back(num_str[i] - '0');

    return b10;
}

vec_int base10::convert_to_base10(unsigned long num)
{
    int loc = num;
    vec_int b10;

    while(loc > 0) {
        b10.insert(b10.begin(), 1, loc%10);
        loc = loc/10;
    }

    return b10;
}

vec_int base10::convert_to_base10(const std::vector<bool>& bin_num)
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

void base10::power_of_2(uint32_t exponent, vec_int& pow)
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

void base10::print()
{
    for (auto m:digits)
        std::cout << m;

    std::cout << std::endl;
}
