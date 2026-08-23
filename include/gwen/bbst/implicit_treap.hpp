#pragma once

#include <cassert>
#include <format>
#include <string>
#include <utility>
#include <vector>

#include "gwen/dump.hpp"
#include "gwen/types.hpp"
#include "gwen/utils/node_pool.hpp"
#include "gwen/utils/xorshift.hpp"

namespace gwen {

/**
 * @brief インデックスベースの動的配列（Treap）
 * @details 値の保持と反転のみをサポートする最速のTreap。モノイド演算は持たない。
 * @tparam T 要素型
 */
template <typename T> class ImplicitTreap {
public:
    using tree = i32;

    /** @brief Treapの内部ノード。 */
    struct node {
        tree left = 0, right = 0;
        T val;
        i32 size = 0;
        u32 prio = 0;
        bool rev = false;

        /** @brief 空ノードを構築する。 */
        node() = default;

        /**
         * @brief 値を持つ単一ノードを構築する。
         * @param v 保持する値
         */
        explicit node(const T& v) : val(v), size(1), prio(rand32()) {}
    };

    static inline NodePool<node> d;
    static constexpr tree NIL = 0;

private:
    tree root = NIL;

public:
    /** @brief 空のTreapを構築する。 */
    ImplicitTreap() = default;

    /**
     * @brief 配列からTreapを構築する。
     * @details 計算量は期待 O(N)。
     * @param vec 初期要素の配列
     */
    explicit ImplicitTreap(const std::vector<T>& vec) {
        if (vec.empty()) return;
        std::vector<tree> nodes;
        nodes.reserve(vec.size());
        for (const T& x : vec) nodes.push_back(d.new_node(node(x)));

        tree r = NIL;
        std::vector<tree> st;
        st.reserve(vec.size());
        for (tree t : nodes) {
            tree last = NIL;
            while (!st.empty() && d[st.back()].prio < d[t].prio) {
                last = st.back();
                st.pop_back();
            }
            d[t].left = last;
            if (!st.empty())
                d[st.back()].right = t;
            else
                r = t;
            st.push_back(t);
        }
        root = r;
        update_all(root);
    }

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
     * @brief 指定位置に要素を挿入する。
     * @param pos 挿入位置
     * @param x 挿入する要素
     * @pre `0 <= pos && pos <= size()`
     */
    void insert(i32 pos, const T& x) {
        assert(0 <= pos && pos <= size());
        auto [l, r] = split(root, pos);
        root = merge(merge(l, d.new_node(node(x))), r);
    }

    /**
     * @brief 指定位置の要素を削除する。
     * @param pos 削除位置
     * @pre `0 <= pos && pos < size()`
     */
    void erase(i32 pos) {
        assert(0 <= pos && pos < size());
        auto [l, r] = split(root, pos);
        auto [m, rr] = split(r, 1);
        root = merge(l, rr);
        d.free_node(m);
    }

    /**
     * @brief 区間 [l, r) の要素順を反転する。
     * @param l 区間の左端
     * @param r 区間の右端
     * @pre `0 <= l && l <= r && r <= size()`
     */
    void reverse(i32 l, i32 r) {
        assert(0 <= l && l <= r && r <= size());
        if (l >= r) return;
        auto [left, mid_r] = split(root, l);
        auto [mid, right] = split(mid_r, r - l);
        d[mid].rev ^= true;
        root = merge(merge(left, mid), right);
    }

    /**
     * @brief 指定位置の要素を返す。
     * @param pos 取得位置
     * @return 指定位置の要素
     * @pre `0 <= pos && pos < size()`
     */
    T get(i32 pos) {
        assert(0 <= pos && pos < size());
        auto [l, mid_r] = split(root, pos);
        auto [mid, r] = split(mid_r, 1);
        push(mid);
        T res = d[mid].val;
        root = merge(merge(l, mid), r);
        return res;
    }

    /**
     * @brief 指定位置の要素を更新する。
     * @param pos 更新位置
     * @param x 新しい要素
     * @pre `0 <= pos && pos < size()`
     */
    void set(i32 pos, const T& x) {
        assert(0 <= pos && pos < size());
        auto [l, mid_r] = split(root, pos);
        auto [mid, r] = split(mid_r, 1);
        push(mid);
        d[mid].val = x;
        update(mid);
        root = merge(merge(l, mid), r);
    }

    /**
     * @brief 別のTreapを末尾に連結する。
     * @details 連結後、other は空になる。計算量は期待 O(log N)。
     * @param other 連結するTreap
     * @pre `this != &other`
     */
    void concat(ImplicitTreap& other) {
        assert(this != &other);
        if (this == &other) return;
        root = merge(root, other.root);
        other.root = NIL;
    }

    /**
     * @brief 2つのTreapを連結した新しいTreapを返す。
     * @details 連結後、t0 と t1 は空になる。計算量は期待 O(log N)。
     * @param t0 前半のTreap
     * @param t1 後半のTreap
     * @return t0 の後ろに t1 を連結したTreap
     * @pre `&t0 != &t1`
     */
    static ImplicitTreap concat(ImplicitTreap& t0, ImplicitTreap& t1) {
        assert(&t0 != &t1);
        if (&t0 == &t1) return {};
        ImplicitTreap r;
        r.root = merge(t0.root, t1.root);
        t0.root = t1.root = NIL;
        return r;
    }

    /**
     * @brief 末尾に要素を追加する。
     * @param x 追加する要素
     */
    void push_back(const T& x) { insert(size(), x); }

    /**
     * @brief 先頭に要素を追加する。
     * @param x 追加する要素
     */
    void push_front(const T& x) { insert(0, x); }

    /**
     * @brief 全要素を格納した配列を返す。
     * @return 要素を順番に格納した配列
     */
    std::vector<T> to_vec() {
        std::vector<T> res;
        res.reserve(size());
        to_vec_(root, res);
        return res;
    }

    /**
     * @brief デバッグ用の文字列表現を返す。
     * @return Treapの文字列表現
     */
    std::string dump() const {
        std::vector<T> vec = const_cast<ImplicitTreap*>(this)->to_vec();
        return std::format("ImplicitTreap{{\n  size = {},\n  data = {},\n}}", size(), internal::format_range(vec));
    }

private:
    static i32 size_(tree t) { return t == NIL ? 0 : d[t].size; }

    static void push(tree t) {
        if (t == NIL) return;
        node& n = d[t];
        if (n.rev) {
            n.rev = false;
            std::swap(n.left, n.right);
            if (n.left != NIL) d[n.left].rev ^= true;
            if (n.right != NIL) d[n.right].rev ^= true;
        }
    }

    static void update(tree t) {
        if (t == NIL) return;
        node& n = d[t];
        n.size = 1 + size_(n.left) + size_(n.right);
    }

    static void update_all(tree t) {
        if (t == NIL) return;
        update_all(d[t].left);
        update_all(d[t].right);
        update(t);
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

    static void to_vec_(tree t, std::vector<T>& out) {
        if (t == NIL) return;
        push(t);
        to_vec_(d[t].left, out);
        out.push_back(d[t].val);
        to_vec_(d[t].right, out);
    }
};

}  // namespace gwen
