#pragma once

#include <algorithm>

#include "gwen/types.hpp"

namespace gwen {

i64 pow_mod(i64 x, i64 n, i64 m = 998244353) {
    if (x == 0) return (n ? 0 : 1);
    x %= m;
    i64 ret = 1;
    while (n) {
        if (n & 1) ret = (ret * x) % m;
        x = (x * x) % m;
        n >>= 1;
    }
    return ret;
}

i64 inv_mod(i64 a, i64 m = 998244353) {
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