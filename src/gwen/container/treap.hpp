#pragma once

#include <cassert>
#include <utility>
#include <vector>

#include "gwen/misc/node_pool.hpp"
#include "gwen/misc/xorshift.hpp"
#include "gwen/types.hpp"

namespace gwen {

template <typename Monoid> struct treap {
    using S = typename Monoid::S;
    using tree = i32;

    struct node {
        tree left = 0, right = 0;
        S val = Monoid::e(), prod = Monoid::e();
        i32 size = 0;
        u32 prio = 0;
        node() = default;
        explicit node(const S& v) : val(v), prod(v), size(1), prio(rand32()) {}
    };

    static inline node_pool<node> d;
    static constexpr tree NIL = 0;

    tree root = NIL;

    treap() = default;
    explicit treap(const std::vector<S>& vec) {
        for (const S& x : vec) push_back(x);
    }

    i32 size() const { return size_(root); }
    bool empty() const { return root == NIL; }

    void insert(i32 pos, const S& x) {
        assert(0 <= pos && pos <= size());
        auto [l, r] = split(root, pos);
        root = merge(merge(l, d.new_node(node(x))), r);
    }
    void erase(i32 pos) {
        assert(0 <= pos && pos < size());
        auto [l, r] = split(root, pos);
        auto [m, rr] = split(r, 1);
        root = merge(l, rr);
    }

    S prod(i32 l, i32 r) {
        assert(0 <= l && l <= r && r <= size());
        if (l >= r) return Monoid::e();
        tree left, mid, right;
        std::tie(left, mid) = split(root, l);
        std::tie(mid, right) = split(mid, r - l);
        S res = d[mid].prod;
        root = merge(merge(left, mid), right);
        return res;
    }
    S get(i32 pos) {
        assert(0 <= pos && pos < size());
        return prod(pos, pos + 1);
    }

    void set(i32 pos, const S& x) {
        assert(0 <= pos && pos < size());
        auto [l, r] = split(root, pos);
        auto [m, rr] = split(r, 1);
        d[m].val = x;
        update(m);
        root = merge(merge(l, m), rr);
    }

    S all_prod() const { return prod_(root); }

    void concat(treap& other) {
        root = merge(root, other.root);
        other.root = NIL;
    }
    static treap concat(treap& t0, treap& t1) {
        treap r;
        r.root = merge(t0.root, t1.root);
        t0.root = t1.root = NIL;
        return r;
    }

    void push_back(const S& x) { insert(size(), x); }
    void push_front(const S& x) { insert(0, x); }

private:
    static i32 size_(tree t) { return t == NIL ? 0 : d[t].size; }
    static S prod_(tree t) { return t == NIL ? Monoid::e() : d[t].prod; }

    static void update(tree t) {
        if (t == NIL) return;
        node& n = d[t];
        n.size = 1 + size_(n.left) + size_(n.right);
        n.prod = Monoid::op(Monoid::op(prod_(n.left), n.val), prod_(n.right));
    }

    static tree merge(tree l, tree r) {
        if (l == NIL) return r;
        if (r == NIL) return l;
        if (d[l].prio > d[r].prio) {
            d[l].right = merge(d[l].right, r);
            update(l);
            return l;
        }
        d[r].left = merge(l, d[r].left);
        update(r);
        return r;
    }

    static std::pair<tree, tree> split(tree t, i32 k) {
        if (t == NIL) return {NIL, NIL};
        node& n = d[t];
        if (k <= size_(n.left)) {
            auto [a, b] = split(n.left, k);
            n.left = b;
            update(t);
            return {a, t};
        }
        auto [a, b] = split(n.right, k - size_(n.left) - 1);
        n.right = a;
        update(t);
        return {t, b};
    }
};

}  // namespace gwen
