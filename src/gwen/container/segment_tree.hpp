#pragma once

#include <bit>
#include <cassert>
#include <vector>

#include "gwen/types.hpp"
#include "gwen/algebra/monoid.hpp"

namespace gwen {

template <monoid M>
class segment_tree {
private:
    using S = M::S;
    i32 n, bc;
    std::vector<S> d;

public:
    // コンストラクタ
    explicit segment_tree() : segment_tree(0) {}
    explicit segment_tree(i32 n_)
        : segment_tree(std::vector<S>(n_, M::e)) {}
    explicit segment_tree(const std::vector<S>& vec)
        : n(vec.size()), bc(std::bit_ceil(vec.size())) {
        d.assign(bc * 2, M::e);
        for (i32 i = 0; i < n; ++i) d[i + bc] = vec[i];
        for (i32 i = bc - 1; i >= 1; --i) d[i] = M::op(d[i * 2], d[i * 2 + 1]);
    }

    void set(i32 p, S x) {
        assert(0 <= p && p < n && "set()");
        p += bc;
        d[p] = x;
        for (; p > 1; p >>= 1) update(p >> 1);
    }

    S get(i32 p) const {
        assert(0 <= p && p < n && "get()");
        return d[p + bc];
    }

    S prod(i32 l, i32 r) const {
        assert(0 <= l && l <= r && r <= n && "prod()");
        S sml = M::e, smr = M::e;
        l += bc;
        r += bc;

        while (l < r) {
            if (l & 1) sml = M::op(sml, d[l++]);
            if (r & 1) smr = M::op(d[--r], smr);
            l >>= 1;
            r >>= 1;
        }
        return M::op(sml, smr);
    }

    S all_prod() const { return d[1]; }

    template <class F> i32 max_right(i32 l, F f) const {
        assert(0 <= l && l <= n && "max_right()");
        assert(f(M::e) && "max_right()");

        if (l == n) return n;
        l += bc;
        S sm = M::e;
        do {
            while (l % 2 == 0) l >>= 1;
            if (!f(M::op(sm, d[l]))) {
                while (l < bc) {
                    l *= 2;
                    if (f(M::op(sm, d[l]))) {
                        sm = M::op(sm, d[l]);
                        l++;
                    }
                }
                return l - bc;
            }
            sm = M::op(sm, d[l]);
            l++;
        } while ((l & -l) != l);
        return n;
    }

    template <class F> i32 min_left(i32 r, F f) const {
        assert(0 <= r && r <= n && "min_left()");
        assert(f(M::e) && "min_left()");

        if (r == 0) return 0;
        r += bc;
        S sm = M::e;
        do {
            r--;
            while (r > 1 && (r % 2)) r >>= 1;
            if (!f(M::op(d[r], sm))) {
                while (r < bc) {
                    r = r * 2 + 1;
                    if (f(M::op(d[r], sm))) {
                        sm = M::op(d[r], sm);
                        r--;
                    }
                }
                return r + 1 - bc;
            }
            sm = M::op(d[r], sm);
        } while ((r & -r) != r);
        return 0;
    }

private:
    void update(i32 k) { d[k] = M::op(d[k * 2], d[k * 2 + 1]); }
};

}  // namespace gwen