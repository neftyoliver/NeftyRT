//
// Created by NeftyOliver on 25. 4. 18.
//

#include <iostream>

#include "native.h"

void fff(const uint32_t a) {
    std::cout << "Hello World!\n"  << a << std::endl;
}

int main() {
    void (* ttt)(const uint32_t a) = fff;
    PTR64 ptr64 = ptr64FromAddress((uint64_t) ttt);
    ptr64.asVoidPtr(100);
}