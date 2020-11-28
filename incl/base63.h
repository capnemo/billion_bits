/*
 * Header file for base 63 and operators.
 */



#include "base_types.h"
#include "base2.h"
#include "base10.h"

#ifndef BASE63_H
#define BASE63_H

class base63 {
 
    public:   
    static base63 convert_to_b63(const std::string& num_str);
    static base63 convert_to_b63(const base2& b2_num);
    static base63 convert_to_base63(const vec_int& exponents);

    base63(b63_vec bits, bool sign = false):dig_vec(bits),
                                            is_negative(sign) {}

    void add_to(const base63& addend);
    void multiply_with(const base63& multiplicand);
    void subtract_from(const base63& arg);
    void divide_by(const base63& arg);
    base63 get_modulo(const base63& arg);
    void difference(const base63& arg);
    void sum(const base63& arg);

    void print_base10(); 
    void print_bits(bool sep = false);

    int get_size() const { return dig_vec.size(); }
    b63_vec get_bits() const  { return dig_vec; }
    bool is_less_than_zero() const;
    void flip_sign();

    private:
    void trim_leading_zeros();
    void splice(const b63_vec& target, int length);
    bool is_greater_than(b63_vec& arg) const;
    void convert_to_base2(base2& b2) const;
    void resolve_sign(bool arg_sign);
    void check_for_zero();

    private:
    b63_vec dig_vec;
    bool is_negative;
};

#endif /* BASE63_H */
