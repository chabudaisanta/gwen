#pragma once

#include <cassert>
#include <concepts>
#include <iterator>
#include <memory>
#include <tuple>
#include <utility>
#include <vector>

#include "gwen/algebra/monoid.hpp"
#include "gwen/types.hpp"
namespace gwen {
namespace internal {

template <acted_monoid AM> class reversible_splay_tree_impl {
public:
    using S = AM::S;
    using F = AM::F;

private:
    static constexpr i32 NIL = 0;
    using tree = i32;
    struct node {
        tree par;
        tree lch;
        tree rch;
        S val;
        S prod;
        S rev_prod;
        F lz;
        i32 cnt;
        bool rev;
    };
    std::vector<node> d;
    AM am;

public:
    //------------------------------------
    //  constructor
    //------------------------------------
    reversible_splay_tree_impl(const AM& am_, i32 max_size = 1 << 20)
        : am(am_) {
        d.reserve(max_size);
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
            .rev_prod = x,
            .lz = am.act.e,
            .cnt = 1,
            .rev = false,
        });
        return ret;
    }

public:
    tree build() { return NIL; }
    tree build(const S& x) { return new_node(x); }
    template <std::random_access_iterator Itr> tree build(Itr begin, Itr end) {
        auto dist = std::distance(begin, end);
        if (dist == 1) return build(*begin);
        if (dist == 0) return build();

        Itr mid = begin + (dist / 2);
        tree root = build(*mid);
        tree l = build(begin, mid), r = build(mid + 1, end);
        d[root].lch = l;
        d[root].rch = r;
        d[l].par = root;
        d[r].par = root;
        update(root);
        return root;
    }

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
            update(t);
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
        assert(0 <= p && p < size(t));
        return splay(bound(t, [&](tree cur) {
                         if (p < pos(cur)) return -1;
                         p -= pos(cur);
                         if (p == 0) return 0;
                         p--;
                         return 1;
                     }).second);
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
        auto [lt, mrt] = split_at(t, l);
        auto [mt, rt] = split_at(mrt, r - l);
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
    tree set(tree t, i32 p, const S& x) {
        assert(0 <= p && p < size(t));
        t = splay_at(t, p);  // d[t].lz = am.act.e
        d[t].val = x;
        update(t);
        return t;
    }
    std::pair<tree, S> at(tree t, i32 p) {
        assert(0 <= p && p < size(t));
        t = splay_at(t, p);  // d[t].lz = am.act.e
        return {t, d[t].val};
    }

    inline S all_prod(tree t) const {
        return am.mapping(d[t].lz, d[t].rev ? d[t].prod : d[t].rev_prod);
    }
    std::pair<tree, S> prod(tree t, i32 l, i32 r) {
        assert(0 <= l && l <= r && r <= size(t));
        auto [lt, mt, rt] = split3(t, l, r);
        S ret = all_prod(mt);
        return {merge3(lt, mt, rt), ret};
    }

    std::tuple<tree, S, S> prod_bound(tree t, i32 p) {
        assert(0 <= p && p <= size(t));
        t = splay_at(t, p);
        // t.lch, t, t.rch は push済み
        return {t, d[d[t].lch].prod, am.monoid.op(d[t].prod, d[d[t].rch].prod)};
    }

    inline void all_apply(tree t, const F& f) {
        d[t].lz = am.act.op(f, d[t].lz);
    }
    tree apply(tree t, i32 p, const F& f) {
        t = splay_at(t, p);
        d[t].val = am.mapping(f, d[t].val);
        update(t);
        return t;
    }
    tree apply(tree t, i32 l, i32 r, const F& f) {
        assert(0 <= l && l <= r && r <= size(t));
        auto [lt, mt, mr] = split3(t, l, r);
        all_apply(mt, f);
        return merge3(lt, mt, mr);
    }

    tree rotate(tree t, i32 p) {
        assert(0 <= p && p <= size(t));
        if (p == 0 || p == size(t)) return t;
        auto [l, r] = split_at(t, p);
        return merge(r, l);
    }

    void all_reverse(tree t) { d[t].rev ^= true; }

    tree reverse(tree t, i32 l, i32 r) {
        assert(0 <= l && l <= r && r <= size(t));
        auto [lt, mt, rt] = split3(t, l, r);
        all_reverse(mt);
        return merge3(lt, mt, rt);
    }

    //------------------------------------
    //  insert / erase
    //------------------------------------
public:
    tree insert_at(tree t, i32 p, const S& x) {
        assert(0 <= p && p <= size(t));
        auto [l, r] = split_at(t, p);
        return merge3(l, build(x), r);
    }

    tree erase_at(tree t, i32 p) {
        assert(0 <= p && p < size(t));
        auto [l, m, r] = split3(t, p, p + 1);
        return merge(l, r);
    }

    tree erase_range(tree t, i32 l, i32 r) {
        assert(0 <= l && l <= r && r <= size(t));
        auto [lt, mt, rt] = split3(t, l, r);
        return merge(lt, rt);
    }

