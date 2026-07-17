#pragma once

#include <cassert>
#include <functional>
#include <optional>
#include <utility>
#include <vector>

#include "gwen/alge/monoid.hpp"
#include "gwen/types.hpp"
#include "gwen/utils/node_pool.hpp"
#include "gwen/utils/xorshift.hpp"

namespace gwen {

/**
 * @brief キーベースで区間積が取得可能な順序付き多重集合・辞書（Treap）
 * @details 各キーに値を結びつけ、キーの範囲に対する区間積を計算します。
 */
template <typename K, monoid M, typename Compare = std::less<K>> class ProdSortedTreap {
public:
    using S = typename M::S;
    using tree = i32;

    struct node {
        tree left = 0, right = 0, parent = 0;
        K key{};
        S val = M::e(), prod = M::e();
        i32 size = 0;
        u32 prio = 0;

        node() = default;
        explicit node(const K& k, const S& v) : key(k), val(v), prod(v), size(1), prio(rand32()) {}
    };

    static inline NodePool<node> d;
    static constexpr tree NIL = 0;
    static constexpr Compare cmp{};

private:
    tree root = NIL;

public:
    ProdSortedTreap() = default;

    /**
     * @brief 双方向イテレータ
     */
    class iterator {
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type        = K;
        using difference_type   = isize;
        using pointer           = const K*;
        using reference         = const K&;

    private:
        tree id;
        const ProdSortedTreap* tr;

        friend class ProdSortedTreap;
        iterator(tree id, const ProdSortedTreap* tr) : id(id), tr(tr) {}

    public:
        iterator() : id(NIL), tr(nullptr) {}

        reference operator*() const { return d[id].key; }
        pointer operator->() const { return &d[id].key; }

        iterator& operator++() {
            if (id == NIL) return *this;
            if (d[id].right != NIL) {
                id = d[id].right;
                while (d[id].left != NIL) id = d[id].left;
            } else {
                tree p = d[id].parent;
                while (p != NIL && d[p].right == id) {
                    id = p;
                    p = d[p].parent;
                }
                id = p;
            }
            return *this;
        }

        iterator operator++(int) { iterator tmp = *this; ++(*this); return tmp; }

        iterator& operator--() {
            if (id == NIL) {
                if (tr && tr->root != NIL) {
                    id = tr->root;
                    while (d[id].right != NIL) id = d[id].right;
                }
                return *this;
            }
            if (d[id].left != NIL) {
                id = d[id].left;
                while (d[id].right != NIL) id = d[id].right;
            } else {
                tree p = d[id].parent;
                while (p != NIL && d[p].left == id) {
                    id = p;
                    p = d[p].parent;
                }
                id = p;
            }
            return *this;
        }

        iterator operator--(int) { iterator tmp = *this; --(*this); return tmp; }

        bool operator==(const iterator& other) const { return id == other.id; }
        bool operator!=(const iterator& other) const { return id != other.id; }
    };

    /**
     * @brief 最小要素を指すイテレータを取得します。
     */
    iterator begin() const {
        tree curr = root;
        if (curr == NIL) return end();
        while (d[curr].left != NIL) curr = d[curr].left;
        return iterator(curr, this);
    }

    /**
     * @brief 番兵（終端）イテレータを取得します。
     */
    iterator end() const { return iterator(NIL, this); }

    i32 size() const { return size_(root); }
    bool empty() const { return root == NIL; }

    /**
     * @brief キーと値のペアを挿入します（重複を許容します）
     */
    void insert(const K& key, const S& val = M::e()) {
        auto [l, r] = split_lt(root, key);
        root = merge(merge(l, d.new_node(node(key, val))), r);
        if (root != NIL) d[root].parent = NIL;
    }

    /**
     * @brief 指定したキーに一致する要素を1つ削除します。存在しない場合は何もしません。
     */
    void erase(const K& key) {
        auto [l, r] = split_lt(root, key);
        if (r == NIL) {
            root = l;
            if (root != NIL) d[root].parent = NIL;
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
        if (root != NIL) d[root].parent = NIL;
    }

    /**
     * @brief 指定したキーに一致する要素を全て削除します。
     */
    void erase_all(const K& key) {
        auto [l, r] = split_lt(root, key);
        auto [m, rr] = split_le(r, key);
        root = merge(l, rr);
        if (root != NIL) d[root].parent = NIL;
    }

    /**
     * @brief `[lower, upper)` の範囲に含まれるキーを持つ要素の区間積を返します。
     */
    S prod(const K& lower, const K& upper) {
        auto [l, r1] = split_lt(root, lower);
        auto [mid, r2] = split_lt(r1, upper);
        S res = prod_(mid);
        root = merge(merge(l, mid), r2);
        return res;
    }

    /**
     * @brief 全要素の積を返します。
     */
    S all_prod() const { return prod_(root); }

    /**
     * @brief 指定したキーに値を設定します（既存の同一キー要素は全て削除されます）。Map的な挙動をします。
     */
    void set(const K& key, const S& val) {
        erase_all(key);
        insert(key, val);
    }

    /**
     * @brief 指定したキー x が含まれているか判定します。
     */
    bool contains(const K& x) const {
        auto it = lower_bound(x);
        if (it == end()) return false;
        return key_eq(*it, x);
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

    /**
     * @brief 指定したキー x 以上の最初の要素を指すイテレータを返します。
     */
    iterator lower_bound(const K& x) const {
        tree curr = root;
        tree res = NIL;
        while (curr != NIL) {
            if (!cmp(d[curr].key, x)) {
                res = curr;
                curr = d[curr].left;
            } else {
                curr = d[curr].right;
            }
        }
        return iterator(res, this);
    }

    /**
     * @brief 指定したキー x より真に大きい最初の要素を指すイテレータを返します。
     */
    iterator upper_bound(const K& x) const {
        tree curr = root;
        tree res = NIL;
        while (curr != NIL) {
            if (cmp(x, d[curr].key)) {
                res = curr;
                curr = d[curr].left;
            } else {
                curr = d[curr].right;
            }
        }
        return iterator(res, this);
    }

private:
    static bool key_eq(const K& a, const K& b) { return !cmp(a, b) && !cmp(b, a); }

    static i32 size_(tree t) { return t == NIL ? 0 : d[t].size; }
    static S prod_(tree t) { return t == NIL ? M::e() : d[t].prod; }

    static void update(tree t) {
        if (t == NIL) return;
        node& n = d[t];
        n.size = 1 + size_(n.left) + size_(n.right);
        n.prod = M::op(M::op(prod_(n.left), n.val), prod_(n.right));
        if (n.left != NIL) d[n.left].parent = t;
        if (n.right != NIL) d[n.right].parent = t;
    }

    static std::pair<tree, tree> split_lt(tree t, const K& key) {
        if (t == NIL) return {NIL, NIL};
        if (cmp(d[t].key, key)) {
            auto [a, b] = split_lt(d[t].right, key);
            d[t].right = a;
            update(t);
            if (b != NIL) d[b].parent = NIL;
            return {t, b};
        }
        auto [a, b] = split_lt(d[t].left, key);
        d[t].left = b;
        update(t);
        if (a != NIL) d[a].parent = NIL;
        return {a, t};
    }

    static std::pair<tree, tree> split_le(tree t, const K& key) {
        if (t == NIL) return {NIL, NIL};
        if (!cmp(key, d[t].key)) {
            auto [a, b] = split_le(d[t].right, key);
            d[t].right = a;
            update(t);
            if (b != NIL) d[b].parent = NIL;
            return {t, b};
        }
        auto [a, b] = split_le(d[t].left, key);
        d[t].left = b;
        update(t);
        if (a != NIL) d[a].parent = NIL;
        return {a, t};
    }

    static std::pair<tree, tree> split_idx(tree t, i32 k) {
        if (t == NIL) return {NIL, NIL};
        node& n = d[t];
        if (k <= size_(n.left)) {
            auto [a, b] = split_idx(n.left, k);
            n.left = b;
            update(t);
            if (a != NIL) d[a].parent = NIL;
            return {a, t};
        }
        auto [a, b] = split_idx(n.right, k - size_(n.left) - 1);
        n.right = a;
        update(t);
        if (b != NIL) d[b].parent = NIL;
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
