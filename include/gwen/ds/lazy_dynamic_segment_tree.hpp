#pragma once

#include <cassert>

#include "gwen/alge/acted_monoid.hpp"
#include "gwen/types.hpp"
#include "gwen/utils/node_pool.hpp"

namespace gwen {

/**
 * @brief 遅延評価付き動的セグメント木
 * @details 座標圧縮なしで広大な区間への作用と区間積を扱えるセグメント木です。
 */
template <acted_monoid M> class LazyDynamicSegmentTree {
public:
    using S = typename M::S;
    using F = typename M::F;
    using tree = i32;

    struct node {
        tree left = 0, right = 0;
        S val = M::e();
        F lz = M::id();
        node() = default;
    };

    static inline NodePool<node> d;
    static constexpr tree NIL = 0;

private:
    tree root = NIL;
    i64 n;

    void all_apply_(tree t, F f) {
        d[t].val = M::mapping(f, d[t].val);
        d[t].lz = M::composition(f, d[t].lz);
    }

    void push_(tree t) {
        // Need `operator==` or just apply unconditionally for `lz`.
        // Let's assume unconditional push is fine, or check `has_lazy` like in Treap.
        // For simplicity and matching Treap, we should ideally use `has_lazy` or unconditional.
        // Let's do it with `if constexpr(std::equality_comparable<F>)` or similar.
        bool apply_lz = true;
        if constexpr (std::equality_comparable<F>) {
            if (d[t].lz == M::id()) apply_lz = false;
        }

        if (apply_lz) {
            if (d[t].left == NIL) d[t].left = d.new_node(node());
            if (d[t].right == NIL) d[t].right = d.new_node(node());
            all_apply_(d[t].left, d[t].lz);
            all_apply_(d[t].right, d[t].lz);
            d[t].lz = M::id();
        }
    }

    S prod_(tree t, i64 l, i64 r, i64 ql, i64 qr) {
        if (t == NIL || r <= ql || qr <= l) return M::e();
        if (ql <= l && r <= qr) return d[t].val;
        push_(t);
        i64 m = l + (r - l) / 2;
        return M::op(prod_(d[t].left, l, m, ql, qr), prod_(d[t].right, m, r, ql, qr));
    }

    tree set_(tree t, i64 l, i64 r, i64 p, S x) {
        if (t == NIL) t = d.new_node(node());
        if (r - l == 1) {
            d[t].val = x;
            return t;
        }
        push_(t);
        i64 m = l + (r - l) / 2;
        if (p < m) {
            tree cl = set_(d[t].left, l, m, p, x);
            d[t].left = cl;
        }
        else {
            tree cr = set_(d[t].right, m, r, p, x);
            d[t].right = cr;
        }
        S lv = d[t].left == NIL ? M::e() : d[d[t].left].val;
        S rv = d[t].right == NIL ? M::e() : d[d[t].right].val;
        d[t].val = M::op(lv, rv);
        return t;
    }

    S get_(tree t, i64 l, i64 r, i64 p) {
        if (t == NIL) return M::e();
        if (r - l == 1) return d[t].val;
        push_(t);
        i64 m = l + (r - l) / 2;
        if (p < m) return get_(d[t].left, l, m, p);
        return get_(d[t].right, m, r, p);
    }

    tree apply_(tree t, i64 l, i64 r, i64 ql, i64 qr, F f) {
        if (r <= ql || qr <= l) return t;
        if (t == NIL) t = d.new_node(node());
        if (ql <= l && r <= qr) {
            all_apply_(t, f);
            return t;
        }
        push_(t);
        i64 m = l + (r - l) / 2;
        tree cl = apply_(d[t].left, l, m, ql, qr, f);
        d[t].left = cl;
        tree cr = apply_(d[t].right, m, r, ql, qr, f);
        d[t].right = cr;
        S lv = d[t].left == NIL ? M::e() : d[d[t].left].val;
        S rv = d[t].right == NIL ? M::e() : d[d[t].right].val;
        d[t].val = M::op(lv, rv);
        return t;
    }

public:
    LazyDynamicSegmentTree() : n(0) {}
    explicit LazyDynamicSegmentTree(i64 n_) : n(n_) {}

    void set(i64 p, S x) {
        assert(0 <= p && p < n);
        root = set_(root, 0, n, p, x);
    }

    S get(i64 p) {
        assert(0 <= p && p < n);
        return get_(root, 0, n, p);
    }

    S prod(i64 l, i64 r) {
        assert(0 <= l && l <= r && r <= n);
        return prod_(root, 0, n, l, r);
    }

    S all_prod() const { return root == NIL ? M::e() : d[root].val; }

    void apply(i64 l, i64 r, F f) {
        assert(0 <= l && l <= r && r <= n);
        if (l >= r) return;
        root = apply_(root, 0, n, l, r, f);
    }
};

}  // namespace gwen
