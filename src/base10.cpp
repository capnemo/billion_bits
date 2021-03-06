#include <iostream>
#include <vector>
#include "util.h"
#include "base10.h"
#include "base_types.h"

/*
 *  Print digits in base 10 format.
 */
void base10::print()
{
    if (is_negative == true)
        std::cout << "-";

    for (auto m:digits)
        std::cout << m;

    std::cout << std::endl;
}
