#include <iostream>
#include <vector>
#include "util.h"

int main(int argc, char *argv[])
{
    std::vector<int> a = {1};
    std::vector<int> b = {1,1,1,1};
    util::add(a, b, 2);

    for (auto m:a)
        std::cout << m;
    std::cout << std::endl;
    
    std::vector<int> d = {3,4,5,9};
    std::vector<int> e = {7,6,5,9};
    util::add(d, e, 10);

    for (auto m:d)
        std::cout << m;
    std::cout << std::endl;

    std::vector<bool> f = {1};
    std::vector<bool> g = {1,1,1,1};
    util::add(f, g, 2);

    for (auto m:f)
        std::cout << m;
    std::cout << std::endl;
}
