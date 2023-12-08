#include <iostream>
#include <vector>
#include <bitset>
#include "util.h"
#include "base2.h"
#include "base63.h"

/*
 * Template function to add using different bases. Used for base 2 and
 * base 10.
 * computes n1 += n2
 */
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
        n1[n1_pos--] = t%base;
        carry = t/base;
    }

    if (carry != 0)
        n1.insert(n1.begin(), 1, carry);
}

/*
 * converts num_str to base 10.
 */
vec_int util::convert_to_base10(const char* num_str)
{
    vec_int b10;
    for (int i = 0; num_str[i] != '\0'; i++) 
        b10.push_back(num_str[i] - '0');

    return b10;
}

/*
 * converts num to base 10
 */
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

void util::multiply_with_2_base10(const vec_int& num, vec_int& out)
{
    out.clear();
    int carry = 0;
    for (int i = num.size() - 1; i >= 0; i--) {
        int t = 2*num[i] + carry;
        out.insert(out.begin(), t%10);
        carry = t/10;
    }
    if (carry != 0) 
        out.insert(out.begin(), carry);
}

vec_int util::convert_to_base10(const bool_vec& bin_num)
{
    int sz = bin_num.size();
    std::vector<vec_int> powers_of_2 = {{1}};

    for (int i = 1; i < sz; i++) {
        vec_int p;
        multiply_with_2_base10(powers_of_2[i - 1], p);
        powers_of_2.push_back(p);
    }
    
    vec_int res;
    for (int i = 0; i < bin_num.size(); i++) {
        if (bin_num[i] == 1) 
            util::add(res, powers_of_2[(sz -1) - i], 10);
    }

    if (res.size() == 0)
        res.push_back(0);

    return res;
}
#if 0
/*
 * converts binary to base 10
 */
vec_int util::convert_to_base10(const bool_vec& bin_num)
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

/*
 * converts binary to base 10.
 */
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
#endif
/*
 * Returns if num is a valid base 10 number.
 */
bool util::is_valid_num(const std::string& num)
{
    for (auto m:num) 
        if ((m < '0') || (m > '9'))
            return false;

    return true;
}

/*
 * Returns the base 2 form of the base 10 number in digit
 * Remember, msbit is at index 0 in the returned vector.
 */
bool_vec util::convert_to_bits(unsigned char digit)
{
    std::bitset<4> bt = digit;
    bool_vec num(4, 0);
    
    for (int i = 0; i < 4; i++) 
        num[3 - i] = bt[i];

    int i = 0;
    while((num[i++] == 0) && (i <= 3));
    num.erase(num.begin(), num.begin() + i - 1);
    return num; 
}

/*
 * Returns the number of trailing zeros in the number
 */
 
uint8_t util::get_trailing_zeros(uint64_t num)
{
    uint64_t mask = 1ULL;
    uint8_t zeros = 0;
    while ((mask & num) == 0) {
        zeros++;
        mask = mask << 1;
    }
    
    return zeros;
}

template <typename T>
void util::raise_to(T& num, uint64_t exponent)
{
    uint64_t mask = 1ULL << 63;
    uint8_t pos = 63;
    while ((mask & exponent) == 0) {
        pos--;
        mask = mask >> 1;
    }

    std::vector<T> powers;
    powers.push_back(num); 
    for (int i = 1; i <= pos; i++) {
        T n = powers[i - 1];
        T m = n;
        m.multiply_with(n);
        powers.push_back(m);
    }

    std::bitset<64> bits = exponent;
    T pow;
    for (int i = 0; i <= pos; i++) {
        if (bits[i] == true)
            pow.multiply_with(powers[i]);
    }
    
    num = pow;   
}

template void util::add<bool>(bool_vec&, const bool_vec&, int);
template void util::add<int>(vec_int&, const vec_int&, int);
template void util::raise_to<base2>(base2& num, uint64_t exponent);
template void util::raise_to<base63>(base63& num, uint64_t exponent);
