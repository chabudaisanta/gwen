#pragma once

#include <algorithm>
#include <bit>
#include <cassert>
#include <vector>

#include "gwen/alge/acted_monoid.hpp"
#include "gwen/types.hpp"

namespace gwen {

/**
 * @brief 遅延評価セグメント木
 * @details 作用付きモノイドを要素に持ち、区間更新と区間積の取得を $O(\log N)$ で行います。
 */
template <acted_monoid M> class LazySegmentTree {
public:
    using S = typename M::S;
    using F = typename M::F;

private:
    i32 n;
    i32 size;
    i32 log;
    std::vector<S> d;
    std::vector<F> lz;

    void update(i32 k) { d[k] = M::op(d[2 * k], d[2 * k + 1]); }
    void all_apply(i32 k, F f) {
        d[k] = M::mapping(f, d[k]);
        if (k < size) lz[k] = M::composition(f, lz[k]);
    }
    void push(i32 k) {
        all_apply(2 * k, lz[k]);
        all_apply(2 * k + 1, lz[k]);
        lz[k] = M::id();
    }

public:
    LazySegmentTree() : LazySegmentTree(0) {}
    explicit LazySegmentTree(i32 n_) : LazySegmentTree(std::vector<S>(n_, M::e())) {}
    explicit LazySegmentTree(const std::vector<S>& v) {
        n = i32(v.size());
        size = std::bit_ceil(static_cast<u32>(std::max(1, n)));
        log = std::countr_zero(static_cast<u32>(size));
        d.assign(2 * size, M::e());
        lz.assign(size, M::id());
        for (i32 i = 0; i < n; i++) d[size + i] = v[i];
        for (i32 i = size - 1; i >= 1; i--) update(i);
    }

    void set(i32 p, S x) {
        assert(0 <= p && p < n);
        p += size;
        for (i32 i = log; i >= 1; i--) push(p >> i);
        d[p] = x;
        for (i32 i = 1; i <= log; i++) update(p >> i);
    }

    S get(i32 p) {
        assert(0 <= p && p < n);
        p += size;
        for (i32 i = log; i >= 1; i--) push(p >> i);
        return d[p];
    }

    S prod(i32 l, i32 r) {
        assert(0 <= l && l <= r && r <= n);
        if (l == r) return M::e();

        l += size;
        r += size;

        for (i32 i = log; i >= 1; i--) {
            if (((l >> i) << i) != l) push(l >> i);
            if (((r >> i) << i) != r) push((r - 1) >> i);
        }

        S sml = M::e(), smr = M::e();
        while (l < r) {
            if (l & 1) sml = M::op(sml, d[l++]);
            if (r & 1) smr = M::op(d[--r], smr);
            l >>= 1;
            r >>= 1;
        }

        return M::op(sml, smr);
    }

    S all_prod() const { return d[1]; }

    void apply(i32 p, F f) {
        assert(0 <= p && p < n);
        p += size;
        for (i32 i = log; i >= 1; i--) push(p >> i);
        d[p] = M::mapping(f, d[p]);
        for (i32 i = 1; i <= log; i++) update(p >> i);
    }

    void apply(i32 l, i32 r, F f) {
        assert(0 <= l && l <= r && r <= n);
        if (l == r) return;

        l += size;
        r += size;

        for (i32 i = log; i >= 1; i--) {
            if (((l >> i) << i) != l) push(l >> i);
            if (((r >> i) << i) != r) push((r - 1) >> i);
        }

        {
            i32 l2 = l, r2 = r;
            while (l < r) {
                if (l & 1) all_apply(l++, f);
                if (r & 1) all_apply(--r, f);
                l >>= 1;
                r >>= 1;
            }
            l = l2;
            r = r2;
        }

        for (i32 i = 1; i <= log; i++) {
            if (((l >> i) << i) != l) update(l >> i);
            if (((r >> i) << i) != r) update((r - 1) >> i);
        }
    }

    template <bool (*g)(S)> i32 max_right(i32 l) {
        return max_right(l, [](S x) { return g(x); });
    }

    template <class G> i32 max_right(i32 l, G g) {
        assert(0 <= l && l <= n);
        assert(g(M::e()));
        if (l == n) return n;
        l += size;
        for (i32 i = log; i >= 1; i--) push(l >> i);
        S sm = M::e();
        do {
            while (l % 2 == 0) l >>= 1;
            if (!g(M::op(sm, d[l]))) {
                while (l < size) {
                    push(l);
                    l = (2 * l);
                    if (g(M::op(sm, d[l]))) {
                        sm = M::op(sm, d[l]);
                        l++;
                    }
                }
                return l - size;
            }
            sm = M::op(sm, d[l]);
            l++;
        } while ((l & -l) != l);
        return n;
    }

    template <bool (*g)(S)> i32 min_left(i32 r) {
        return min_left(r, [](S x) { return g(x); });
    }

    template <class G> i32 min_left(i32 r, G g) {
        assert(0 <= r && r <= n);
        assert(g(M::e()));
        if (r == 0) return 0;
        r += size;
        for (i32 i = log; i >= 1; i--) push((r - 1) >> i);
        S sm = M::e();
        do {
            r--;
            while (r > 1 && (r % 2)) r >>= 1;
            if (!g(M::op(d[r], sm))) {
                while (r < size) {
                    push(r);
                    r = (2 * r + 1);
                    if (g(M::op(d[r], sm))) {
                        sm = M::op(d[r], sm);
                        r--;
                    }
                }
                return r + 1 - size;
            }
            sm = M::op(d[r], sm);
        } while ((r & -r) != r);
        return 0;
    }
};

}  // namespace gwen
