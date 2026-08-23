#pragma once

#include <cassert>
#include <format>
#include <functional>
#include <string>
#include <utility>
#include <vector>

#include "gwen/dump.hpp"
#include "gwen/types.hpp"
#include "gwen/utils/node_pool.hpp"
#include "gwen/utils/xorshift.hpp"

namespace gwen {

/**
 * @brief キーベースの順序付き多重集合（Treap）
 * @details モノイドを持たず、キーの大小で二分探索ツリーを構築する。
 * @tparam K キー型
 * @tparam Compare キー比較型
 */
template <typename K, typename Compare = std::less<K>> class SortedTreap {
public:
    using tree = i32;

    /** @brief Treapの内部ノード。 */
    struct node {
        tree left = 0, right = 0, parent = 0;
        K key{};
        i32 size = 0;
        u32 prio = 0;

        /** @brief 空ノードを構築する。 */
        node() = default;

        /**
         * @brief キーを持つ単一ノードを構築する。
         * @param k 保持するキー
         */
        explicit node(const K& k) : key(k), size(1), prio(rand32()) {}
    };

    static inline NodePool<node> d;
    static constexpr tree NIL = 0;
    static constexpr Compare cmp{};

private:
    tree root = NIL;

public:
    /** @brief 空のTreapを構築する。 */
    SortedTreap() = default;

    /**
     * @brief 双方向イテレータ
     */
    class iterator {
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = K;
        using difference_type = isize;
        using pointer = const K*;
        using reference = const K&;

    private:
        tree id;
        const SortedTreap* tr;

        friend class SortedTreap;
        iterator(tree id, const SortedTreap* tr) : id(id), tr(tr) {}

    public:
        /** @brief 終端を指すデフォルトイテレータを構築する。 */
        iterator() : id(NIL), tr(nullptr) {}

        /**
         * @brief 指しているキーへの参照を返す。
         * @return キーへの参照
         */
        reference operator*() const { return d[id].key; }

        /**
         * @brief 指しているキーへのポインタを返す。
         * @return キーへのポインタ
         */
        pointer operator->() const { return &d[id].key; }

        /**
         * @brief 次の要素へ進める。
         * @return 更新後のイテレータへの参照
         */
        iterator& operator++() {
            if (id == NIL) return *this;
            if (d[id].right != NIL) {
                id = d[id].right;
                while (d[id].left != NIL) id = d[id].left;
            }
            else {
                tree p = d[id].parent;
                while (p != NIL && d[p].right == id) {
                    id = p;
                    p = d[p].parent;
                }
                id = p;
            }
            return *this;
        }

        /**
         * @brief 現在位置を保持したイテレータを返し、次の要素へ進める。
         * @return 更新前のイテレータ
         */
        iterator operator++(int) {
            iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        /**
         * @brief 前の要素へ戻す。終端では最大要素へ移動する。
         * @return 更新後のイテレータへの参照
         */
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
            }
            else {
                tree p = d[id].parent;
                while (p != NIL && d[p].left == id) {
                    id = p;
                    p = d[p].parent;
                }
                id = p;
            }
            return *this;
        }

        /**
         * @brief 現在位置を保持したイテレータを返し、前の要素へ戻す。
         * @return 更新前のイテレータ
         */
        iterator operator--(int) {
            iterator tmp = *this;
            --(*this);
            return tmp;
        }

        /**
         * @brief 2つのイテレータが同じノードを指すか判定する。
         * @param other 比較対象
         * @return 同じノードを指せばtrue
         */
        bool operator==(const iterator& other) const { return id == other.id; }

        /**
         * @brief 2つのイテレータが異なるノードを指すか判定する。
         * @param other 比較対象
         * @return 異なるノードを指せばtrue
         */
        bool operator!=(const iterator& other) const { return id != other.id; }
    };

    /**
     * @brief 最小要素を指すイテレータを返す。
     * @return 最小要素を指すイテレータ。空ならend()
     */
    iterator begin() const {
        tree curr = root;
        if (curr == NIL) return end();
        while (d[curr].left != NIL) curr = d[curr].left;
        return iterator(curr, this);
    }

    /**
     * @brief 終端イテレータを返す。
     * @return 終端イテレータ
     */
    iterator end() const { return iterator(NIL, this); }

    /**
     * @brief 要素数を返す。
     * @return 要素数
     */
    i32 size() const { return size_(root); }

    /**
     * @brief Treapが空かどうかを返す。
     * @return 空ならtrue
     */
    bool empty() const { return root == NIL; }

    /**
     * @brief キーを挿入する。
     * @param key 挿入するキー
     */
    void insert(const K& key) {
        auto [l, r] = split_lt(root, key);
        root = merge(merge(l, d.new_node(node(key))), r);
        if (root != NIL) d[root].parent = NIL;
    }

