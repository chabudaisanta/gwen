#pragma once

#include <cassert>
#include <functional>
#include <utility>
#include <vector>

#include "gwen/misc/node_pool.hpp"
#include "gwen/misc/xorshift.hpp"
#include "gwen/types.hpp"

namespace gwen {

/// Key-value treap: BST by key, lazy propagation. Dynamic lazy segment tree by key range.
/// Monoid: S, F, op, e, mapping, composition, id(). Compare: key order (default std::less<Key>).
template <typename Key, typename Monoid, typename Compare = std::less<Key>> struct associative_treap {
    using S = typename Monoid::S;
    using F = typename Monoid::F;
    using tree = i32;

    struct node {
        tree left = 0, right = 0;
        Key key{};
        S val = Monoid::e(), prod = Monoid::e();
        i32 size = 0;
        u32 prio = 0;
        F lazy{};
        bool has_lazy = false;
        node() = default;
        node(const Key& k, const S& v) : key(k), val(v), prod(v), size(1), prio(rand32()) {}
    };

    static inline node_pool<node> d;
    static constexpr tree NIL = 0;
    static constexpr Compare cmp{};

    tree root = NIL;

    associative_treap() = default;

    i32 size() const { return size_(root); }
    bool empty() const { return root == NIL; }

    /// Set value at key (insert or overwrite). Unique keys (map semantics).
    void set(const Key& key, const S& value) {
        erase(key);
        auto [l, r] = split_lt(root, key);
        root = merge(merge(l, d.new_node(node(key, value))), r);
    }

    /// Remove key if present.
    void erase(const Key& key) {
        auto [l, r] = split_lt(root, key);
        if (r == NIL) {
            root = l;
            return;
        }
        auto [m, rr] = split_idx(r, 1);
        if (key_eq(d[m].key, key))
            root = merge(l, rr);
        else
            root = merge(merge(l, m), rr);
    }

    S get(const Key& key) {
        auto [l, r] = split_lt(root, key);
        if (r == NIL) {
            root = l;
            return Monoid::e();
        }
        auto [m, rr] = split_idx(r, 1);
        S res = key_eq(d[m].key, key) ? (push(m), d[m].val) : Monoid::e();
        root = merge(merge(l, m), rr);
        return res;
    }

    /// Product over keys in [key_l, key_r) (key_l <= k < key_r).
    S prod(const Key& key_l, const Key& key_r) {
        auto [left, mid_r] = split_lt(root, key_l);
        auto [mid, right] = split_lt(mid_r, key_r);
        if (mid == NIL) {
            root = merge(merge(left, mid), right);
            return Monoid::e();
        }
        push(mid);
        S res = d[mid].prod;
        root = merge(merge(left, mid), right);
        return res;
    }

    void apply(const Key& key_l, const Key& key_r, const F& f) {
        auto [left, mid_r] = split_lt(root, key_l);
        auto [mid, right] = split_lt(mid_r, key_r);
        if (mid == NIL) {
            root = merge(merge(left, mid), right);
            return;
        }
        push(mid);
        if (d[mid].has_lazy)
            d[mid].lazy = Monoid::composition(f, d[mid].lazy);
        else
            d[mid].lazy = f;
        d[mid].has_lazy = true;
        root = merge(merge(left, mid), right);
    }

    S all_prod() {
        if (empty()) return Monoid::e();
        push(root);
        return d[root].prod;
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

    /// Debug: in-order DFS, returns (key, val) in key order.
    std::vector<std::pair<Key, S>> to_vec() {
        std::vector<std::pair<Key, S>> res;
        res.reserve(size());
        to_vec_(root, res);
        return res;
    }

private:
    static bool key_eq(const Key& a, const Key& b) { return !cmp(a, b) && !cmp(b, a); }
    static i32 size_(tree t) { return t == NIL ? 0 : d[t].size; }
    static S prod_(tree t) { return t == NIL ? Monoid::e() : d[t].prod; }
    static void to_vec_(tree t, std::vector<std::pair<Key, S>>& out) {
        if (t == NIL) return;
        push(t);
        to_vec_(d[t].left, out);
        out.emplace_back(d[t].key, d[t].val);
        to_vec_(d[t].right, out);
    }

    static void push(tree t) {
        if (t == NIL) return;
        node& n = d[t];
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

    /// (left, right) where every key in left is < key, every key in right is >= key.
    static std::pair<tree, tree> split_lt(tree t, const Key& key) {
        if (t == NIL) return {NIL, NIL};
        push(t);
        if (cmp(d[t].key, key)) {
            auto [a, b] = split_lt(d[t].right, key);
            d[t].right = a;
            update(t);
            return {t, b};
        }
        auto [a, b] = split_lt(d[t].left, key);
        d[t].left = b;
        update(t);
        return {a, t};
    }

    static std::pair<tree, tree> split_idx(tree t, i32 k) {
        if (t == NIL) return {NIL, NIL};
        push(t);
        node& n = d[t];
        if (k <= size_(n.left)) {
            auto [a, b] = split_idx(n.left, k);
            n.left = b;
            update(t);
            return {a, t};
        }
        auto [a, b] = split_idx(n.right, k - size_(n.left) - 1);
        n.right = a;
        update(t);
        return {t, b};
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
};

}  // namespace gwen
