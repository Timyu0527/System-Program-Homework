#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <iostream>
#include <string>
#include <cctype>
#include <algorithm>

using namespace std;

long long base_16_string_to_base_10_llong(string base16);
string base_10_llong_to_base_16_string(long long base10, int len);

#endif
