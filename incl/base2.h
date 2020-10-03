#include <iostream>
#include <vector>
#include "util.h"
#include "base_types.h"

#ifndef BASE2_H
#define BASE2_H

class base2 {

    public:
    //should return bool_vec and be called from the constructor.

    static bool_vec convert_to_binary(unsigned long num);
    static bool_vec convert_to_binary(const char* num_str);
    static bool_vec convert_to_binary(const std::string& num_str);

    static base2 convert_to_bits(unsigned char digit);
    static bool validate(const std::string& num);

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

    base2 get_zero_value(int sz)
    {
        bool_vec z(sz, 0);
        return base2(z);
    }

/*
    void add_to(const base2& addend)
    {
        bool_vec a_bits = addend.get_bits();
        util::add(bit_rep, a_bits, 2);
    }
*/
   
    void add_to(const base2& addend); //Removed??
    void sum(const base2& arg);
    void difference(const base2& arg);
    void subtract_from(const base2& subtrahend); //Should be private??
    void multiply_with(const base2& multiplicand);

    inline void divide_by(const base2& divisor)
    {
        divide(divisor);
    }

    inline bool less_than_zero() const
    {
        return is_negative;
    }

    inline void set_negative()
    {
        is_negative = true;
    }

    base2 get_modulo(const base2& divisor);
    bool is_greater_than(const base2& num) const;
    
    void print_bits() const; 
    void print_base10() const;

    private:
    inline void reset_bits(const bool_vec& new_bits)
    {
        bit_rep = new_bits;
    }
    
    void shift_left(int places)
    {
        if (places <= 0) 
            return;

        bit_rep.insert(bit_rep.end(), places, 0);
    }

    void shift_right(int places)
    {
        if (places <= 0) 
            return;

        bit_rep.insert(bit_rep.begin(), places, 0);
    }

    inline void set_bits(bool_vec& d_bits) const
    {
        d_bits = bit_rep;
    }

    inline int get_size() const
    {
        return bit_rep.size();
    }

    inline bool_vec get_bits() const
    {
        return bit_rep;
    }

    inline void flip() 
    {
        bit_rep.flip();
    }

    inline bool at(int i) const
    {
        return bit_rep[i];
    }

    inline bool is_equal_to(const base2& arg)
    {
        return (get_bits() == arg.get_bits());
    }

    inline bool is_less_than(const base2& arg)
    {
        return !is_greater_than(arg);
    }

    void increment();
    base2 divide(const base2& divisor);
    base2 get_exponent_bin(int exp);
    void trim_left();
    void trim_right(int places);
    void add_exponent(int ex);
    bool is_zero();

    //bool is_negative = false;
    std::vector<bool> bit_rep;
    bool is_negative;
};
#endif
