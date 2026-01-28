#pragma once

#include <algorithm>
#include <random>

#include "gwen/misc/xorshift.hpp"
#include "gwen/mod/mod61.hpp"
#include "gwen/types.hpp"

namespace gwen {
namespace rhash {
template <i32 ID = 0> struct rolling_hash_monoid {
    static inline const u64 r = rand64() % (mod61 - 2) + 2;

    // rhash([a_0, a_1, ... ]) = Σ (a_i * r^i)
    struct S {
        u64 v, p;
    };

    static S op(S a, S b) {
        u64 v = add_mod(mul_mod(b.v, a.p), a.v);
        u64 p = mul_mod(a.p, b.p);
        return {v, p};
    }
    static S e() { return {0, 1}; }

    template <typename T> static S unit(T x) { return {u64(x), r}; }

    template <typename Iterator> static S range(Iterator begin, Iterator end) {
        u64 v = 0, p = 1;
        for (auto it = begin; it != end; ++it) {
            v = add_mod(v, mul_mod(p, u64(*it)));
            p = mul_mod(p, r);
        }
        return {v, p};
    }
};
}  // namespace rhash
}  // namespace gwen