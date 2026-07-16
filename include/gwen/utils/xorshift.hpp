#pragma once

// std
#include <chrono>
#include <random>

#include "gwen/types.hpp"

namespace gwen {

namespace internal {
    struct XorShift {
        u64 x = []() {
            u64 seed = static_cast<u64>(std::random_device{}()) ^ 
                       static_cast<u64>(std::chrono::steady_clock::now().time_since_epoch().count());
            return seed == 0 ? 1 : seed;
        }();

        u64 get() {
            x ^= x >> 4;
            x ^= x << 37;
            x ^= x >> 11;
            return x;
        }
    };
} // namespace internal

/**
 * @brief 64ビットの乱数を生成します。
 * @details 内部で static な xorshift 乱数生成器を使用しているため、**スレッドセーフではありません**。
 * 計算量: \f$O(1)\f$
 * @return 64ビットの乱数値
 */
inline u64 rand64() {
    static internal::XorShift rng;
    return rng.get();
}

/**
 * @brief 32ビットの乱数を生成します。
 * @details 内部で `rand64()` を呼び出しているため、**スレッドセーフではありません**。
 * 計算量: \f$O(1)\f$
 * @return 32ビットの乱数値
 */
inline u32 rand32() {
    return rand64() >> 32;
}

/**
 * @brief `0` 以上 `r - 1` 以下の32ビット乱数を生成します。
 * @details 内部で `rand64()` を呼び出しているため、**スレッドセーフではありません**。
 * 計算量: \f$O(1)\f$
 * @param r 生成する乱数の上限 (排他)
 * @return `[0, r)` の乱数値
 */
inline u32 rand32(u32 r) {
    return ((rand64() >> 32) * r) >> 32;
}

/**
 * @brief `l` 以上 `r - 1` 以下の32ビット乱数を生成します。
 * @details 内部で `rand64()` を呼び出しているため、**スレッドセーフではありません**。
 * 計算量: \f$O(1)\f$
 * @param l 生成する乱数の下限 (包含)
 * @param r 生成する乱数の上限 (排他)
 * @pre `l <= r`
 * @return `[l, r)` の乱数値
 */
inline u32 rand32(u32 l, u32 r) {
    return rand32(r - l) + l;
}

} // namespace gwen
