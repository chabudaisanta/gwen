#pragma once

#include <random>
#include <concepts>
#include "gwen/types.hpp"

namespace gwen {

template<std::integral T>
u64 to_hash(T x) {
    static const u64 FIXED_RANDOM = std::random_device{}();
    x += FIXED_RANDOM;
    x = x ^ (x >> 30) * 3048498876;
    x = x ^ (x << 27) * 52136394;
    x = x ^ (x >> 31);
    return x;
}

}