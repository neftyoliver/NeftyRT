//
// Created by NeftyOliver on 25. 4. 18.
//

#include <iostream>

#include "native.h"

uint64_t complexFunction(const char * name, const uint64_t number, const bool repeat) {
    if (repeat) {
        for (uint64_t i = 0; i < number; i++) {
            std::cout << "Hello World!\n" << name << std::endl;
        }
    }
    else {
        std::cout << "Hello World!\n" << name << std::endl;
    }

    return number + 1;
}

int main() {

    FunctionPTR64 fnPtr;
    fnPtr.asPTR64 = ptr64FromVoidPtr(&complexFunction);

    std::cout << "the functions address: " << fnPtr.asUint64 << std::endl;


    MAKE_FUNCTION_TYPE(uint64_t, complexFunction, const char *, const uint64_t, const bool);
    uint64_t value = reinterpret_cast<complexFunction_T>(fnPtr.asFunction)("Name", 5, true);
    std::cout << value << std::endl;
}