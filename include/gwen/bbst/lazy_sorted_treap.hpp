#pragma once

#include <cassert>
#include <functional>
#include <utility>
#include <vector>

#include "gwen/alge/acted_monoid.hpp"
#include "gwen/types.hpp"
#include "gwen/utils/node_pool.hpp"
#include "gwen/utils/xorshift.hpp"

namespace gwen {

/**
 * @brief キーベースで区間作用・区間積が取得可能な順序付き多重集合・辞書（Treap）
 * @details 各キーに値を結びつけ、キーの範囲に対する区間積と遅延評価による区間作用をサポートします。
 */
template <typename K, acted_monoid M, typename Compare = std::less<K>> class LazySortedTreap {
public:
    using S = typename M::S;
    using F = typename M::F;
    using tree = i32;

    struct node {
        tree left = 0, right = 0;
        K key{};
        S val = M::e(), prod = M::e();
        F lz = M::id();
        i32 size = 0;
        u32 prio = 0;
        bool has_lazy = false;

        node() = default;
        explicit node(const K& k, const S& v) : key(k), val(v), prod(v), lz(M::id()), size(1), prio(rand32()) {}
    };

    static inline NodePool<node> d;
    static constexpr tree NIL = 0;
    static constexpr Compare cmp{};

private:
    tree root = NIL;

public:
    LazySortedTreap() = default;

    i32 size() const { return size_(root); }
    bool empty() const { return root == NIL; }

    void insert(const K& key, const S& val = M::e()) {
        auto [l, r] = split_lt(root, key);
        root = merge(merge(l, d.new_node(node(key, val))), r);
    }

    void erase(const K& key) {
        auto [l, r] = split_lt(root, key);
        if (r == NIL) {
            root = l;
            return;
        }
        auto [m, rr] = split_idx(r, 1);
        if (key_eq(d[m].key, key)) {
            root = merge(l, rr);
            d.free_node(m);
        }
        else {
            root = merge(merge(l, m), rr);
        }
    }

    void erase_all(const K& key) {
        auto [l, r] = split_lt(root, key);
        auto [m, rr] = split_le(r, key);
        root = merge(l, rr);
    }

    void set(const K& key, const S& val) {
        erase_all(key);
        insert(key, val);
    }

    void apply(const K& lower, const K& upper, const F& f) {
        auto [l, r1] = split_lt(root, lower);
        auto [mid, r2] = split_lt(r1, upper);
        if (mid != NIL) {
            d[mid].val = M::mapping(f, d[mid].val);
            d[mid].prod = M::mapping(f, d[mid].prod);
            d[mid].lz = M::composition(f, d[mid].lz);
            d[mid].has_lazy = true;
        }
        root = merge(merge(l, mid), r2);
    }

    S prod(const K& lower, const K& upper) {
        auto [l, r1] = split_lt(root, lower);
        auto [mid, r2] = split_lt(r1, upper);
        S res = prod_(mid);
        root = merge(merge(l, mid), r2);
        return res;
    }

    S all_prod() { return prod_(root); }

    bool contains(const K& x) {
        auto [idx, id] = lower_bound(x);
        if (id == NIL) return false;
        return key_eq(d[id].key, x);
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

private:
    static bool key_eq(const K& a, const K& b) { return !cmp(a, b) && !cmp(b, a); }

    static i32 size_(tree t) { return t == NIL ? 0 : d[t].size; }
    static S prod_(tree t) { return t == NIL ? M::e() : d[t].prod; }

    static void push(tree t) {
        if (t == NIL) return;
        node& n = d[t];

        if (n.has_lazy) {
            if (n.left != NIL) {
                d[n.left].val = M::mapping(n.lz, d[n.left].val);
                d[n.left].prod = M::mapping(n.lz, d[n.left].prod);
                d[n.left].lz = M::composition(n.lz, d[n.left].lz);
                d[n.left].has_lazy = true;
            }
            if (n.right != NIL) {
                d[n.right].val = M::mapping(n.lz, d[n.right].val);
                d[n.right].prod = M::mapping(n.lz, d[n.right].prod);
                d[n.right].lz = M::composition(n.lz, d[n.right].lz);
                d[n.right].has_lazy = true;
            }
            n.lz = M::id();
            n.has_lazy = false;
        }
    }

    static void update(tree t) {
        if (t == NIL) return;
        node& n = d[t];
        n.size = 1 + size_(n.left) + size_(n.right);
        n.prod = M::op(M::op(prod_(n.left), n.val), prod_(n.right));
    }

    static std::pair<tree, tree> split_lt(tree t, const K& key) {
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

    static std::pair<tree, tree> split_le(tree t, const K& key) {
        if (t == NIL) return {NIL, NIL};
        push(t);
        if (!cmp(key, d[t].key)) {
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
        if (d[l].prio > d[r].prio) {
            push(l);
            d[l].right = merge(d[l].right, r);
            update(l);
            return l;
        }
        push(r);
        d[r].left = merge(l, d[r].left);
        update(r);
        return r;
    }
};

}  // namespace gwen
