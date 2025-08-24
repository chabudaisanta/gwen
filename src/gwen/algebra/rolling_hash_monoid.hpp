#pragma once

#include <random>

#include "gwen/algebra/monoid.hpp"
#include "gwen/mod/rolling_hash.hpp"
#include "gwen/types.hpp"

namespace gwen {
using gwen::hash::add_mod;
using gwen::hash::calc_mod;
using gwen::hash::mul_mod;
using gwen::hash::sub_mod;

template <class T, u64 (*conv)(const T&)> struct rolling_hash_monoid {
    using S = gwen::hash::hash_segment;
    const u64 base = (std::random_device{}() | 2);
    S e = S();
    S op(const S& a, const S& b) const {
        return S(add_mod(mul_mod(b.v, a.p), a.v), mul_mod(a.p, b.p));
    }

    S make_single(const T& x) const { return S(calc_mod(conv(x)), base); }

    template <typename Iterator> S make_range(Iterator begin, Iterator end) {
        S ret = e;
        for (auto it = begin; it != end; ++it) {
            S cur = make_single(*it);
            ret = op(ret, cur);
        }
        return ret;
    }
};

}  // namespace gwen