#pragma once

#include "gwen/types.hpp"

namespace gwen {
namespace rhash {

constexpr u64 mod61 = (1ull << 61) - 1;
constexpr u64 msk30 = (1ull << 30) - 1;
constexpr u64 msk31 = (1ull << 31) - 1;
constexpr u64 msk61 = (1ull << 61) - 1;

constexpr u64 calc_mod(u64 x) {
    u64 res = (x >> 61) + (x & msk61);
    if (res >= mod61) res -= mod61;
    return res;
}

constexpr u64 mul_mod(u64 a, u64 b) {
    u128 t = u128(a) * b;
    u64 res = (u64)(t >> 61) + (u64)(t & msk61);
    if (res >= mod61) res -= mod61;
    return res;
}

constexpr u64 add_mod(u64 a, u64 b) {
    u64 res = a + b;
    if (res >= mod61) res -= mod61;
    return res;
}

constexpr u64 sub_mod(u64 a, u64 b) {
    if (a < b) return a + mod61 - b;
    return a - b;
}

}  // namespace rhash
}  // namespace gwen