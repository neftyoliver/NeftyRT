#pragma once
#include <cstdint>

namespace NEFTY {
    template<typename T>
    struct ArrayWithLengthU64 {
        T data[];
        uint64_t length;
    };

    template<typename T>
    union SpookyPTR64 {
        T * asKnownType;
        void * asVoidPtr;
        uint64_t asUint64;
        SpookyPTR64() : asKnownType(nullptr), asVoidPtr(nullptr) {}
        explicit SpookyPTR64(T * ptr) : asKnownType(ptr), asVoidPtr(nullptr) {}
    };
}