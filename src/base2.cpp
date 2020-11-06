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
//TODO Corner and unity cases in addition, and subtraction.

bool_vec n10 = {1,0,1,0};
static base2 num_10(n10);

bool_vec n0 = {0};
static base2 zero(n0);

bool_vec one = {1};
static base2 unity(one);

/*
 * Static function converts @num to bool_vec.
 * Cannot handle numbers > 2^63.
 * For numbers > 2^63, use the function with the string argument.
 */
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
 
/*
 * Returns true if *this > num.
 * Does not handle signs. Not required at this point.
 */   
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

/*
 * Sums *this and arg. Computes *this = *this + arg. 
 * sign aware.
 */
void base2::sum(const base2& arg)
{
    if (arg.is_zero() == true)
        return;

    if (is_zero() ==  true) {
        *this = arg;
        return;
    }

    if (less_than_zero() == arg.less_than_zero())  {
        util::add(bit_rep, arg.get_bits(), 2);
        return;
    }
        
    subtract_from(arg);
    if (arg.is_greater_than(*this) == true)
        is_negative = arg.less_than_zero();
    
    if (is_zero() == true)
        is_negative = false;
}

/*
 * Find the difference between *this and arg.
 * Computes *this = *this - arg.
 * sign aware.
 */
void base2::difference(const base2& arg)
{
    bool_vec arg_bits = arg.get_bits();
    base2 m_arg(arg_bits, !arg.less_than_zero());
    sum(m_arg);
}

/*
 * Adds *this and addend. Computes *this += addend;
 * NOT sign aware.
 */
void base2::add_to(const base2& addend)
{
    bool_vec a_bits = addend.get_bits();
    util::add(bit_rep, a_bits, 2);
}

/*
 * Subtracts addend from *this. Computes *this -= subtrahend;
 * NOT sign aware.
 */
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
}

/*
 * Sets *this to one.
 */
void base2::set_unity()
{
    bit_rep = {1};
}

/*
 * Returns if *this equals 1.
 */
bool base2::is_unity() const
{
    if ((get_size() == 1) && (bit_rep[0] == 1))
        return true;

    return false;
}

/*
 * Sets *this to zero.
 */
void base2::set_zero()
{
    is_negative = false;
    bit_rep = {0};
}

/*
 * Returns if *this equals 0.
 */
bool base2::is_zero() const
{
    if ((get_size() == 1) && (bit_rep[0] == 0))
        return true;

    return false;
}

/*
 * Removes leading zeros.
 */
void base2::trim_left()
{
    int i = 0;
    while (at(i++) == 0);
    bit_rep.erase(bit_rep.begin(), bit_rep.begin() + i - 1);

    if (bit_rep.size() == 0) {
        bit_rep.push_back(0);
        is_negative = false;
    }
}

/*
 * Removes trailing zeros.
 */
void base2::trim_right(int places)
{
    bit_rep.erase(bit_rep.end() - places, bit_rep.end());
}

/*
 * Increments *this by 1.
 */
void base2::increment()
{
    bool_vec b(1,1);
    base2 t(b);
    add_to(t);
}

/*
 * Returns bool_vec corresponding to 2^exp
 */
base2 base2::get_exponent_bin(int exp)
{
    bool_vec exp_bin(exp + 1, 0);
    exp_bin[0] = 1;

    return exp_bin;
}

/*
 * Returns the number of ones in bit_rep.
 */
int base2::get_num_ones() const
{
    int n_r = 0;
    for (auto m:bit_rep)
        if (m == 1)
            n_r++;

    return n_r;
}

/*
 * Computes *this *= multiplicand.
 * Sign aware
 */
void base2::multiply_with(const base2& multiplicand)
{   
    is_negative = 
        (is_negative == multiplicand.less_than_zero()) ? false:true;

    if (is_zero() == true) 
        return;

    if (multiplicand.is_zero() == true) {
        set_zero();
        return;
    }

    if (is_unity() == true) {
        bit_rep = multiplicand.get_bits();
        return;
    }

    if (multiplicand.is_unity() == true) 
        return;

    bool_vec l_bits, g_bits;
    if (get_num_ones() < multiplicand.get_num_ones()) {
        l_bits = bit_rep;
        g_bits = multiplicand.get_bits();
    } else {
        l_bits = multiplicand.get_bits();
        g_bits = bit_rep;
    }
    
    bool_vec res(l_bits.size() + g_bits.size(), 0);
    int i = l_bits.size() - 1;
    int curr_pos = 0;
    int prev_pos = 0;
    for (;i >= 0; i--) {
        if (l_bits[i] == 1) {
            g_bits.insert(g_bits.end(), curr_pos - prev_pos, 0);
            util::add(res, g_bits, 2);
            prev_pos = curr_pos;
        }
        curr_pos++;
    }

    bit_rep = res;
    trim_left();
}

/*
 * Computes *this *= 10. Faster than multiply_with(10);
 */
void base2::multiply_with_ten()
{   
    if (is_unity() == true) {
        bit_rep = n10;
        return;
    }

    if (is_zero() == true)
        return;

    bit_rep.push_back(0);
    bool_vec addend = bit_rep;
    addend.insert(addend.end(), 2, 0);
    util::add(bit_rep, addend, 2);
}

/*
 * Returns *this%divisor
 */
base2 base2::get_modulo(const base2& divisor)
{
    base2 dividend(bit_rep);

    return dividend.divide(divisor);
}

/*
 * Computes *this /= divisor. 
 * Sign aware
 */
base2 base2::divide(const base2& divisor)
{
    is_negative = 
        (is_negative == divisor.less_than_zero()) ? false:true;

    if (divisor.is_unity() == true)
        return zero;

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

/*
 * Computes *this += 2^ex
 */
void base2::add_exponent(int ex)
{
    int b_ex = get_size() - ex - 1;
    if (bit_rep[b_ex] == 1)
        shift_left(1);
    else
        bit_rep[b_ex] = 1;
}

/*
 * Static function to convert num_str to binary(bool_vec).
 */
bool_vec base2::convert_to_binary(const char* num_str)
{
    unsigned long num = std::strtoull(num_str, nullptr, 10);
    bool_vec num_bits = convert_to_binary(num);

    return num_bits;
}

/*
 * Static function to convert num_str to binary(bool_vec).
 */
bool_vec base2::convert_to_binary(const std::string& num_str)
{
    base2 dst = zero;
    int last = num_str.size() - 1;
    int i = last;
    base2 ten_exp = one;

    while (i >= 0) {
        bool_vec bits = util::convert_to_bits(num_str[i] - '0');
        base2 bin_digit(bits);
        bin_digit.multiply_with(ten_exp);
        dst.add_to(bin_digit);
        ten_exp.multiply_with_ten();
        i--;
    }

    return dst.get_bits();
}

/*
 * Prints *this in binary.
 */
void base2::print_bits() const
{
    if (is_negative == true)
        std::cout << "-";
    for (auto m:bit_rep)
        std::cout << m;

    std::cout << std::endl;
}

/*
 * Prints *this in base 10.
 */
void base2::print_base10() const
{
    base10 b10(bit_rep, is_negative);
    b10.print();
}
