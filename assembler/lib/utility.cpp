#include "utility.hpp"

long long base_16_string_to_base_10_llong(string base16){
    long long base10 = 0;

    for(int i = base16.size() - 1, p = 1; i >= 0; --i, p *= 16){
        long long num = 0;

        if(std::isalpha(base16[i])) num = base16[i] - 'A' + 10;
        else num = base16[i] - '0';
        
        base10 += p * num;
    }

    return base10;
}

string base_10_llong_to_base_16_string(long long base10, int len){
    string base16;
    for(int i = 0; i < len; ++i){
        char r = base10 % 16;
        if(r < 10) base16.push_back(r + '0');
        else base16.push_back(r + 'A' - 10);
        base10 /= 16;
    }
    reverse(base16.begin(), base16.end());
    // cout << base16 << endl;
    return base16;
}

long long base_16_string_to_base_10_llong(string base16, int len){
    long long base10 = base_16_string_to_base_10_llong(base16);
    if(base10 >= (1 << (len - 1))) base10 -= (1 << len);
    return base10;
}
