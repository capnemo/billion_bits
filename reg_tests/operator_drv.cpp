#include <iostream>
#include <cstring>
#include <map>
#include "base63.h"

template <typename T>
void modulo_and_print(T& n1, T& n2);
template <typename T>
bool execute_operator(char op, T& a1, T& a2);

bool is_less_than_zero(std::string& num);
void print_usage(const std::string& prog_name);
//base63 and base2 need default constructors
int main(int argc, char *argv[])
{
    std::vector<std::string> arg_vec;
    for (int i = 0; i < argc; i++) 
        arg_vec.push_back(std::string(argv[i]));

    if (arg_vec.size() != 5) {
        print_usage(arg_vec[0]);
        return -1;
    }
    
    if ((arg_vec[1] != "-b2") && (arg_vec[1] != "-b63")) {
        print_usage(arg_vec[0]);
        return -1;
    }

    if (arg_vec[2][0] != '-') {
        print_usage(arg_vec[0]);
        return -1;
    }

    
    char func = arg_vec[2][1];
    if (func == 'p') {
        if (arg_vec[1] == "-b63") {
            base63 b1 = base63::convert_to_b63(arg_vec[3]);
            util::raise_to(b1, std::strtoull(argv[4], nullptr, 10));
            std::cout << "----------" << std::endl;
            //b1.print_base10();
/*
            b1.raise_to(std::strtoull(argv[4], nullptr, 10));
            b1.print_base10();
*/
        } else {
            base2 b1(arg_vec[3], is_less_than_zero(arg_vec[3]));
            util::raise_to(b1, std::strtoull(argv[4], nullptr, 10));
            std::cout << "----------" << std::endl;
            //b1.print_base10();
/*
            b1.raise_to(std::strtoull(argv[4], nullptr, 10));
            b1.print_base10();
*/
        }
        return 0;
    }

    if (func == 'x') {
        if (arg_vec[1] == "-b63") {
            base63 b1 = base63::convert_to_b63(arg_vec[3]);
            b1.raise_to(std::strtoull(argv[4], nullptr, 10));
            b1.print_base10();
        } else {
            base2 b1(arg_vec[3], is_less_than_zero(arg_vec[3]));
            b1.raise_to(std::strtoull(argv[4], nullptr, 10));
            b1.print_base10();
        }
        return 0;
    }

    if (arg_vec[1] == "-b63") {
        base63 b1 = base63::convert_to_b63(arg_vec[3]);
        base63 b2 = base63::convert_to_b63(arg_vec[4]);
        if (execute_operator(func, b1, b2) == false) {
            print_usage(arg_vec[0]);
            std::cout << "error -- args 3" << std::endl;
            return -1;
        }
    } else { 
        base2 b1(arg_vec[3], is_less_than_zero(arg_vec[3]));
        base2 b2(arg_vec[4], is_less_than_zero(arg_vec[4]));
        if (execute_operator(func, b1, b2) == false) {
            print_usage(arg_vec[0]);
            return -1;
        }
    }
    
    return 0;
}

template <typename T>
bool execute_operator(char op, T& a1, T& a2)
{
    bool rc = true;
    switch (op) {
        case 'a':
            a1.sum(a2);
            //add_and_print(a1, a2);
            break;
        case 's':
            a1.difference(a2);
            //subtract_and_print(a1, a2);
            break;
        case 'm':
            a1.multiply_with(a2);
            //multiply_and_print(a1, a2);
            break;
        case 'd':
            a1.divide_by(a2);
            //divide_and_print(a1, a2);
            break;
        case 'r':
            modulo_and_print(a1, a2);
            break;
        default:
            rc = false;
            break;
    }

    if (rc == true)
        a1.print_base10();

    return rc;       
}

template <typename T>
void modulo_and_print(T& n1, T& n2)
{
    T r = n1.get_modulo(n2);
    n1 = r;
}

bool is_less_than_zero(std::string& num)
{
    bool s = false;
    if (num[0] == '-') {
        num = num.substr(1, num.size() - 1);
        s = true;
    }

    return s;
}

void print_usage(const std::string& prog_name)
{
    std::cout << "Usage: ";
    std::cout << prog_name << " -b2|-b63 " << "-a|-s|-m|-d|-r|-p";
    std::cout << " <operand 1> <operand 2>" << std::endl;
    std::cout << "The operands are in base 10" << std::endl;
}

