#include <iostream>
#include <vector>
#include "util.h"
#include "base_types.h"

#ifndef BASE2_H
#define BASE2_H

/* 
 * Implements base 2 arithmetic.
 */
class base2 {

    public:
    static bool_vec convert_to_binary(unsigned long num);
    static bool_vec convert_to_binary(const char* num_str);
    static bool_vec convert_to_binary(const std::string& num_str);

    base2() {}
    base2(bool_vec& bits, bool sign = false) : 
                           bit_rep(bits),
                           is_negative(sign) {} 

    base2(unsigned long number, bool sign = false) : 
                           bit_rep(convert_to_binary(number)),
                           is_negative(sign) {} 

    base2(const char c_str, bool sign = false) : 
                           bit_rep(convert_to_binary(c_str)),
                           is_negative(sign) {} 

    base2(const std::string& n_str, bool sign = false) :
                            bit_rep(convert_to_binary(n_str)),
                           is_negative(sign) {} 

    void add_to(const base2& addend); 
    void sum(const base2& arg);
    void difference(const base2& arg);
    void multiply_with(const base2& multiplicand);
    base2 get_modulo(const base2& divisor);
    void multiply_with_ten();

    void divide_by(const base2& divisor);
    bool less_than_zero() const;
    bool is_greater_than(const base2& num) const;
    void print_bits() const; 
    void print_base10() const;

    bool_vec get_bits() const;
    int get_size() const;
    void flip();
    void shift_right(int places);

    private:
    void shift_left(int places);
    void set_bits(bool_vec& d_bits) const;
    bool at(int i) const;
    bool is_equal_to(const base2& arg);
    bool is_less_than(const base2& arg);
    void subtract_from(const base2& subtrahend); 
    void increment();
    base2 divide(const base2& divisor);
    void trim_left();
    void trim_right(int places);
    void add_exponent(int ex);
    void set_zero();
    bool is_zero() const;
    bool is_unity() const;
    int get_num_ones() const;
    void set_sign(bool sign);

    std::vector<bool> bit_rep;
    bool is_negative = false;
};
#endif