    //------------------------------------
    //  internal utility
    //------------------------------------
private:
    inline void push(tree t) {
        node& nt = d[t];
        if (nt.rev) {
            std::swap(nt.lch, nt.rch);
            std::swap(nt.prod, nt.rev_prod);
        }
        nt.val = am.mapping(nt.lz, nt.val);
        nt.prod = am.mapping(nt.lz, nt.prod);
        nt.rev_prod = am.mapping(nt.lz, nt.rev_prod);

        if (nt.lch) {
            d[nt.lch].lz = am.act.op(nt.lz, d[nt.lch].lz);
            if (nt.rev) d[nt.lch].rev ^= true;
        }
        if (nt.rch) {
            d[nt.rch].lz = am.act.op(nt.lz, d[nt.rch].lz);
            if (nt.rev) d[nt.rch].rev ^= true;
        }
        nt.lz = am.act.e;
        nt.rev = false;
    }

    inline S op3(const S& a, const S& b, const S& c) const {
        return am.monoid.op(am.monoid.op(a, b), c);
    }
    inline void update(tree t) {
        // nt.lz must be am.act.e, nt.rev = false
        node& nt = d[t];
        nt.cnt = d[nt.lch].cnt + 1 + d[nt.rch].cnt;
        nt.prod = op3(all_prod(nt.lch), nt.val, all_prod(nt.rch));
        nt.rev_prod = op3(all_rev_prod(nt.lch), nt.val, all_rev_prod(nt.rch));
    }
    S all_rev_prod(tree t) {
        return am.mapping(d[t].lz, d[t].rev ? d[t].rev_prod : d[t].prod);
    }

    inline void rotl(tree t) {
        tree p = d[t].par;
        tree g = d[p].par;
        if (g) (d[g].lch == p ? d[g].lch : d[g].rch) = t;
        d[p].par = t;
        d[p].rch = d[t].lch;
        if (d[t].lch) d[d[t].lch].par = p;
        d[t].par = g;
        d[t].lch = p;
    }
    
    inline void rotr(tree t) {
        tree p = d[t].par;
        tree g = d[p].par;
        if (g) (d[g].lch == p ? d[g].lch : d[g].rch) = t;
        d[p].par = t;
        d[p].lch = d[t].rch;
        if (d[t].rch) d[d[t].rch].par = p;
        d[t].par = g;
        d[t].rch = p;
    }

    inline i32 pos(tree t) const { return d[d[t].lch].cnt; }
};
}  // namespace internal

template <acted_monoid AM> class reversible_splay_tree {
public:
    static void init(const AM& am_) {
        if (!impl) {
            impl = std::make_unique<splaytree>(am_);
        }
    }

private:
    using splaytree = internal::reversible_splay_tree_impl<AM>;
    using S = AM::S;
    using F = AM::F;

    static inline std::unique_ptr<splaytree> impl = nullptr;

    using tree = i32;
    tree id;

public:
    reversible_splay_tree() {
        assert(impl && "constructor called without initialize");
        id = impl->build();
    }
    reversible_splay_tree(const S& x) {
        assert(impl && "constructor called without initialize");
        id = impl->build(x);
    }
    reversible_splay_tree(const std::vector<S>& vec) {
        assert(impl && "constructor called without initialize");
        id = impl->build(vec.begin(), vec.end());
    }
    template <typename Itr> reversible_splay_tree(Itr begin, Itr end) {
        using category = typename std::iterator_traits<Itr>::iterator_category;
        if constexpr (std::is_base_of_v<std::random_access_iterator_tag,
                                        category>) {
            id = impl->build(begin, end);
        }
        else {
            std::vector<S> temp(begin, end);
            id = impl->build(temp.begin(), temp.end());
        }
    }

    i32 size() const { return impl->size(id); }
    bool empty() const { return impl->empty(id); }

    void set(i32 p, const S& x) { id = impl->set(id, p, x); }
    S at(i32 p) {
        auto [new_id, ret] = impl->at(id, p);
        id = new_id;
        return ret;
    }

    void insert_at(i32 p, const S& x) { id = impl->insert_at(id, p, x); }
    void erase_at(i32 p) { id = impl->erase_at(id, p); }
    void erase_range(i32 l, i32 r) { id = impl->erase_range(id, l, r); }
    void rotate(i32 p) { id = impl->rotate(id, p); }
    void apply(i32 p, const F& f) { id = impl->apply(id, p, f); }
    void apply(i32 l, i32 r, const F& f) { id = impl->apply(id, l, r, f); }
    void all_apply(const F& f) { impl->all_apply(id, f); }

    S prod(i32 l, i32 r) {
        auto [new_id, ret] = impl->prod(id, l, r);
        id = new_id;
        return ret;
    }
    S all_prod() const { return impl->all_prod(id); }
    std::pair<S, S> prod_bound(i32 p) {
        auto [new_id, sml, smr] = impl->prod_bound(id, p);
        id = new_id;
        return {sml, smr};
    }

    void all_reverse() { impl->all_reverse(id); }
    void reverse(i32 l, i32 r) { id = impl->reverse(id, l, r); }
};

}  // namespace gwen