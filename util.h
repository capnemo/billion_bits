#include <iostream>
#include <vector>

#ifndef UTIL_H
#define UTIL_H

namespace util {
template <typename T>
void add(std::vector<T>& n1, const std::vector<T>& n2, int base)
{
    if (n1.size() < n2.size()) 
        n1.insert(n1.begin(), n2.size() - n1.size(), 0);
    
    int n1_pos = n1.size() - 1;
    int carry = 0;
    for (int i = n2.size() - 1; i >= 0; i--) {
        int t = n1[n1_pos] + n2[i] + carry;
        n1[n1_pos--] = t%base;
        carry = t/base;
    }

    while (n1_pos >= 0) {
        int t = n1[n1_pos] + carry;
        n1[n1_pos--] = t%2;
        carry = t/2;
    }

    if (carry != 0)
        n1.insert(n1.begin(), 1, carry);
}
}

#endif
