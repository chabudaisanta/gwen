#pragma once

#include <cassert>
#include <utility>
#include <vector>

#include "gwen/misc/node_pool.hpp"
#include "gwen/misc/xorshift.hpp"
#include "gwen/types.hpp"

namespace gwen {

/// Implicit treap: split by index (left subtree size).
/// Monoid must provide: S, F, op(S,S), e(), mapping(F,S), composition(F,F), id().
template <typename Monoid>
struct implicit_treap {
    using S = typename Monoid::S;
    using F = typename Monoid::F;
    using tree = i32;

    struct node {
        tree left = 0, right = 0;
        S val = Monoid::e(), prod = Monoid::e();
        i32 size = 0;
        u32 prio = 0;
        bool rev = false;
        F lazy{};
        bool has_lazy = false;
        node() = default;
        explicit node(const S& v) : val(v), prod(v), size(1), prio(rand32()) {}
    };

    static inline node_pool<node> d;
    static constexpr tree NIL = 0;

    tree root = NIL;

    implicit_treap() = default;
    explicit implicit_treap(const std::vector<S>& vec) {
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

    void reverse(i32 l, i32 r) {
        assert(0 <= l && l <= r && r <= size());
        if (l >= r) return;
        auto [left, mid_r] = split(root, l);
        auto [mid, right] = split(mid_r, r - l);
        d[mid].rev ^= true;
        root = merge(merge(left, mid), right);
    }

    void apply(i32 l, i32 r, const F& f) {
        assert(0 <= l && l <= r && r <= size());
        if (l >= r) return;
        auto [left, mid_r] = split(root, l);
        auto [mid, right] = split(mid_r, r - l);
        push(mid);
        if (d[mid].has_lazy)
            d[mid].lazy = Monoid::composition(f, d[mid].lazy);
        else
            d[mid].lazy = f;
        d[mid].has_lazy = true;
        root = merge(merge(left, mid), right);
    }

    S prod(i32 l, i32 r) {
        assert(0 <= l && l <= r && r <= size());
        if (l >= r) return Monoid::e();
        tree left, mid, right;
        std::tie(left, mid) = split(root, l);
        std::tie(mid, right) = split(mid, r - l);
        push(mid);
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
        push(m);
        d[m].val = x;
        update(m);
        root = merge(merge(l, m), rr);
    }

    void all_apply(const F& f) {
        if (empty()) return;
        push(root);
        if (d[root].has_lazy)
            d[root].lazy = Monoid::composition(f, d[root].lazy);
        else
            d[root].lazy = f;
        d[root].has_lazy = true;
    }

    S all_prod() {
        if (empty()) return Monoid::e();
        push(root);
        return d[root].prod;
    }

    void concat(implicit_treap& other) {
        root = merge(root, other.root);
        other.root = NIL;
    }
    static implicit_treap concat(implicit_treap& t0, implicit_treap& t1) {
        implicit_treap r;
        r.root = merge(t0.root, t1.root);
        t0.root = t1.root = NIL;
        return r;
    }

    void push_back(const S& x) { insert(size(), x); }
    void push_front(const S& x) { insert(0, x); }

private:
    static i32 size_(tree t) { return t == NIL ? 0 : d[t].size; }
    static S prod_(tree t) { return t == NIL ? Monoid::e() : d[t].prod; }

    static void push(tree t) {
        if (t == NIL) return;
        node& n = d[t];
        if (n.rev) {
            n.rev = false;
            std::swap(n.left, n.right);
            if (n.left != NIL) d[n.left].rev ^= true;
            if (n.right != NIL) d[n.right].rev ^= true;
        }
        if (n.has_lazy) {
            n.val = Monoid::mapping(n.lazy, n.val);
            n.prod = Monoid::mapping(n.lazy, n.prod);
            if (n.left != NIL) {
                if (d[n.left].has_lazy)
                    d[n.left].lazy = Monoid::composition(n.lazy, d[n.left].lazy);
                else
                    d[n.left].lazy = n.lazy;
                d[n.left].has_lazy = true;
            }
            if (n.right != NIL) {
                if (d[n.right].has_lazy)
                    d[n.right].lazy = Monoid::composition(n.lazy, d[n.right].lazy);
                else
                    d[n.right].lazy = n.lazy;
                d[n.right].has_lazy = true;
            }
            n.has_lazy = false;
        }
    }

    static void update(tree t) {
        if (t == NIL) return;
        node& n = d[t];
        push(n.left);
        push(n.right);
        n.size = 1 + size_(n.left) + size_(n.right);
        n.prod = Monoid::op(Monoid::op(prod_(n.left), n.val), prod_(n.right));
    }

    static tree merge(tree l, tree r) {
        if (l == NIL) return r;
        if (r == NIL) return l;
        push(l);
        push(r);
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
        push(t);
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
