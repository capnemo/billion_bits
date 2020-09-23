#include <iostream>
#include <vector>
#include "util.h"
#include "base2.h"
#include "base10.h"

//TODO Signedness. All over from add_to print.
//TODO Sign.. display
//TODO base2 should also print in base 10. Done.
//TODO Trim leading zeros. After subtract_from. Done
//TODO Should be able to take an exponent of 2 and generate the binary number. 
//     Done.
//TODO divide_by. Done.
//TODO Remainder and divide should call divide_by Done.
//TODO All relational operators should use std::vector::relational operator
//     Doesnt work! Should be investigated, perhaps.
//TODO Tests for modulo. Done
//TODO Cannot handle input strings >= 2^63
//TODO Move all the static functions to the util namespace.

bool_vec n10 = {1,0,1,0};
static base2 num_10(n10);

bool_vec n0 = {0};
static base2 zero(n0);

bool_vec one = {1};
static base2 unity(one);

//Cannot handle 2^63 BUG!!
bool_vec base2::convert_to_binary(unsigned long num)
{
    bool_vec bits;
    unsigned long mask = 1L;

    while (mask <= num) {
        bool l_bit = 0;
        if (mask & num)
            l_bit = 1;
        bits.push_back(l_bit);
        mask = mask << 1;
    }

    int last = bits.size() - 1;
    for (int i = 0; i < bits.size()/2; i++) {
        bool t = bits[i];
        bits[i] = bits[last - i];
        bits[last - i] = t;
    }

    return bits;
}
    
//Cannot handle signs. BUG!!
bool base2::is_greater_than(const base2& num) const
{
    if (get_size() != num.get_size()) 
        return (get_size() > num.get_size()) ? true:false;

    int i = 0;
    int j = 0;
    while ((i < get_size()) && (j < get_size()) && 
            (at(i++) == num.at(j++)));

    return (at(i - 1) > num.at(j - 1)) ? true:false;
}

void base2::add_to(const base2& addend)
{
    if (is_negative != addend.less_than_zero()) {
        bool sign = is_negative;
        if (addend.is_greater_than(*this) == true) 
            sign = addend.less_than_zero();
        
        is_negative = false;
        bool_vec b = addend.get_bits();
        base2 t(b);
        subtract_from(t);
        is_negative = sign;
        if (is_zero() == true)
            is_negative = false;
        return;
    }

    bool_vec a_bits = addend.get_bits();
    util::add(bit_rep, a_bits, 2);
}


void base2::subtract_from(const base2& subtrahend)
{
    base2 t = subtrahend;
    t.shift_right(get_size() - t.get_size());
    t.flip();
    t.increment();

    int sz = get_size();
    add_to(t);
    if (get_size() > sz)
        bit_rep[0] = 0;
    
    trim_left();
    
    if (is_zero() == true)
        is_negative = false;
}

bool base2::is_zero()
{
    if ((get_size() == 1) && (bit_rep[0] == 0))
        return true;

    return false;
}

void base2::trim_left()
{
    int i = 0;
    while (at(i++) == 0);
    
    bit_rep.erase(bit_rep.begin(), bit_rep.begin() + i - 1);
    if (bit_rep.size() == 0)
        bit_rep.push_back(0);
}

void base2::trim_right(int places)
{
    bit_rep.erase(bit_rep.end() - places, bit_rep.end());
}

void base2::increment()
{
    bool_vec b(1,1);
    base2 t(b);
    add_to(t);
}

base2 base2::get_exponent_bin(int exp)
{
    bool_vec exp_bin(exp + 1, 0);
    exp_bin[0] = 1;

    return exp_bin;
}

void base2::multiply_with(const base2& multiplicand)
{
    base2 res = get_zero_value(multiplicand.get_size() + bit_rep.size());
    bool_vec m_bits;
    multiplicand.set_bits(m_bits);
    
    for (int i = m_bits.size() - 1; i >= 0; i--) {
        if (m_bits[i] == 1) {
            base2 t(bit_rep);
            t.shift_left(m_bits.size() - 1 - i);
            res.add_to(t);
        }
    }

    is_negative = (is_negative == multiplicand.less_than_zero()) ? false:true;

    res.set_bits(bit_rep);
    trim_left();
}

base2 base2::get_modulo(const base2& divisor)
{
    base2 dividend(bit_rep);

    return dividend.divide(divisor);
}

base2 base2::divide(const base2& divisor)
{
    is_negative = (is_negative == divisor.less_than_zero()) ? false:true;

    if (is_equal_to(divisor) == true) {
        bit_rep = one;
        return zero;
    }

    if (is_less_than(divisor) == true) {
        bit_rep = n0;
        return divisor; 
    }

    base2 remainder = *this;
    bool_vec q_bits(get_size() - divisor.get_size() + 1, 0);
    base2 quotient(q_bits);
    do {
        base2 t = divisor;
        int shift_offset = remainder.get_size() - t.get_size();
        t.shift_left(shift_offset);
        if (t.is_greater_than(remainder) == true) {
            shift_offset--;
            t.trim_right(1);
        }
        remainder.subtract_from(t);
        quotient.add_exponent(shift_offset);
    } while (remainder.is_greater_than(divisor) == true);
    
    if (remainder.is_equal_to(divisor) == true) {
        quotient.increment();
        remainder = zero;
    }

    bit_rep = quotient.get_bits();
    trim_left();

    return remainder;
}

void base2::add_exponent(int ex)
{
    int b_ex = get_size() - ex - 1;
    if (bit_rep[b_ex] == 1)
        shift_left(1);
    else
        bit_rep[b_ex] = 1;
}

bool_vec base2::convert_to_binary(const char* num_str)
{
    unsigned long num = std::strtoull(num_str, nullptr, 10);
    bool_vec num_bits = convert_to_binary(num);

    return num_bits;
}

bool_vec base2::convert_to_binary(const std::string& num_str)
{
    base2 dst = zero;
    int last = num_str.size() - 1;
    int i = last;
    base2 ten_exp = one;

    while (i >= 0) {
        base2 bin_digit = convert_to_bits(num_str[i] - '0');
        bin_digit.multiply_with(ten_exp);
        dst.add_to(bin_digit);
        ten_exp.multiply_with(n10);
        i--;
    }

    return dst.get_bits();
}

bool base2::validate(const std::string& num)
{
    for (auto m:num) 
        if ((m < '0') || (m > '9'))
            return false;

    return true;
}

base2 base2::convert_to_bits(unsigned char digit)
{
    unsigned mask = 128;
    bool_vec num;
    
    while (mask > 0) {
        bool b = (digit & mask) ? 1:0;
        num.push_back(b);
        mask = mask >> 1;
    }

    return base2(num);   
}

void base2::print_bits() const
{
    if (is_negative == true)
        std::cout << "-";
    for (auto m:bit_rep)
        std::cout << m;

    std::cout << std::endl;
}

void base2::print_base10() const
{
    base10 b10(bit_rep, is_negative);
    b10.print();
}
