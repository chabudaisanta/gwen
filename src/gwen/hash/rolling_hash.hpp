#pragma once

#include <algorithm>
#include <cassert>
#include <vector>

#include "gwen/algebra/rolling_hash_monoid.hpp"
#include "gwen/mod/mod61.hpp"
#include "gwen/types.hpp"

namespace gwen {

template <i32 ID = 0>
struct rolling_hash {
    using Monoid = rhash::rolling_hash_monoid<ID>;
    using S = typename Monoid::S;

    i32 n;
    // suf[i] = hash of [i, n) with a_i having weight r^0: a_i + a_{i+1}*r + ...
    std::vector<u64> suf;
    std::vector<u64> P;  // P[i] = r^i

    template <typename Container>
    explicit rolling_hash(const Container& seq)
        : n(static_cast<i32>(std::size(seq))),
          suf(n + 1, 0),
          P(n + 1, 1) {
        for (i32 i = n - 1; i >= 0; --i) {
            suf[i] = rhash::add_mod(
                static_cast<u64>(seq[i]),
                rhash::mul_mod(Monoid::r, suf[i + 1]));
        }
        for (i32 i = 1; i <= n; ++i) {
            P[i] = rhash::mul_mod(P[i - 1], Monoid::r);
        }
    }

    i32 size() const { return n; }

    // Returns S = (hash, r^len) for [l, r).
    // Normalized so the first element has weight r^0 (no inverse required).
    // h = a_l*r^0 + a_{l+1}*r^1 + ... + a_{r-1}*r^{r-l-1}
    S get(i32 l, i32 r) const {
        assert(0 <= l && l <= r && r <= n);
        if (l == r) return Monoid::e();
        u64 v = rhash::sub_mod(suf[l], rhash::mul_mod(suf[r], P[r - l]));
        u64 p = P[r - l];
        return {v, p};
    }

    bool equal(i32 l1, i32 r1, i32 l2, i32 r2) const {
        assert(0 <= l1 && l1 <= r1 && r1 <= n);
        assert(0 <= l2 && l2 <= r2 && r2 <= n);
        if (r1 - l1 != r2 - l2) return false;
        if (l1 == r1) return true;
        auto a = get(l1, r1), b = get(l2, r2);
        return a.v == b.v && a.p == b.p;
    }

    // Longest common prefix of suffixes [l1, n) and [l2, n).
    i32 lcp(i32 l1, i32 l2) const {
        assert(0 <= l1 && l1 <= n);
        assert(0 <= l2 && l2 <= n);
        i32 lo = 0, hi = std::min(n - l1, n - l2) + 1;
        while (hi - lo > 1) {
            i32 mid = (lo + hi) / 2;
            if (equal(l1, l1 + mid, l2, l2 + mid))
                lo = mid;
            else
                hi = mid;
        }
        return lo;
    }
};

}  // namespace gwen
