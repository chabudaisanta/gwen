#pragma once

#include <cassert>
#include <functional>
#include <optional>
#include <utility>
#include <vector>

#include "gwen/types.hpp"
#include "gwen/utils/node_pool.hpp"
#include "gwen/utils/xorshift.hpp"

namespace gwen {

/**
 * @brief キーベースの順序付き多重集合（Treap）
 * @details モノイドを持たず、キーの大小で二分探索ツリーを構築します。
 */
template <typename K, typename Compare = std::less<K>> class SortedTreap {
public:
    using tree = i32;

    struct node {
        tree left = 0, right = 0, parent = 0;
        K key{};
        i32 size = 0;
        u32 prio = 0;

        node() = default;
        explicit node(const K& k) : key(k), size(1), prio(rand32()) {}
    };

    static inline NodePool<node> d;
    static constexpr tree NIL = 0;
    static constexpr Compare cmp{};

private:
    tree root = NIL;

public:
    SortedTreap() = default;

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
        const SortedTreap* tr;

        friend class SortedTreap;
        iterator(tree id, const SortedTreap* tr) : id(id), tr(tr) {}

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

    void insert(const K& key) {
        auto [l, r] = split_lt(root, key);
        root = merge(merge(l, d.new_node(node(key))), r);
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
            root = merge(l, rr);  // erase m
            d.free_node(m);
        }
        else {
            root = merge(merge(l, m), rr);  // put it back
        }
        if (root != NIL) d[root].parent = NIL;
    }

    /**
     * @brief 0-indexed で k 番目に小さいキーを返します。
     */
    K kth(i32 k) {
        assert(0 <= k && k < size());
        tree l, mid, r;
        std::tie(l, mid) = split_idx(root, k);
        std::tie(mid, r) = split_idx(mid, 1);
        K res = d[mid].key;
        root = merge(merge(l, mid), r);
        return res;
    }

    /**
     * @brief 指定したキー x が含まれているか判定します。
     */
    bool contains(const K& x) const {
        auto it = lower_bound(x);
        if (it == end()) return false;
        return key_eq(*it, x);
    }

    /**
     * @brief key 未満の要素数を返します。
     */
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

    static K get_key(tree id) {
        assert(id != NIL);
        return d[id].key;
    }

    std::vector<K> to_vec() const {
        std::vector<K> res;
        res.reserve(size());
        to_vec_(root, res);
        return res;
    }

private:
    static bool key_eq(const K& a, const K& b) { return !cmp(a, b) && !cmp(b, a); }

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
        if (d[t].left != NIL) d[d[t].left].parent = t;
        if (d[t].right != NIL) d[d[t].right].parent = t;
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
