#pragma once

#include <cassert>
#include <string>
#include <format>

#include "gwen/alge/monoid.hpp"
#include "gwen/dump.hpp"
#include "gwen/types.hpp"
#include "gwen/utils/node_pool.hpp"

namespace gwen {

/**
 * @brief 動的セグメント木
 * @details 座標圧縮なしで広大な区間を扱えるセグメント木です。ポインタ（インデックス）ベースでノードを管理します。
 */
template <monoid M> class DynamicSegmentTree {
public:
    using S = typename M::S;
    using tree = i32;

    struct node {
        tree left = 0, right = 0;
        S val = M::e();
        node() = default;
    };

    static inline NodePool<node> d;
    static constexpr tree NIL = 0;

private:
    tree root = NIL;
    i64 n;

    S prod_(tree t, i64 l, i64 r, i64 ql, i64 qr) {
        if (t == NIL || r <= ql || qr <= l) return M::e();
        if (ql <= l && r <= qr) return d[t].val;
        i64 m = l + (r - l) / 2;
        return M::op(prod_(d[t].left, l, m, ql, qr), prod_(d[t].right, m, r, ql, qr));
    }

    tree set_(tree t, i64 l, i64 r, i64 p, S x) {
        if (t == NIL) t = d.new_node(node());
        if (r - l == 1) {
            d[t].val = x;
            return t;
        }
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
        i64 m = l + (r - l) / 2;
        if (p < m) return get_(d[t].left, l, m, p);
        return get_(d[t].right, m, r, p);
    }

    void get_elements_(tree t, i64 l, i64 r, std::vector<std::pair<i64, S>>& res) const {
        if (t == NIL) return;
        if (r - l == 1) {
            res.emplace_back(l, d[t].val);
            return;
        }
        i64 m = l + (r - l) / 2;
        get_elements_(d[t].left, l, m, res);
        get_elements_(d[t].right, m, r, res);
    }

public:
    DynamicSegmentTree() : n(0) {}
    explicit DynamicSegmentTree(i64 n_) : n(n_) {}

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

    std::string dump() const {
        std::vector<std::pair<i64, S>> elements;
        get_elements_(root, 0, n, elements);
        return std::format("DynamicSegmentTree{{\n  N = {},\n  elements = {}\n}}", n, internal::format_range(elements));
    }
};

}  // namespace gwen
