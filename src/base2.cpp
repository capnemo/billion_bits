#include <iostream>
#include <vector>
#include "util.h"
#include "base2.h"
#include "base10.h"

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
 * IN @num to be converted to base2.
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
 * IN   @num number to be compared with.
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
 * Difference of *this and arg. Computes *this = *this + arg. 
 * sign aware.
 * IN   @arg number to be compared with.
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
        
    if (arg.is_greater_than(*this) == true) {
        is_negative = arg.less_than_zero();
        base2 t_arg = arg;
        t_arg.subtract_from(*this);
        *this = t_arg;
    } else 
        subtract_from(arg);
    
    if (is_zero() == true)
        is_negative = false;
}

/*
 * Find the difference between *this and arg.
 * Computes *this = *this - arg.
 * sign aware.
 * IN   @arg number to be compared with.
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
 * IN   @addend number to be added
 */
void base2::add_to(const base2& addend)
{
    bool_vec a_bits = addend.get_bits();
    util::add(bit_rep, a_bits, 2);
}

/*
 * Subtracts addend from *this. Computes *this -= subtrahend;
 * NOT sign aware.
 * IN   @subtrahend number to be subtracted
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
 * Removes trailing digits.
 * IN @places 
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
 * IN @multiplicand number to be multiplied with.
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
 * Raise *this to the power of 'power'
 */
void base2::raise_to(uint64_t power)
{
    base2 pnum = *this;

    uint8_t tz = util::get_trailing_zeros(power);
    shift_left(tz);

    uint64_t odd = power/(1 << tz);
    for (int i = 0; i < odd - 1; i++)
        multiply_with(pnum);
}

/*
 * Returns *this%divisor
 * IN @divisor. The denominator
 */
base2 base2::get_modulo(const base2& divisor)
{
    if (divisor.is_greater_than(*this)) 
        return *this;

    base2 dividend(bit_rep, is_negative);
    return dividend.divide(divisor);
}

/*
 * Computes *this/divisor
 * IN @divisor. The denominator
 */
void base2::divide_by(const base2& divisor)
{
    divide(divisor);
}


/*
 * Computes *this /= divisor. 
 * Sign aware
 * IN @divisor. The denominator
 */
base2 base2::divide(const base2& divisor)
{
    const bool orig_sign = is_negative;
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
        return divisor; //PROBLEM!!
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

    remainder.set_sign(orig_sign);
    return remainder;
}

/*
 * Returns true if *this is less than zero.
 */
bool base2::less_than_zero() const
{
    return is_negative;
}

/*
 * Set the sign.
 * IN @sign   the sign to set to.
 */
void base2::set_sign(bool sign)
{
    is_negative = sign;
}

/*
 * Computes *this += 2^ex
 * IN @ex exponent.
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
 * IN @num_str  string to convert.
 */
bool_vec base2::convert_to_binary(const char* num_str)
{
    unsigned long num = std::strtoull(num_str, nullptr, 10);
    bool_vec num_bits = convert_to_binary(num);

    return num_bits;
}

/*
 * Static function to convert num_str to binary(bool_vec).
 * IN @num_str  string to convert.
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

/*
 * Returns *this as a bool_vec
 */
bool_vec base2::get_bits() const
{
    return bit_rep;
}

/*
 *  Returns the size of the bool_vec
 */
int base2::get_size() const
{
    return bit_rep.size();
}

/*
 * Flips all the bits in the bool_vec
 */
void base2::flip()
{
    bit_rep.flip();
}

/*
 *  Right shift.
 *  IN @places  number of places to shift.
 */
void base2::shift_right(int places)
{
    if (places <= 0)
        return;

    bit_rep.insert(bit_rep.begin(), places, 0);
}

/*
 *  Left shift.
 *  IN @places  number of places to shift.
 */
void base2::shift_left(int places)
{
    if (places <= 0)
        return;

    bit_rep.insert(bit_rep.end(), places, 0);
}

/*
 * Resets the bits in bit_rep.
 * IN @d_bits  Source bit rep.
 */
void base2::set_bits(bool_vec& d_bits) const
{
    d_bits = bit_rep;
}

/*
 * Returns the bit at i.
 * IN @i  position
 */
bool base2::at(int i) const
{
    return bit_rep[i];
}

/*
 * Returns true if *this is equal arg
 * IN @arg   input arg.
 */
bool base2::is_equal_to(const base2& arg)
{
    return (get_bits() == arg.get_bits());
}

/*
 * Returns true if *this < arg
   IN @arg   input argument.
 */
bool base2::is_less_than(const base2& arg)
{
    return !is_greater_than(arg);
}

