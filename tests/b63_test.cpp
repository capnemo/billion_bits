#include <iostream>
#include <cstring>
#include "base63.h"

void display_base2(const char *b10_num);
void display_vector(std::vector<int> ex_vec);
void add_and_print(char *a1, char* a2);
void multiply_and_print(char *a1, char* a2);

int main(int argc, char *argv[])
{
    if ((argc <= 2) || (strlen(argv[1]) != 2) || (argv[1][0] != '-')) {
        std::cout << "error!" << std::endl;
        return -1;
    }

    char func = argv[1][1];
    std::vector<int> exp_vec;
    bool arg_count = true;
    switch (func)  {
        case 'x':
            if (argc != 3)  {
                arg_count = false;
                break;
            }
            display_base2(argv[2]);
            break;
        case 'v':
            for (int i = 2; i < argc; i++)  {
                ulong ex = std::strtoull(argv[i], nullptr, 10);
                exp_vec.push_back(ex);
            }
            display_vector(exp_vec);
            break;
        case 'a':
            if (argc != 4)  {
                arg_count = false;
                break;
            }
            add_and_print(argv[2], argv[3]);
            break;
        case 'm':
            if (argc != 4)  {
                arg_count = false;
                break;
            }
            multiply_and_print(argv[2], argv[3]);
            break;
        default:
            std::cout << "error" << std::endl;
    }

    if (arg_count == false) {
        std::cout << "error" << std::endl;
        return -1;
    }
}

void display_base2(const char *b10_num)
{
    std::string arg(b10_num);
    base63 b63_num = base63::convert_to_b63(arg);
    b63_num.print_bits(false);
}

void display_vector(std::vector<int> ex_vec)
{
    base63 b63 = base63::convert_to_base63(ex_vec);
    b63.print_bits();   
}

void add_and_print(char *a1, char* a2)
{
    base63 n1 = base63::convert_to_b63(std::string(a1));
    base63 n2 = base63::convert_to_b63(std::string(a2));

    n1.add_to(n2);
    n1.print_bits();
}

void multiply_and_print(char *a1, char* a2)
{
    base63 n1 = base63::convert_to_b63(std::string(a1));
    base63 n2 = base63::convert_to_b63(std::string(a2));

    n1.multiply_with(n2);
    n1.print_base10();
}
