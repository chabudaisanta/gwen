#pragma once

#include <algorithm>
#include <utility>
#include <vector>
#include <concepts>
#include <cassert>
#include <iterator>
#include "gwen/types.hpp"
#include "gwen/algebra/monoid.hpp"

namespace gwen {

namespace algebra {

template<typename NodePolicy>
concept is_node_policy = requires(const NodePolicy& np) {
    { np.val } -> std::same_as<typename NodePolicy::S>;
};
template<typename NodePolicy>
concept has_prod = requires(const NodePolicy& np) {
    { np.val } -> std::same_as<typename NodePolicy::S>;
    { np.prod } -> std::same_as<typename NodePolicy::S>;
};
template<typename NodePolicy>
concept has_reverse = requires(const NodePolicy& np) {
    { np.rev } -> std::same_as<bool>;
    requires std::same_as<decltype(np.prod), decltype(np.rev_prod)>;
};
template<typename NodePolicy>
concept has_lazy = requires(const NodePolicy& np) {
    { np.lz } -> std::same_as<typename NodePolicy::F>;
};
template<typename NodePolicy>
concept can_update = requires(const NodePolicy& np, const NodePolicy& l, const NodePolicy& r) {
    np.update(l, r);
};

}

template<typename Monoid, typename Act, typename Mapping>
struct range_apply_reverse_prod_node_policy {
    using mytype = range_apply_reverse_prod_node_policy;
    using S = typename Monoid::S;
    using F = typename Act::S;
    S val;
    S prod;
    S rev_prod;
    F lz = Act::e;
    bool rev = false;

    range_apply_reverse_prod_node_policy(const S& x) : val(x), prod(x), rev_prod(x) {}

    inline void update(const mytype& a, const mytype& b) noexcept {
        // a, b and *this has pushed <=> lz = id(), rev = false
        // but rev is difficult(especially swap), it is processed on the data structure
        // rev is always false
        prod = Mapping::op(Mapping::op(a.prod, val), b.prod);
        rev_prod = Mapping::op(Mapping::op(b.rev_prod, val), a.rev_prod);
    }
    inline void eval() noexcept {
        val = Mapping::op(lz, val);
        prod = Mapping::op(lz, prod);
        rev_prod = Mapping::op(lz, rev_prod);
    }
    inline void push(mytype& child) noexcept {
        // why push lz for each child? -> to check NIL
        child.lz = Mapping::op(lz, child.lz);
    }
    inline void clear_lz() noexcept {
        lz = Act::e;
    }
};

namespace internal {

template<typename NodePolicy>

class splay_tree_base {
public:
    using S = typename NodePolicy::S;
    using tree = i32;
    static constexpr tree NIL = 0;
    struct Node : public NodePolicy {
        tree lch = NIL, rch = NIL, par = NIL;
        i32 cnt;

        Node() : NodePolicy(), cnt(0) {} // copy of NIL
        Node(const S& x) : NodePolicy(x), cnt(1) {}
    };
    
    static inline std::vector<Node> d;

    // constructor
    explicit splay_tree_base() {
        if(d.size()) return;
        // NIL
        d.emplace_back(Node());
    }

    // make node / build tree
    static tree new_node(const S& x) {
        tree ret = d.size();
        d.emplace_back(Node(x));
        return ret;
    }

    static inline tree build() {
        return NIL;
    }
    static inline tree build(const S& x) {
        return new_node(x);
    }
    template<std::random_access_iterator Itr>
    static tree build(Itr begin, Itr end) {
        i32 dist = end - begin;
        if(dist == 0) return NIL;
        if(dist == 1) return new_node(*begin);
        Itr mid = begin + dist / 2;
        tree root = new_node(*mid);
        tree lt = build(begin, mid), rt = build(mid + 1, end);
        if(lt) d[lt].par = root;
        if(rt) d[rt].par = root;
        d[root].lch = lt;
        d[root].rch = rt;
        update(root);
        return root;
    }


    // basic utility
    static inline i32 size(tree t) noexcept {
        return d[t].cnt;
    }
    static inline bool empty(tree t) noexcept {
        return !t;
    }

