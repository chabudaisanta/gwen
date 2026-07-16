#pragma once

#include <cassert>
#include <utility>
#include <vector>

#include "gwen/utils/node_pool.hpp"
#include "gwen/utils/xorshift.hpp"
#include "gwen/types.hpp"

namespace gwen {

/**
 * @brief インデックスベースの動的配列（Treap）
 * @details 値の保持と反転のみをサポートする最速のTreap。モノイド演算は持たない。
 */
template <typename T>
class ImplicitTreap {
public:
    using tree = i32;

    struct node {
        tree left = 0, right = 0;
        T val;
        i32 size = 0;
        u32 prio = 0;
        bool rev = false;

        node() = default;
        explicit node(const T& v) : val(v), size(1), prio(rand32()) {}
    };

    static inline NodePool<node> d;
    static constexpr tree NIL = 0;

private:
    tree root = NIL;

public:
    ImplicitTreap() = default;

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

    i32 size() const { return size_(root); }
    bool empty() const { return root == NIL; }

    void insert(i32 pos, const T& x) {
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

    T get(i32 pos) {
        assert(0 <= pos && pos < size());
        auto [l, mid_r] = split(root, pos);
        auto [mid, r] = split(mid_r, 1);
        push(mid);
        T res = d[mid].val;
        root = merge(merge(l, mid), r);
        return res;
    }

    void set(i32 pos, const T& x) {
        assert(0 <= pos && pos < size());
        auto [l, mid_r] = split(root, pos);
        auto [mid, r] = split(mid_r, 1);
        push(mid);
        d[mid].val = x;
        update(mid);
        root = merge(merge(l, mid), r);
    }

    void concat(ImplicitTreap& other) {
        root = merge(root, other.root);
        other.root = NIL;
    }

    static ImplicitTreap concat(ImplicitTreap& t0, ImplicitTreap& t1) {
        ImplicitTreap r;
        r.root = merge(t0.root, t1.root);
        t0.root = t1.root = NIL;
        return r;
    }

    void push_back(const T& x) { insert(size(), x); }
    void push_front(const T& x) { insert(0, x); }

    std::vector<T> to_vec() {
        std::vector<T> res;
        res.reserve(size());
        to_vec_(root, res);
        return res;
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
        push(n.left);
        push(n.right);
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

    static void to_vec_(tree t, std::vector<T>& out) {
        if (t == NIL) return;
        push(t);
        to_vec_(d[t].left, out);
        out.push_back(d[t].val);
        to_vec_(d[t].right, out);
    }
};

} // namespace gwen
