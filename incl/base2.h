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
    base2(bool_vec& bits) : bit_rep(bits) {} 
    base2(int size) : bit_rep(size, 0) {}
    base2(unsigned long number) : 
                        bit_rep(convert_to_binary(number)){}

    base2(const char str) : bit_rep(convert_to_binary(str)) {}
    base2(const std::string& n_str) : 
                        bit_rep(convert_to_binary(n_str)) {}


    void add_to(const base2& addend)
    {
        bool_vec a_bits = addend.get_bits();
        util::add(bit_rep, a_bits, 2);
    }
   
    void subtract_from(const base2& subtrahend);
    void multiply_with(const base2& multiplicand);

    inline void divide_by(const base2& divisor)
    {
        divide(divisor);
    }

    base2 get_modulo(const base2& divisor);
    bool is_greater_than(const base2& num);
    
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
        return !is_greater_than(arg); //Wrong!
    }

    void increment();
    base2 divide(const base2& divisor);
    base2 get_exponent_bin(int exp);
    void trim_left();
    void trim_right(int places);
    void add_exponent(int ex);

    std::vector<bool> bit_rep;
};
#endif
