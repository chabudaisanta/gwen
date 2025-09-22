#pragma once
#include <random>

#include "gwen/types.hpp"

namespace gwen {

namespace internal {

struct xorshift {
    u64 x = std::random_device{}();

    u64 get() {
        x ^= x >> 4;
        x ^= x << 37;
        x ^= x >> 11;
        return x;
    }
};

}  // namespace internal

u64 rand64() {
    static internal::xorshift rng;
    return rng.get();
}

u32 rand32() { return rand64() >> 32; }
u32 rand32(u32 r) { return ((rand64() >> 32) * r) >> 32; }
u32 rand32(u32 l, u32 r) { return rand32(r - l) + l; }

}  // namespace gwen