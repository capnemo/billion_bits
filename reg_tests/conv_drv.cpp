#include <iostream>
#include <cstring>
#include <map>
#include "base63.h"

void display_base2(const char* b10_num);
void display_vector(int exp_count, char** exponents);
void usage(const char* prog_name);

int main(int argc, char *argv[])
{
    if ((argc <= 2) || (strlen(argv[1]) != 2) || (argv[1][0] != '-')) {
        usage(argv[0]);
        return -1;
    }

    char func = argv[1][1];
    if ((func != 'x') && (func != 'v')) {
        std::cout << "error -- args" << std::endl;
        return -1;
    }

    if (func == 'v') {
        display_vector(argc, argv);
        return 0;
    } else if (func == 'x') 
        display_base2(argv[2]);

    return 0;
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

void usage(const char* prog_name)
{
    std::cout << "usage:" << prog_name << " -v|-x " << "<num_string>"
    << std::endl;
}

