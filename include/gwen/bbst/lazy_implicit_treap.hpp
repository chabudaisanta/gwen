#pragma once

#include <cassert>
#include <concepts>
#include <utility>
#include <vector>

#include "gwen/alge/acted_monoid.hpp"
#include "gwen/types.hpp"
#include "gwen/utils/node_pool.hpp"
#include "gwen/utils/xorshift.hpp"

namespace gwen {

/**
 * @brief 遅延評価付きインデックスベース動的配列（Treap）
 * @details 区間作用、区間反転、区間積取得をサポートします。
 * @tparam M 作用付きモノイド（acted_monoid）
 */
template <acted_monoid M, bool Commutative = false> class LazyImplicitTreap {
public:
    using S = typename M::S;
    using F = typename M::F;
    using tree = i32;

    struct node {
        tree left = 0, right = 0;
        S val = M::e(), prod = M::e(), rev_prod = M::e();
        F lz = M::id();
        i32 size = 0;
        u32 prio = 0;
        bool rev = false;
        bool has_lazy = false;

        node() = default;
        explicit node(const S& v) : val(v), prod(v), rev_prod(v), lz(M::id()), size(1), prio(rand32()) {}
    };

    static inline NodePool<node> d;
    static constexpr tree NIL = 0;

private:
    tree root = NIL;

public:
    LazyImplicitTreap() = default;

    /**
     * @brief 配列からTreapを構築します。
     * @param vec 初期要素の配列
     */
    explicit LazyImplicitTreap(const std::vector<S>& vec) {
        if (vec.empty()) return;
        std::vector<tree> nodes;
        nodes.reserve(vec.size());
        for (const S& x : vec) nodes.push_back(d.new_node(node(x)));

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
     * @brief 要素数を取得します。
     */
    i32 size() const { return size_(root); }

    /**
     * @brief 空かどうかを判定します。
     */
    bool empty() const { return root == NIL; }

    /**
     * @brief 指定した位置に要素を挿入します。
     * @param pos 挿入位置
     * @param x 挿入する要素
     */
    void insert(i32 pos, const S& x) {
        assert(0 <= pos && pos <= size());
        auto [l, r] = split(root, pos);
        root = merge(merge(l, d.new_node(node(x))), r);
    }

    /**
     * @brief 指定した位置の要素を削除します。
     * @param pos 削除する位置
     */
    void erase(i32 pos) {
        assert(0 <= pos && pos < size());
        auto [l, r] = split(root, pos);
        auto [m, rr] = split(r, 1);
        root = merge(l, rr);
        d.free_node(m);
    }

    /**
     * @brief 区間 [l, r) の要素を反転させます。
     * @param l 区間の開始位置
     * @param r 区間の終了位置
     */
    void reverse(i32 l, i32 r) {
        assert(0 <= l && l <= r && r <= size());
        if (l >= r) return;
        auto [left, mid_r] = split(root, l);
        auto [mid, right] = split(mid_r, r - l);
        if (mid != NIL) {
            d[mid].rev ^= true;
            if constexpr (!Commutative) std::swap(d[mid].prod, d[mid].rev_prod);
        }
        root = merge(merge(left, mid), right);
    }

    /**
     * @brief 区間 [l, r) の要素に作用素 f を適用します。
     * @param l 区間の開始位置
     * @param r 区間の終了位置
     * @param f 適用する作用素
     */
    void apply(i32 l, i32 r, const F& f) {
        assert(0 <= l && l <= r && r <= size());
        if (l >= r) return;
        auto [left, mid_r] = split(root, l);
        auto [mid, right] = split(mid_r, r - l);
        if (mid != NIL) {
            d[mid].val = M::mapping(f, d[mid].val);
            d[mid].prod = M::mapping(f, d[mid].prod);
            if constexpr (!Commutative) d[mid].rev_prod = M::mapping(f, d[mid].rev_prod);
            d[mid].lz = M::composition(f, d[mid].lz);
            d[mid].has_lazy = true;
        }
        root = merge(merge(left, mid), right);
    }

    /**
     * @brief 全ての要素に作用素 f を適用します。
     * @param f 適用する作用素
     */
    void all_apply(const F& f) {
        if (root != NIL) {
            d[root].val = M::mapping(f, d[root].val);
            d[root].prod = M::mapping(f, d[root].prod);
            if constexpr (!Commutative) d[root].rev_prod = M::mapping(f, d[root].rev_prod);
            d[root].lz = M::composition(f, d[root].lz);
            d[root].has_lazy = true;
        }
    }

    /**
     * @brief 区間 [l, r) の要素の積を取得します。
     * @param l 区間の開始位置
     * @param r 区間の終了位置
     * @return 区間の積
     */
    S prod(i32 l, i32 r) {
        assert(0 <= l && l <= r && r <= size());
        if (l >= r) return M::e();
        tree left, mid, right;
        std::tie(left, mid) = split(root, l);
        std::tie(mid, right) = split(mid, r - l);
        S res = prod_(mid);
        root = merge(merge(left, mid), right);
        return res;
    }

    /**
     * @brief 全ての要素の積を取得します。
     * @return 全要素の積
     */
    S all_prod() {
        if (empty()) return M::e();
        return d[root].prod;
    }

    /**
     * @brief 指定した位置の要素を取得します。
     * @param pos 取得する位置
     * @return 要素の値
     */
    S get(i32 pos) {
        assert(0 <= pos && pos < size());
        return prod(pos, pos + 1);
    }

    /**
     * @brief 指定した位置の要素を書き換えます。
     * @param pos 書き換える位置
     * @param x 新しい値
     */
    void set(i32 pos, const S& x) {
        assert(0 <= pos && pos < size());
        auto [l, r] = split(root, pos);
        auto [m, rr] = split(r, 1);
        push(m);
        d[m].val = x;
        update(m);
        root = merge(merge(l, m), rr);
    }

    /**
     * @brief 別のTreapを末尾に連結します。
     * @param other 連結するTreap
     */
    void concat(LazyImplicitTreap& other) {
        root = merge(root, other.root);
        other.root = NIL;
    }

    /**
     * @brief 2つのTreapを連結した新しいTreapを返します。
     */
    static LazyImplicitTreap concat(LazyImplicitTreap& t0, LazyImplicitTreap& t1) {
        LazyImplicitTreap r;
        r.root = merge(t0.root, t1.root);
        t0.root = t1.root = NIL;
        return r;
    }

    /**
     * @brief 末尾に要素を追加します。
     * @param x 追加する要素
     */
    void push_back(const S& x) { insert(size(), x); }

    /**
     * @brief 先頭に要素を追加します。
     * @param x 追加する要素
     */
    void push_front(const S& x) { insert(0, x); }

    /**
     * @brief 現在のTreapの要素をstd::vectorとして返します。
     */
    std::vector<S> to_vec() {
        std::vector<S> res;
        res.reserve(size());
        to_vec_(root, res);
        return res;
    }

private:
    static i32 size_(tree t) { return t == NIL ? 0 : d[t].size; }
    static S prod_(tree t) { return t == NIL ? M::e() : d[t].prod; }
    static S rev_prod_(tree t) {
        if constexpr (Commutative) return prod_(t);
        return t == NIL ? M::e() : d[t].rev_prod;
    }

    static void push(tree t) {
        if (t == NIL) return;
        node& n = d[t];

        if (n.rev) {
            n.rev = false;
            std::swap(n.left, n.right);
            if (n.left != NIL) {
                d[n.left].rev ^= true;
                if constexpr (!Commutative) std::swap(d[n.left].prod, d[n.left].rev_prod);
            }
            if (n.right != NIL) {
                d[n.right].rev ^= true;
                if constexpr (!Commutative) std::swap(d[n.right].prod, d[n.right].rev_prod);
            }
        }

        if (n.has_lazy) {
            if (n.left != NIL) {
                d[n.left].val = M::mapping(n.lz, d[n.left].val);
                d[n.left].prod = M::mapping(n.lz, d[n.left].prod);
                if constexpr (!Commutative) d[n.left].rev_prod = M::mapping(n.lz, d[n.left].rev_prod);
                d[n.left].lz = M::composition(n.lz, d[n.left].lz);
                d[n.left].has_lazy = true;
            }
            if (n.right != NIL) {
                d[n.right].val = M::mapping(n.lz, d[n.right].val);
                d[n.right].prod = M::mapping(n.lz, d[n.right].prod);
                if constexpr (!Commutative) d[n.right].rev_prod = M::mapping(n.lz, d[n.right].rev_prod);
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
        if constexpr (!Commutative) {
            n.rev_prod = M::op(M::op(rev_prod_(n.right), n.val), rev_prod_(n.left));
        }
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

    static void to_vec_(tree t, std::vector<S>& out) {
        if (t == NIL) return;
        push(t);
        to_vec_(d[t].left, out);
        out.push_back(d[t].val);
        to_vec_(d[t].right, out);
    }
};

}  // namespace gwen
