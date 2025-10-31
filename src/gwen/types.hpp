#pragma once

#include <cstdint>

namespace gwen {
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;
using i128 = __int128_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;
using u128 = __uint128_t;

constexpr i32 operator""_i32(unsigned long long x) {
    return i32(x);
}
constexpr u32 operator""_u32(unsigned long long x) {
    return u32(x);
}
constexpr i64 operator""_i64(unsigned long long x) {
    return i64(x);
}
constexpr u64 operator""_u64(unsigned long long x) {
    return u64(x);
}

}  // namespace gwen