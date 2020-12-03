#include <bitset>
#include "base10.h"
#include "base63.h"

extern "C" void mul_dq(unsigned long* a1, unsigned long* a2);
static const ulong msb_on_mask = 0x8000000000000000;
static const ulong msb_off_mask = 0x7fffffffffffffff;

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
 * Converts a series of exponents of 2 to a base 63 number.
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

    base63 b(b63_num);
    b.trim_leading_zeros();
    return b;
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
    
    return base63(b63_digits, b2_num.less_than_zero());
}

//Returns true IFF dig_vec > arg_bits. Sign is not considered.
bool base63::is_greater_than(b63_vec& arg_bits) const
{
    if (dig_vec.size() != arg_bits.size()) 
        return (dig_vec.size() > arg_bits.size());

    for (int i = 0; i < dig_vec.size(); i++) 
        if (dig_vec[i] != arg_bits[i])
            return (dig_vec[i] > arg_bits[i]);
    
    return false;
}

/*
 * Adds addend to *this. 
 */
/* confirm vectorization */
void base63::add_to(const base63& addend)
{
    int local_sz = get_size();
    int addend_sz = addend.get_size();
    int i = local_sz - 1;
    int j = addend_sz - 1;
    b63_vec addend_bits = addend.get_bits();

    while ((i >= 0) && (j >= 0)) 
        dig_vec[i--] += addend_bits[j--];

    if (local_sz < addend_sz) 
        splice(addend_bits, addend_sz - local_sz);

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

void base63::sum(const base63& arg)
{
    if (arg.is_less_than_zero() == is_less_than_zero())
        add_to(arg);
    else
        subtract_from(arg);
}

void base63::difference(const base63& arg)
{
    if (arg.is_less_than_zero() == is_less_than_zero())
        subtract_from(arg);
    else
        add_to(arg);
}

//Case of a - b where abs(a) is greater than abs(b) is not optimal 
//because it involves a copy.
void base63::subtract_from(const base63& arg)
{
    bool real_arg_sign = !arg.is_less_than_zero();
    b63_vec arg_bits = arg.get_bits();
    if (arg.is_greater_than(dig_vec)) {
        b63_vec tmp = dig_vec;
        dig_vec = arg_bits;
        arg_bits = tmp;
        is_negative = real_arg_sign;
    }
    b63_vec* greater = &dig_vec;
    b63_vec* lesser = &arg_bits;
    
    int g_sz = greater->size();
    int l_sz = lesser->size();
    int i = g_sz - 1;
    int j = l_sz - 1;
    while ((i >= 0) && (j >= 0)) 
        (*greater)[i--] -= (*lesser)[j--];

    int carry = 0;
    for (int i = g_sz - 1; i > 0; i--) {
        if (carry == 1) {
            (*greater)[i]--;
            carry = 0;
        }

        if ((*greater)[i] & msb_on_mask) {
            (*greater)[i] &= msb_off_mask; 
            carry = 1;
        }
    }

    ulong& msb = (*greater)[0];
    if (carry == 1) 
        msb--;

    if (msb & msb_on_mask) {
        msb &= msb_off_mask;
        msb = ~msb;
        msb++;
    }
    
    trim_leading_zeros();
}
/*
 * Splices the first length members of source to the beginning of 
 * dig_vec
 */
void base63::splice(const b63_vec& source, int length)
{
    b63_vec::const_iterator source_begin = source.begin();
    dig_vec.insert(dig_vec.begin(), source_begin, 
                   source_begin + length);
}

/*
 * Multiplies multiplicand with *this. 
 */
void base63::multiply_with(const base63& multiplicand)
{
    resolve_sign(multiplicand.is_less_than_zero());
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

base63 base63::get_modulo(const base63& arg)
{
    if (arg.is_greater_than(dig_vec))
        return *this;

    base2 a1;
    base2 a2;
    convert_to_base2(a1);
    arg.convert_to_base2(a2);

    base2 rem = a1.get_modulo(a2);
    return convert_to_b63(rem);
}

void base63::divide_by(const base63& arg)
{
    base2 a1;
    base2 a2;
    convert_to_base2(a1);
    arg.convert_to_base2(a2);
    
    a1.divide_by(a2);
    *this = convert_to_b63(a1);
}

/*
 * Flips the sign
 */
void base63::flip_sign()
{
    is_negative = !is_negative;
}

bool base63::is_less_than_zero() const
{
    return is_negative;
}

void base63::resolve_sign(bool arg_sign)
{
    is_negative = (is_negative != arg_sign) ? true:false;
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

    if (dig_vec.size() == 0) {
        dig_vec.push_back(0);
        is_negative = false;
    }
}

void base63::convert_to_base2(base2& b2) const
{
    bool_vec bits;
    for (auto m:dig_vec) {
        std::bitset<63> digit(m);
        std::string dig_str = digit.to_string();
        for (auto c:dig_str)
            bits.push_back(c - '0');
    }
    
    int i = 0;
    while (bits[i] == 0)
        i++;
    bits.erase(bits.begin(), bits.begin() + i);
    base2 b2_num(bits, is_negative);
    //b2_num.print_bits();
    b2 = b2_num;
}

/*
 * Print in base 10.
 */
void base63::print_base10()
{
    if (is_negative == true)
        std::cout << "-";

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
    if (is_negative == true)
        std::cout << "-";

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
