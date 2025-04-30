#pragma once
#include <cstdint>

namespace NEFTY {
    template<typename T>
    struct ArrayWithLengthU64 {
        T data[];
        uint64_t length;
    };

    template<typename T>
    union TemplatedPTR64 {
        T * asKnownType;
        void * asVoidPtr;
        uint64_t asUint64;
        TemplatedPTR64() : asKnownType(nullptr), asVoidPtr(nullptr) {}
        explicit TemplatedPTR64(T * ptr) : asKnownType(ptr), asVoidPtr(nullptr) {}
    };

    union SpookyPTR64 {
        void * asVoidPtr;
        uint64_t asUint64;
    };
}