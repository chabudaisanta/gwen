#pragma once

#include <cassert>
#include <functional>
#include <utility>
#include <vector>

#include "gwen/misc/node_pool.hpp"
#include "gwen/misc/xorshift.hpp"
#include "gwen/types.hpp"

namespace gwen {

/// Ordered multiset (BST by key, heap by priority). std::multiset-like API (no iterators).
/// Supports insert, erase one, kth (k-th smallest), size, count_lower.
template <typename K, typename Compare = std::less<K>>
struct sorted_treap {
    using tree = i32;

    struct node {
        tree left = 0, right = 0;
        K key{};
        i32 size = 0;
        u32 prio = 0;
        node() = default;
        explicit node(const K& k) : key(k), size(1), prio(rand32()) {}
    };

    static inline node_pool<node> d;
    static constexpr tree NIL = 0;
    static constexpr Compare cmp{};

    tree root = NIL;

    sorted_treap() = default;

    i32 size() const { return size_(root); }
    bool empty() const { return root == NIL; }

    void insert(const K& key) {
        auto [l, r] = split_lt(root, key);
        root = merge(merge(l, d.new_node(node(key))), r);
    }

    /// Remove one occurrence of key. No-op if key not present.
    void erase(const K& key) {
        auto [l, r] = split_lt(root, key);
        if (r == NIL) {
            root = l;
            return;
        }
        auto [m, rr] = split_idx(r, 1);
        root = merge(l, rr);
    }

    /// 0-based: kth(0) is the smallest. Undefined if k < 0 or k >= size().
    K kth(i32 k) {
        assert(0 <= k && k < size());
        tree l, mid, r;
        std::tie(l, mid) = split_idx(root, k);
        std::tie(mid, r) = split_idx(mid, 1);
        K res = d[mid].key;
        root = merge(merge(l, mid), r);
        return res;
    }

    /// Number of elements with key < key.
    i32 count_lower(const K& key) {
        auto [l, r] = split_lt(root, key);
        i32 res = size_(l);
        root = merge(l, r);
        return res;
    }

    i32 count(const K& x) {
        auto [l, r] = split_lt(root, x);
        if (r == NIL) {
            root = l;
            return 0;
        }
        auto [mid, r2] = split_le(r, x);
        i32 res = size_(mid);
        root = merge(merge(l, mid), r2);
        return res;
    }

    /// Number of elements with key in [lower, upper).
    i32 count(const K& lower, const K& upper) {
        auto [l, r1] = split_lt(root, lower);
        if (r1 == NIL) {
            root = l;
            return 0;
        }
        auto [mid, r2] = split_lt(r1, upper);
        i32 res = size_(mid);
        root = merge(merge(l, mid), r2);
        return res;
    }

    bool contains(const K& x) {
        auto [idx, id] = lower_bound(x);
        if (id == NIL) return false;
        return key_eq(get_val(id), x);
    }

    /// Index of the smallest element >= x, and its node id. If none, returns (size(), NIL).
    std::pair<i32, tree> lower_bound(const K& x) {
        auto [l, r] = split_lt(root, x);
        i32 idx = size_(l);
        if (r == NIL) {
            root = l;
            return {idx, NIL};
        }
        auto [m, rr] = split_idx(r, 1);
        root = merge(merge(l, m), rr);
        return {idx, m};
    }

    /// Index of the smallest element > x, and its node id. If none, returns (size(), NIL).
    std::pair<i32, tree> upper_bound(const K& x) {
        auto [l, r] = split_le(root, x);
        i32 idx = size_(l);
        if (r == NIL) {
            root = l;
            return {idx, NIL};
        }
        auto [m, rr] = split_idx(r, 1);
        root = merge(merge(l, m), rr);
        return {idx, m};
    }

    /// Value (key) of the node with the given id. Assert if id == NIL.
    static K get_val(tree id) {
        assert(id != NIL);
        return d[id].key;
    }

    /// Debug: in-order DFS, returns keys in sorted order.
    std::vector<K> to_vec() const {
        std::vector<K> res;
        res.reserve(size());
        to_vec_(root, res);
        return res;
    }

private:
    static bool key_eq(const K& a, const K& b) {
        return !cmp(a, b) && !cmp(b, a);
    }
    static i32 size_(tree t) { return t == NIL ? 0 : d[t].size; }
    static void to_vec_(tree t, std::vector<K>& out) {
        if (t == NIL) return;
        to_vec_(d[t].left, out);
        out.push_back(d[t].key);
        to_vec_(d[t].right, out);
    }
    static void update(tree t) {
        if (t == NIL) return;
        d[t].size = 1 + size_(d[t].left) + size_(d[t].right);
    }

    /// (left, right) where every key in left is < key, every key in right is >= key.
    static std::pair<tree, tree> split_lt(tree t, const K& key) {
        if (t == NIL) return {NIL, NIL};
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

    /// (left, right) where every key in left is <= key, every key in right is > key.
    static std::pair<tree, tree> split_le(tree t, const K& key) {
        if (t == NIL) return {NIL, NIL};
        if (!cmp(key, d[t].key)) {  // d[t].key <= key
            auto [a, b] = split_le(d[t].right, key);
            d[t].right = a;
            update(t);
            return {t, b};
        }
        auto [a, b] = split_le(d[t].left, key);
        d[t].left = b;
        update(t);
        return {a, t};
    }

    static std::pair<tree, tree> split_idx(tree t, i32 k) {
        if (t == NIL) return {NIL, NIL};
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
