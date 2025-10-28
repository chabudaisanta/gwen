#pragma once

#include <random>

#include "gwen/algebra/monoid.hpp"
#include "gwen/hash/rolling_hash.hpp"
#include "gwen/types.hpp"

namespace gwen {

template <class T, u64 (*conv)(const T&)> struct rolling_hash_monoid {
    using S = gwen::hash::hash_segment;
    static inline u64 base = (std::random_device{}() | 2);
    static inline S e = S();
    static S op(const S& a, const S& b) {
        return S(mod61::add_mod(mod61::mul_mod(b.v, a.p), a.v), mod61::mul_mod(a.p, b.p));
    }
    static S e_fp() { return e; }

    static S make_single(const T& x) { return S(mod61::calc_mod(conv(x)), base); }

    template <typename Iterator>
    static S make_range(Iterator begin, Iterator end) {
        S ret = e;
        for (auto it = begin; it != end; ++it) {
            S cur = make_single(*it);
            ret = op(ret, cur);
        }
        return ret;
    }
};

}  // namespace gwen