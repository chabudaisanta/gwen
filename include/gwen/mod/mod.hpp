#pragma once

#include <cassert>
#include <utility>

#include "gwen/types.hpp"

namespace gwen {

/**
 * @brief べき乗余 $x^n \pmod{m}$ を返す
 * @details 計算量: $O(\log n)$
 * @param x 底
 * @param n 指数
 * @param m 法（デフォルト: 998244353）
 * @return $x^n \bmod m$
 */
i64 pow_mod(i64 x, i64 n, i64 m = 998244353) {
    if (x == 0) return (n ? 0 : 1);
    x %= m;
    i64 res = 1;
    while (n) {
        if (n & 1) res = (res * x) % m;
        x = (x * x) % m;
        n >>= 1;
    }
    return res;
}

/**
 * @brief 逆元 $a^{-1} \pmod{m}$ を返す
 * @details 拡張ユークリッド互除法を用いる。計算量: $O(\log m)$
 * @param a 逆元を求める値（$a \ne 0$）
 * @param m 法（デフォルト: 998244353）
 * @pre $a \ne 0$
 * @return $a^{-1} \bmod m$
 */
i64 inv_mod(i64 a, i64 m = 998244353) {
    assert(a != 0 && "inv_mod(0, m): inverse does not exist");
    i64 b = m, u = 1, v = 0;
    while (b) {
        i64 t = a / b;
        a -= t * b;
        std::swap(a, b);
        u -= t * v;
        std::swap(u, v);
    }
    u %= m;
    if (u < 0) u += m;
    return u;
}

/**
 * @brief 64ビット値の逆元 $a^{-1} \pmod{m}$ を返す
 * @details 内部で `i128` を利用する拡張ユークリッド互除法を用いる。計算量: $O(\log m)$
 * @param a 逆元を求める値
 * @param m 法
 * @pre $\gcd(a, m) = 1$
 * @return $a^{-1} \bmod m$
 */
u64 inv_mod_64(u64 a, u64 m) {
    i128 s = m, t = a;
    i128 x = 0, y = 1;

    while (t) {
        i128 u = s / t;
        s -= t * u;
        x -= y * u;
        std::swap(s, t);
        std::swap(x, y);
    }
    // assert(s == 1); // gcd(a,m)が1でなければ逆元は存在しない
    return (x % m + m) % m;
}

}  // namespace gwen