    // bound
protected:
    static std::pair<tree, tree> bound(tree t, auto&& cond) {
        tree lt = NIL, rt = NIL;
        push(t);
        while (t) {
            push(d[t].lch);
            push(d[t].rch);
            auto dir = cond(t);
            if (dir == 0) {
                return {lt, t};
            }

            if (dir < 0) {
                rt = t;
                t = d[t].lch;
            }
            else {
                lt = t;
                t = d[t].rch;
            }
        }
        return {lt, rt};
    }
    // ex) bound is {k-1th | kth}
    // ancestors of k-1th or kth and their children has pushed (https://qiita.com/ngtkana/items/4d0b84d45210771aa074)
    // tree structure remains unchanged
    // return k-1th element node and kth element node

    // splay
protected:
    // only splay
    // ancestors of t must have pushed
    static tree splay(tree t) {
        if (!t) return t;
        push(t);
        while (d[t].par) {
            tree p = d[t].par;
            if (!d[p].par) {
                // zig
                if (d[p].lch == t)
                    rotr(t);
                else
                    rotl(t);
                update(p);
                update(t);
                return t;
            }

            tree g = d[p].par;
            if (d[p].lch == t) {
                if (d[g].lch == p) { // zig-zig
                    rotr(p);
                    rotr(t);
                }
                else { // zig-zag
                    rotr(t);
                    rotl(t);
                }
            }
            else {
                if (d[g].lch == p) { // zig-zag
                    rotl(t);
                    rotr(t);
                }
                else { // zig-zig
                    rotl(p);
                    rotl(t);
                }
            }
            update(g);
            update(p);
            update(t);
        }
        return t;
    }

    // bound -> splay
    static tree splay_head(tree t) {
        return splay(bound(t, [](tree){ return -1; }).second);
    }
    static tree splay_tail(tree t) {
        return splay(bound(t, [](tree){ return 1; }).second);
    }
    static tree splay_at(tree t, i32 p) {
        assert(0 <= p && p < size(t));
        return splay(bound(t, [&](tree cur) {
            if(p < pos(cur)) return -1;
            p -= pos(cur);
            if(p == 0) return 0;
            p--;
            return 1;
        }).second);
    }

    // merge
public:
    static tree merge(tree lt, tree rt){
        if(!lt || !rt) return lt ? lt : rt;
        assert(!d[lt].par && !d[rt].par);
        if(size(lt) < size(rt)) {
            rt = splay_head(rt); // include bound(push)
            // rt has pushed
            // rt.lch is NIL
            d[rt].lch = lt;
            d[lt].par = rt;
            update(rt);
            return rt;
        }
        else {
            lt = splay_tail(lt);
            // same as above
            d[lt].rch = rt;
            d[rt].par = lt;
            update(lt);
            return lt;
        }
    }
    static tree merge3(tree lt, tree mt, tree rt) {
        return merge(merge(lt, mt), rt);
    }

    // split
public:
    static std::pair<tree,tree> split(tree t, auto&& cond) {
        assert(!d[t].par);
        tree rt = bound(t, cond).second;
        if(!rt) return {t, NIL};
        splay(rt); // rt becomes the new root of the subtree originally represented as t
        // rt has pushed
        if(!d[rt].lch) return {NIL, rt};

        tree lt = d[rt].lch;
        d[lt].par = NIL;
        d[rt].lch = NIL;
        update(rt);
        return {lt, rt};
    }

    static std::pair<tree,tree> split_at(tree t, i32 p) {
        assert(0 <= p && p <= size(t));
        if(p == 0) return {NIL, t};
        if(p == size(t)) return {t, NIL};

        return split(t, [&](tree cur) {
            if (p < pos(cur)) return -1;
            p -= pos(cur);
            if (p == 0) return 0;
            p--;
            return 1;
        });
    }
    static std::tuple<tree,tree,tree> split3_at(tree t, i32 l, i32 r) {
        assert(0 <= l && l <= r && r <= size(t));
        auto [lt, mrt] = split_at(t, l);
        auto [mt, rt] = split_at(mrt, r - l);
        return {lt, mt, rt};
    }