    /**
     * @brief 指定キーと等価な要素を1つ削除する。存在しなければ何もしない。
     * @param key 削除するキー
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
     * @brief 0-indexedでk番目に小さいキーを返す。
     * @details 計算量は期待 O(log N)。
     * @param k 取得するキーの順位
     * @return k 番目に小さいキー
     * @pre `0 <= k && k < size()`
     */
    K kth(i32 k) const {
        assert(0 <= k && k < size());
        tree t = root;
        while (t != NIL) {
            i32 left_size = size_(d[t].left);
            if (k < left_size) {
                t = d[t].left;
            }
            else if (k == left_size) {
                return d[t].key;
            }
            else {
                k -= left_size + 1;
                t = d[t].right;
            }
        }
        assert(false);
        return K{};
    }

    /**
     * @brief 指定キーと等価な要素が存在するか判定する。
     * @param x 検索するキー
     * @return 存在すればtrue
     */
    bool contains(const K& x) const {
        auto it = lower_bound(x);
        if (it == end()) return false;
        return key_eq(*it, x);
    }

    /**
     * @brief key未満の要素数を返す。
     * @details 計算量は期待 O(log N)。
     * @param key 上限となるキー
     * @return key 未満の要素数
     */
    i32 count_lower(const K& key) const { return count_lt_(root, key); }

    /**
     * @brief 指定キーと等価な要素数を返す。
     * @details Compareによって等価と判定される要素を数える。計算量は期待 O(log N)。
     * @param x 数えるキー
     * @return x と等価な要素数
     */
    i32 count(const K& x) const { return count_le_(root, x) - count_lt_(root, x); }

    /**
     * @brief キー区間 [lower, upper) に含まれる要素数を返す。
     * @details `lower < upper`でなければ0を返す。計算量は期待 O(log N)。
     * @param lower 区間の下限
     * @param upper 区間の上限
     * @return 区間内の要素数
     */
    i32 count(const K& lower, const K& upper) const {
        if (!cmp(lower, upper)) return 0;
        return count_lt_(root, upper) - count_lt_(root, lower);
    }

    /**
     * @brief x以上の最初の要素を指すイテレータを返す。
     * @param x 検索するキー
     * @return 条件を満たす最初の要素。存在しなければend()
     */
    iterator lower_bound(const K& x) const {
        tree curr = root;
        tree res = NIL;
        while (curr != NIL) {
            if (!cmp(d[curr].key, x)) {
                res = curr;
                curr = d[curr].left;
            }
            else {
                curr = d[curr].right;
            }
        }
        return iterator(res, this);
    }

    /**
     * @brief xより大きい最初の要素を指すイテレータを返す。
     * @param x 検索するキー
     * @return 条件を満たす最初の要素。存在しなければend()
     */
    iterator upper_bound(const K& x) const {
        tree curr = root;
        tree res = NIL;
        while (curr != NIL) {
            if (cmp(x, d[curr].key)) {
                res = curr;
                curr = d[curr].left;
            }
            else {
                curr = d[curr].right;
            }
        }
        return iterator(res, this);
    }

    /**
     * @brief ノードIDに対応するキーを返す。
     * @param id ノードID
     * @return 対応するキー
     * @pre `id != NIL`
     */
    static K get_key(tree id) {
        assert(id != NIL);
        return d[id].key;
    }

    /**
     * @brief 全キーを昇順に格納した配列を返す。
     * @return 全キーの配列
     */
    std::vector<K> to_vec() const {
        std::vector<K> res;
        res.reserve(size());
        to_vec_(root, res);
        return res;
    }

    /**
     * @brief デバッグ用の文字列表現を返す。
     * @return Treapの文字列表現
     */
    std::string dump() const {
        std::vector<K> vec = to_vec();
        return std::format("SortedTreap{{\n  size = {},\n  data = {},\n}}", size(), internal::format_range(vec));
    }

private:
    static bool key_eq(const K& a, const K& b) { return !cmp(a, b) && !cmp(b, a); }

    static i32 size_(tree t) { return t == NIL ? 0 : d[t].size; }

    static i32 count_lt_(tree t, const K& key) {
        i32 result = 0;
        while (t != NIL) {
            if (cmp(d[t].key, key)) {
                result += size_(d[t].left) + 1;
                t = d[t].right;
            }
            else {
                t = d[t].left;
            }
        }
        return result;
    }

    static i32 count_le_(tree t, const K& key) {
        i32 result = 0;
        while (t != NIL) {
            if (!cmp(key, d[t].key)) {
                result += size_(d[t].left) + 1;
                t = d[t].right;
            }
            else {
                t = d[t].left;
            }
        }
        return result;
    }

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
