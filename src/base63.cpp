#include <bitset>
#include "base10.h"
#include "base63.h"

extern "C" void mul_dq(unsigned long* a1, unsigned long* a2);

/*
 * Converts num_str to base 63 number.
 */
base63 base63::convert_to_b63(const std::string& num_str) 
{   
    std::string abs_num = num_str;
    bool is_less_than_zero = false;
    if (num_str[0] == '-') {
        abs_num = num_str.substr(1);
        is_less_than_zero = true;
    }

    bool_vec b2_bits = base2::convert_to_binary(abs_num);
    base2 b2_num(b2_bits, is_less_than_zero);
    
    return convert_to_b63(b2_num);
}

/*
 * Converts a series of exponents of 2 in exponents to a base 63 number.
 */
base63 base63::convert_to_base63(const vec_int& exponents)
{
    ulong mask = 1;
    int max = 0;
    for (auto m:exponents)
        if (m > max)
            max = m;

    while (max%63 != 0)
        max++;

    int vec_sz = max/63 + 1;
    b63_vec b63_num(vec_sz, 0);
    for (auto m:exponents) {
        ulong& dig = b63_num[vec_sz - 1 - m/63];
        dig = dig | (mask << (m%63));
    }

    return base63(b63_num);
}

/*
 * Converts a base 2 number to a base 63 number.
 */
base63 base63::convert_to_b63(const base2& b2_num) 
{
    base2 b2 = b2_num;
    int bit_sz = b2.get_size();
    
    while (bit_sz%63 != 0)
        bit_sz++;

    if (bit_sz - b2.get_size() > 0)
        b2.shift_right(bit_sz - b2.get_size());

    bool_vec b2_bits = b2.get_bits();
    b63_vec b63_digits;
    for (int i = 0; i < b2_bits.size(); i += 63) {
        std::string num_str;
        for (int j = i; j < i + 63; j++)
            num_str += b2_bits[j] + '0';
        b63_digits.push_back(std::bitset<63>(num_str).to_ullong());
    }
    
    return base63(b63_digits);
}

/*
 * Adds addend to *this. 
 */
/* should be vectorized */
void base63::add_to(const base63& addend)
{
    int local_sz = get_size();
    int addend_sz = addend.get_size();
    int i = local_sz - 1;
    int j = addend_sz - 1;
    b63_vec addend_bits = addend.get_bits();

    while ((i >= 0) && (j >= 0)) 
        dig_vec[i--] += addend_bits[j--];

    if (local_sz < addend_sz) {
        int diff = addend_sz - local_sz;
        b63_vec::iterator addend_begin = addend_bits.begin();
        dig_vec.insert(dig_vec.begin(), addend_begin, 
                        addend_begin + diff);
    }

    int carry = 0;
    for (int i = dig_vec.size() - 1; i >= 0; i--) {
        if (carry != 0)
            dig_vec[i]++;
        if (dig_vec[i] & msb_on_mask) {
            carry = 1;
            dig_vec[i] = msb_off_mask & dig_vec[i];
        } else 
            carry = 0;
    }
    
    if (carry == 1)  {
        dig_vec[0] = msb_off_mask & dig_vec[0];
        dig_vec.insert(dig_vec.begin(), 1);
    }
}

/*
 * Multiplies multiplicand with *this. 
 */
void base63::multiply_with(const base63& multiplicand)
{
    b63_vec m_bits = multiplicand.get_bits();
    int l_sz = dig_vec.size();
    int m_sz = m_bits.size();
    base63 res(b63_vec(m_sz + l_sz, 0));

    for (int i = 0; i < l_sz; i++) {
        if (dig_vec[i] == 0)
            continue;
        for (int j = 0; j < m_sz; j++) {
            if (m_bits[j] == 0)
                continue;
            ulong prod[2] = {dig_vec[i], 0};
            ulong arg2 = m_bits[j];
            mul_dq(prod, &arg2);
            b63_vec prod_arr;
            if (prod[0] != 0)  
                prod_arr.push_back(prod[0]); 
            prod_arr.push_back(prod[1]);
            prod_arr.insert(prod_arr.end(), 
                            l_sz  - 1 - i + m_sz  - 1 - j, 0);
            res.add_to(prod_arr);
        }
    }
    
    dig_vec = res.get_bits();
    trim_leading_zeros();
}

/*
 * Removes all leading zeros from dig_vec.
 */
void base63::trim_leading_zeros()
{
    int i = 0;
    while ((dig_vec[i] == 0) && (i < dig_vec.size()))
        i++;
    dig_vec.erase(dig_vec.begin(), dig_vec.begin() + i);

    if (dig_vec.size() == 0)
        dig_vec.push_back(0);
}

/*
 * Print in base 10.
 */
void base63::print_base10()
{
    bool_vec b2;
    for (auto m:dig_vec) {
        std::string str = std::bitset<63>(m).to_string();
        for (auto n:str)
            b2.push_back(n - '0');
    }
    base10(b2).print();
}

/*
 * Prints in base 2. If sep == true, prints ':' between every 63 bits.
 */
void base63::print_bits(bool sep)
{
    std::string msb = std::bitset<63>(dig_vec[0]).to_string();
    if (sep == false) {
        int i = 0;
        while (msb[i] == '0')
            i++;
        msb.erase(0, i);
    }
    std::cout << msb;
    if (sep == true)
        std::cout << ":" << std::endl;

    for (int i = 1; i < dig_vec.size(); i++) {
        std::cout << std::bitset<63>(dig_vec[i]);
        if (sep == true)
            std::cout << ":" << std::endl; 
    }

    if (sep == false)
        std::cout << std::endl;
}
