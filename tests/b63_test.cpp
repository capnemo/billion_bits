#include <iostream>
#include <cstring>
#include <map>
#include "base63.h"

using func_map = std::map<char, void(*)(base63&, base63&)>;
using func_map_iter = std::map<char, void(*)(base63&, base63&)>::iterator;
using arg_map = std::map<char, int>;
using arg_map_iter = std::map<char, int>::iterator;

void initialize_function_dispatch(func_map& f_table);
void initialize_arg_counts(arg_map& arg_table);

void display_base2(const char* b10_num);
void display_vector(int exp_count, char** exponents);
void add_and_print(base63& n1, base63& n2);
void multiply_and_print(base63& n1, base63& n2);
void subtract_and_print(base63& n1, base63& n2);
void divide_and_print(base63& n1, base63& n2);
void modulo_and_print(base63& n1, base63& n2);

int main(int argc, char *argv[])
{
    if ((argc <= 2) || (strlen(argv[1]) != 2) || (argv[1][0] != '-')) {
        std::cout << "error!" << std::endl;
        return -1;
    }

    char func = argv[1][1];
    if (func == 'v') {
        display_vector(argc, argv);
        return 0;
    }

    arg_map arg_counts;
    initialize_arg_counts(arg_counts);
    arg_map_iter count_iter = arg_counts.find(func);
    if ((count_iter == arg_counts.end()) || (count_iter->second != argc)) {
        std::cout << "error -- args" << std::endl;
        return -1;
    }
    
    func_map f_table;
    initialize_function_dispatch(f_table);
    if (func == 'x') {
        display_base2(argv[2]);
    } else {
        base63 b1 = base63::convert_to_b63(std::string(argv[2]));
        base63 b2 = base63::convert_to_b63(std::string(argv[3]));
        func_map_iter d_iter = f_table.find(func);
        if (d_iter != f_table.end())  {
            d_iter->second(b1, b2);
            b1.print_base10();
        }
    }
    return 0;
}

void initialize_arg_counts(arg_map& arg_table)
{
    arg_table['x'] = 3;
    arg_table['a'] = 4;
    arg_table['s'] = 4;
    arg_table['m'] = 4;
    arg_table['d'] = 4;
    arg_table['r'] = 4;
}

void initialize_function_dispatch(func_map& f_table)
{
    f_table['a'] = add_and_print;
    f_table['s'] = subtract_and_print;
    f_table['m'] = multiply_and_print;
    f_table['d'] = divide_and_print;
    f_table['r'] = modulo_and_print;
}

void display_base2(const char* b10_num)
{
    std::string arg(b10_num);
    base63 b63_num = base63::convert_to_b63(arg);
    b63_num.print_bits(false);
}

void display_vector(int exp_count, char** exponents)
{
    std::vector<int> exp_vec;
    for (int i = 2; i < exp_count; i++)  {
        ulong ex = std::strtoull(exponents[i], nullptr, 10);
        exp_vec.push_back(ex);
    }
    base63 b63 = base63::convert_to_base63(exp_vec);
    b63.print_bits();   
}

void add_and_print(base63& n1, base63& n2)
{
    n1.sum(n2);
}

void multiply_and_print(base63& n1, base63& n2)
{
    n1.multiply_with(n2);
}

void subtract_and_print(base63& n1, base63& n2)
{
    n1.difference(n2);
}

void divide_and_print(base63& n1, base63& n2)
{
    n1.divide_by(n2);
}

void modulo_and_print(base63& n1, base63& n2)
{
    base63 r = n1.get_modulo(n2);
    n1 = r;
}
