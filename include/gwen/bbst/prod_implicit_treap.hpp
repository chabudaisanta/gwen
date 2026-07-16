#pragma once

#include <cassert>
#include <utility>
#include <vector>

#include "gwen/alge/monoid.hpp"
#include "gwen/types.hpp"
#include "gwen/utils/node_pool.hpp"
#include "gwen/utils/xorshift.hpp"

namespace gwen {

/**
 * @brief 区間積取得が可能なインデックスベースの動的配列（Treap）
 * @details 反転操作（非可換モノイド対応）と区間積取得をサポートします。
 */
template <monoid M, bool Commutative = false> class ProdImplicitTreap {
public:
    using S = typename M::S;
    using tree = i32;

    struct node {
        tree left = 0, right = 0;
        S val = M::e(), prod = M::e(), rev_prod = M::e();
        i32 size = 0;
        u32 prio = 0;
        bool rev = false;

        node() = default;
        explicit node(const S& v) : val(v), prod(v), rev_prod(v), size(1), prio(rand32()) {}
    };

    static inline NodePool<node> d;
    static constexpr tree NIL = 0;

private:
    tree root = NIL;

public:
    ProdImplicitTreap() = default;

    explicit ProdImplicitTreap(const std::vector<S>& vec) {
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
        d.free_node(m);
    }

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

    S prod(i32 l, i32 r) {
        assert(0 <= l && l <= r && r <= size());
        if (l >= r) return M::e();
        tree left, mid, right;
        std::tie(left, mid) = split(root, l);
        std::tie(mid, right) = split(mid, r - l);
        push(mid);
        S res = d[mid].prod;
        root = merge(merge(left, mid), right);
        return res;
    }

    S all_prod() {
        if (empty()) return M::e();
        push(root);
        return d[root].prod;
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

    void concat(ProdImplicitTreap& other) {
        root = merge(root, other.root);
        other.root = NIL;
    }

    static ProdImplicitTreap concat(ProdImplicitTreap& t0, ProdImplicitTreap& t1) {
        ProdImplicitTreap r;
        r.root = merge(t0.root, t1.root);
        t0.root = t1.root = NIL;
        return r;
    }

    void push_back(const S& x) { insert(size(), x); }
    void push_front(const S& x) { insert(0, x); }

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
