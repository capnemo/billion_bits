#include <iostream>
#include <vector>
#include "util.h"
#include "base10.h"
#include "base_types.h"

void base10::print()
{
    for (auto m:digits)
        std::cout << m;

    std::cout << std::endl;
}
