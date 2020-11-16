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
    void print_base10(); 

    void print_bits(bool sep = false);
    int get_size() const { return dig_vec.size(); }
    b63_vec get_bits() const  { return dig_vec; }

    private:
    void trim_leading_zeros();

    private:
    b63_vec dig_vec;
    bool is_negative;
    const ulong msb_on_mask = 0x8000000000000000;
    const ulong msb_off_mask = 0x7fffffffffffffff;
};

#endif /* BASE63_H */
