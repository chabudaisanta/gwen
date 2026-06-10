#pragma once

#include "gwen/misc/xorshift.hpp"
#include "gwen/types.hpp"

namespace gwen {

inline u64 to_hash(u64 x) {
    static u64 seed = rand64();
    x += seed;
    x ^= x >> 33;
    x *= 0xff51afd7ed558ccdULL;
    x ^= x >> 33;
    x *= 0xc4ceb9fe1a85ec53ULL;
    x ^= x >> 33;
    return x;
}

}  // namespace gwen
