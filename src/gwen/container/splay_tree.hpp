#pragma once

#include <utility>
#include <vector>

#include "gwen/algebra/monoid.hpp"
#include "gwen/types.hpp"
namespace gwen {
namespace internal {

template <acted_monoid AM> class splay_tree_impl {
public:
    using S = AM::monoid::S;
    using F = AM::act::S;

private:
    static constexpr i32 NIL = 0;
    using tree = i32;
    struct node {
        tree par;
        tree lch;
        tree rch;
        S val;
        S prod;
        F lz;
        i32 cnt;
    };
    std::vector<node> d;
    AM am;

public:
    //------------------------------------
    //  constructor
    //------------------------------------
    explicit splay_tree_impl(const AM& am_) : am(am_) {
        d[new_node(am.monoid.e)].cnt = 0;  // NIL
    }

    //------------------------------------
    //  make node / tree
    //------------------------------------
private:
    tree new_node(const S& x) {
        tree ret = d.size();
        d.emplace_back(node{
            .par = NIL,
            .lch = NIL,
            .rch = NIL,
            .val = x,
            .prod = x,
            .lz = am.act.e,
            .cnt = 1,
        });
        return ret;
    }

public:
    tree build() { return NIL; }
    tree build(const S& x) { return new_node(x); }

    //------------------------------------
    //  utility
    //------------------------------------
public:
    inline i32 size(tree t) const { return d[t].cnt; }
    inline bool empty(tree t) const { return !t; }

    //------------------------------------
    //  bound
    //------------------------------------
private:
    std::pair<tree, tree> bound(tree t, auto&& cond) {
        tree l = NIL, r = NIL;
        while (t) {
            push(t);
            auto dir = cond(t);
            if (dir == 0) {
                return {l, t};
            }

            if (dir < 0) {
                r = t;
                t = d[t].lch;
            }
            else {
                l = t;
                t = d[t].rch;
            }
        }
        return {l, r};
    }

    //------------------------------------
    //  splay
    //------------------------------------
private:
    tree splay(tree t) {
        if (!t) return t;
        push(t);
        while (d[t].par) {
            tree p = d[t].par;
            if (!d[p].par) {
                // zig
                d[t].cnt = std::move(d[p].cnt);
                d[t].prod = std::move(d[p].prod);
                if (d[p].lch == t)
                    rotr(t);
                else
                    rotl(t);
                update(p);
                return t;
            }

            tree g = d[p].par;
            d[t].cnt = std::move(d[g].cnt);
            d[t].prod = std::move(d[g].prod);
            if (d[p].lch == t) {
                if (d[g].lch == p) {
                    // zig-zig
                    rotr(p);
                    update(g);
                    rotr(t);
                    update(p);
                }
                else {
                    // zig-zag
                    rotr(t);
                    update(p);
                    rotl(t);
                    update(g);
                }
            }
            else {
                if (d[g].lch == p) {
                    // zig-zag
                    rotl(t);
                    update(p);
                    rotr(t);
                    update(g);
                }
                else {
                    // zig-zig
                    rotl(p);
                    update(g);
                    rotl(t);
                    update(p);
                }
            }
        }
        return t;
    }

public:
    tree splay_head(tree t) {
        return splay(bound(t, [](tree) { return -1; }).second);
    }
    tree splay_tail(tree t) {
        return splay(bound(t, [](tree) { return 1; }).first);
    }
    tree splay_at(tree t, i32 p) {
        return splay(bound(t, [&](tree cur) {
            if (p < pos(cur)) return -1;
            p -= pos(cur);
            if (p == 0) return 0;
            p--;
            return 1;
        }));
    }
    //------------------------------------
    //  split
    //------------------------------------
private:
    std::pair<tree, tree> split(tree t, auto&& cond) {
        tree r = bound(t, cond).second;
        if (!r) return {t, NIL};  // r = NIL <-> bound = end
        splay(r);
        tree l = d[r].lch;
        if (!l) return {NIL, r};  // l = NIL <-> bound = begin
        d[l].par = NIL;
        d[r].lch = NIL;
        update(r);
        return {l, r};
    }

public:
    std::pair<tree, tree> split_at(tree t, i32 p) {
        assert(0 <= p && p <= size(t));
        if (p == 0) return {NIL, t};
        if (p == size(t)) return {t, NIL};

        return split(t, [&](tree cur) {
            if (p < pos(cur)) return -1;
            p -= pos(cur);
            if (p == 0) return 0;
            p--;
            return 1;
        });
    }
    std::tuple<tree, tree, tree> split3(tree t, i32 l, i32 r) {
        assert(0 <= l && l <= r && r <= size(t));
        auto [lt, mrt] = split(t, l);
        auto [mt, rt] = split(mr, r - l);
        return {lt, mt, rt};
    }

    //------------------------------------
    //  merge
    //------------------------------------
public:
    tree merge(tree l, tree r) {
        if (!l || !r) return l ? l : r;
        if (size(l) < size(r)) {
            r = splay_head(r);
            d[r].lch = l;
            d[l].par = r;
            update(r);
            return r;
        }
        else {
            l = splay_tail(l);
            d[l].rch = r;
            d[r].par = l;
            update(l);
            return l;
        }
    }
    tree merge3(tree l, tree m, tree r) { return merge(merge(l, m), r); }

    //------------------------------------
    //  query
    //------------------------------------
public:
    inline S all_prod(tree t) const { return am.mapping(d[t].lz, d[t].prod); }

    //------------------------------------
    //  internal utility
    //------------------------------------
private:
    inline void push(tree t) {
        node& nt = d[t];
        nt.val = am.mapping(nt.lz, nt.val);
        nt.prod = am.mapping(nt.lz, nt.prod);

        if (nt.lch) d[nt.lch].lz = am.act.op(nt.lz, d[nt.lch].lz);
        if (nt.rch) d[nt.rch].lz = am.act.op(nt.lz, d[nt.rch].lz);
        nt.lz = am.act.e;
    }

    inline S op3(const S& a, const S& b, const S& c) const {
        return am.monoid.op(am.monoid.op(a, b), c);
    }
    inline void update(tree t) {
        // nt.lz must be am.act.e
        node& nt = d[t];
        nt.cnt = d[nt.lch].cnt + 1 + d[nt.rch].cnt;
        nt.prod = op3(all_prod(nt.lch), nt.val, all_prod(nt.rch));
    }

    inline void rotl(tree t) {
        tree p = d[t].par;
        tree g = d[p].par;
        if (g) (d[g].lch == p ? d[g].lch : d[g].rch) = t;
        d[p].par = t;
        d[p].rch = d[t].lch;
        d[d[t].lch].par = p;
        d[t].par = g;
        d[t].lch = p;
    }
    inline void rotr(tree t) {
        tree p = d[t].par;
        tree g = d[p].par;
        if (g) (d[g].lch == p ? d[g].lch : d[g].rch) = t;
        d[p].par = t;
        d[p].lch = d[t].rch;
        d[d[t].rch].par = p;
        d[t].par = g;
        d[t].rch = p;
    }

    inline i32 pos(tree t) const { return d[d[t].lch].cnt; }
};
}  // namespace internal

}  // namespace gwen