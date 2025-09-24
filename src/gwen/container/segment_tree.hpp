#pragma once

#include <bit>
#include <cassert>
#include <vector>

#include "gwen/algebra/monoid.hpp"

namespace gwen {

template <monoid M>
class segment_tree {
private:
    using S = M::S;
    int n, bc;
    std::vector<S> d;
    M m;

public:
    // コンストラクタ
    explicit segment_tree() {}
    explicit segment_tree(int n_, const M& m_)
        : segment_tree(std::vector<S>(n_, m_.e), m_) {}
    explicit segment_tree(const std::vector<S>& vec, const M& m_)
        : n(vec.size()), bc(std::bit_ceil(vec.size())), m(m_) {
        d.assign(bc * 2, m.e);
        for (int i = 0; i < n; ++i) d[bc + i] = vec[i];
        for (int i = bc - 1; i >= 1; --i) d[i] = m.op(d[i * 2], d[i * 2 + 1]);
    }

    void set(int p, S x) {
        assert(0 <= p && p < n && "set()");
        p += bc;
        d[p] = x;
        for (; p > 1; p >>= 1) update(p >> 1);
    }

    S get(int p) const {
        assert(0 <= p && p < n && "get()");
        return d[p + bc];
    }

    S prod(int l, int r) const {
        assert(0 <= l && l <= r && r <= n && "prod()");
        S sml = m.e, smr = m.e;
        l += bc;
        r += bc;

        while (l < r) {
            if (l & 1) sml = m.op(sml, d[l++]);
            if (r & 1) smr = m.op(d[--r], smr);
            l >>= 1;
            r >>= 1;
        }
        return m.op(sml, smr);
    }

    S all_prod() const { return d[1]; }

    template <class F>
    int max_right(int l, F f) const {
        assert(0 <= l && l <= n && "max_right()");
        assert(f(m.e) && "max_right()");

        if (l == n) return n;
        l += bc;
        S sm = m.e;
        do {
            while (l % 2 == 0) l >>= 1;
            if (!f(m.op(sm, d[l]))) {
                while (l < bc) {
                    l *= 2;
                    if (f(m.op(sm, d[l]))) {
                        sm = m.op(sm, d[l]);
                        l++;
                    }
                }
                return l - bc;
            }
            sm = m.op(sm, d[l]);
            l++;
        } while ((l & -l) != l);
        return n;
    }

    template <class F>
    int min_left(int r, F f) const {
        assert(0 <= r && r <= n && "min_left()");
        assert(f(m.e) && "min_left()");

        if (r == 0) return 0;
        r += bc;
        S sm = m.e;
        do {
            r--;
            while (r > 1 && (r % 2)) r >>= 1;
            if (!f(m.op(d[r], sm))) {
                while (r < bc) {
                    r = r * 2 + 1;
                    if (f(m.op(d[r], sm))) {
                        sm = m.op(d[r], sm);
                        r--;
                    }
                }
                return r + 1 - bc;
            }
            sm = m.op(d[r], sm);
        } while ((r & -r) != r);
        return 0;
    }

private:
    void update(int k) { d[k] = m.op(d[k * 2], d[k * 2 + 1]); }
};

}  // namespace gwen