    // erase
    static tree erase(tree t, auto&& cond) {
        assert(t); // erase on NIL(empty tree) is invalid
        t = splay(bound(t, cond).second);
        if(!t) return t; // not assert(for erase by key)
        tree lt = d[t].lch, rt = d[t].rch;
        d[t].lch = d[t].rch = NIL;
        if(lt) d[lt].par = NIL;
        if(rt) d[rt].par = NIL;
        // lt and rt is valid
        return merge(lt, rt);
    }

    // insert
    static tree insert(tree t, auto&& cond, const S& x) {
        tree newbie = new_node(x);
        if(!t) return newbie;

        tree p = NIL;
        push(t);
        auto dir = 0; // ここでは宇宙演算子は使えません...
        while (t) {
            push(d[t].lch);
            push(d[t].rch);
            p = t;
            dir = cond(t);

            if (dir < 0) {
                t = d[t].lch;
            }
            else {
                t = d[t].rch;
            }
        }
        if(dir < 0) {
            d[p].lch = newbie;
        }
        else {
            d[p].rch = newbie;
        }
        d[newbie].par = p;
        update(p);
        return splay(newbie);
    }

    // query
    static inline void all_reverse(tree t) requires algebra::has_reverse<NodePolicy> {
        d[t].rev ^= true;
    }
    static tree reverse(tree t, i32 l, i32 r) requires algebra::has_reverse<NodePolicy> {
        assert(0 <= l && l <= r && r <= size(t));
        if(l >= r) return t;
        auto [lt, mt, rt] = split3_at(t, l, r);
        all_reverse(mt);
        return merge3(lt, mt, rt);
    }

    static inline void all_prod(tree t) requires algebra::has_prod<NodePolicy> {

    }
    static std::pair<tree,S> prod(tree t, i32 l, i32 r) requires algebra::has_prod<NodePolicy> {

    }

    static inline void all_apply(tree t) requires algebra::has_lazy<NodePolicy> {
        
    }
    static inline tree apply(tree t) requires algebra::has_lazy<NodePolicy> {

    }

    // internal
private:
    static void rotl(tree t) noexcept {
        // when called, p must not be NIL
        tree p = d[t].par;
        tree g = d[p].par;
        if (g) (d[g].lch == p ? d[g].lch : d[g].rch) = t;
        d[p].par = t;
        d[p].rch = d[t].lch;
        if (d[t].lch) d[d[t].lch].par = p;
        d[t].par = g;
        d[t].lch = p;
    }
    static void rotr(tree t) noexcept {
        tree p = d[t].par;
        tree g = d[p].par;
        if (g) (d[g].lch == p ? d[g].lch : d[g].rch) = t;
        d[p].par = t;
        d[p].lch = d[t].rch;
        if (d[t].rch) d[d[t].rch].par = p;
        d[t].par = g;
        d[t].rch = p;
    }

    static inline void update(tree t) noexcept {
        if constexpr (algebra::can_update<NodePolicy>) d[t].update(d[d[t].lch], d[d[t].rch]);
        d[t].cnt = d[d[t].lch].cnt + 1 + d[d[t].rch].cnt;
    }
    static inline void push(tree t) noexcept {
        if(!t) return;
        if constexpr (algebra::has_reverse<NodePolicy>) if(d[t].rev) {
            std::swap(d[t].lch, d[t].rch);
            if constexpr (algebra::has_prod<NodePolicy>) std::swap(d[t].prod, d[t].rev_prod);
            if(d[t].lch) d[d[t].lch].rev ^= true;
            if(d[t].rch) d[d[t].rch].rev ^= true;
            d[t].rev = false;
            // push rev is complete
        }
        if constexpr (algebra::has_lazy<NodePolicy>) {
            d[t].eval();
            if(d[t].lch) d[t].push(d[d[t].lch]);
            if(d[t].rch) d[t].push(d[d[t].rch]);
            d[t].clear_lz();
        }
    }

    static inline i32 pos(tree t) noexcept {
        return size(d[t].lch);
    }
};
} // namespace internal


template<typename NodePolicy>
class splay_tree_sequence : private internal::splay_tree_base<NodePolicy> {
    using base = internal::splay_tree_base<NodePolicy>;
    using typename base::tree;
    using base::NIL;
    using base::d;

    tree root;
    explicit splay_tree_sequence() : root(NIL) {}

};

} // namespace gwen