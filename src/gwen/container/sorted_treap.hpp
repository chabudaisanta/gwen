#pragma once

#include <cassert>
#include <concepts>
#include <memory>
#include <ranges>
#include <vector>

#include "gwen/algebra/monoid.hpp"
#include "gwen/misc/xorshift.hpp"
#include "gwen/types.hpp"

namespace gwen {

namespace internal {

template <monoid M, typename Compare>
class sorted_treap_impl {
public:
    using S = M::S;
    using tree = int;

private:
    struct node {
        tree lch;
        tree rch;
        S val;
        S prod;
        int cnt;
        u32 pri;
    };

    std::vector<node> d;
    static constexpr tree NIL = 0;
    M m;
    Compare comp;

    //------------------------------------
    // constructor
    //------------------------------------
public:
    explicit sorted_treap_impl(const M& m_, const Compare& comp_) : m(m_), comp(comp_) {
        d.emplace_back(new_node(m.e, 0, 0u));  // NIL 用ダミーノード、書き換え厳禁。
    }

    //------------------------------------
    //  make node/tree
    //------------------------------------
public:
    tree build() { return NIL; }
    tree build(const S& x) { return new_node(x); }

private:
    tree new_node(const S& x, int c = 1, u32 p = rand32()) {
        tree ret = d.size();
        d.emplace_back(node{
            .lch = NIL,
            .rch = NIL,
            .val = x,
            .prod = x,
            .cnt = c,
            .pri = p,
        });
        return ret;
    }

    //------------------------------------
    //  utility
    //------------------------------------
public:
    inline int size(tree t) const { return d[t].cnt; }
    inline bool empty(tree t) const { return !d[t].cnt; }

    // O(N)
    std::vector<S> to_vec(tree t) const {
        static std::vector<S> ret;
        ret.clear();
        auto dfs = [&](int cur, auto self) -> void {
            if (d[cur].lch) self(d[cur].lch, self);
            ret.emplace_back(d[cur].val);
            if (d[cur].rch) self(d[cur].rch, self);
        };
        if (t) dfs(t, dfs);
        return ret;
    }

    bool equal(const S& a, const S& b) const {
        return !comp(a, b) && !comp(b, a);
    }

    S get_val(tree t) const {
        return d[t].val;
    }

    //------------------------------------
    //  internal utility
    //------------------------------------
private:
    void update(tree t) {
        node& nt = d[t];
        nt.cnt = size(nt.lch) + 1 + size(nt.rch);
        nt.prod = m.op(m.op(d[nt.lch].prod, nt.val), d[nt.rch].prod);
    }

    inline int pos(tree t) const { return size(d[t].lch); }

public:
    std::tuple<tree, int, S> search(tree t, auto cond) const {
        tree ret = NIL;
        int lower = 0;
        S sml = m.e;
        while (t) {
            auto dir = cond(t);
            if (!dir) break;

            if (dir > 0) {
                lower += pos(t) + 1;
                sml = m.op(sml, m.op(all_prod(d[t].lch), all_prod(t)));
                t = d[t].rch;
            }
            else {
                ret = t;
                t = d[t].lch;
            }
        }
        return {ret, lower, sml};
    }

    tree at(tree t, int p) const {
        assert(0 <= p && p <= size(t));
        while (pos(t) != p) {
            if (p < pos(t)) {
                t = d[t].lch;
            }
            else {
                p -= pos(t) + 1;
                t = d[t].rch;
            }
        }
        return t;
    }

    //------------------------------------
    //  interval query
    //------------------------------------
public:
    S all_prod(tree t) const { return d[t].prod; }
    S prod(tree t, int p, int q) {
        assert(0 <= p && p <= q && q <= size(t));
        auto [l, mr] = split_at(t, p);
        auto [m, r] = split_at(mr, q - p);
        S ret = all_prod(m);
        merge3(l, m, r);
        return ret;
    }

    //------------------------------------
    //  merge
    //------------------------------------
public:
    tree merge(tree l, tree r) {
        if (!l || !r) return l ? l : r;

        static std::vector<std::pair<tree, bool>> path;
        path.clear();

        while (l && r) {
            if (d[l].pri >= d[r].pri) {
                path.emplace_back(l, true);
                l = d[l].rch;
            }
            else {
                path.emplace_back(r, false);
                r = d[r].lch;
            }
        }

        tree cur = l ? l : r;
        for (auto [par, dir] : path | std::views::reverse) {
            (dir ? d[par].rch : d[par].lch) = cur;
            update(par);
            cur = par;
        }
        return cur;
    }
    tree merge3(tree l, tree m, tree r) { return merge(l, merge(m, r)); }

    //------------------------------------
    //  split
    //------------------------------------
public:
    std::pair<tree, tree> split_at(tree t, int p) {
        assert(0 <= p && p <= size(t));
        if (p == 0) return {NIL, t};
        if (p == size(t)) return {t, NIL};

        return split(t, [&](int cur) -> int {
            if (p <= pos(cur)) {
                return -1;
            }
            else {
                // p は減らしておく
                p -= pos(cur) + 1;
                return 1;
            }
        });
    }

    // comp(d[cur].val, x) (d[cur].val < x) を満たさない (x <= d[cur].val)
    // 最左のcurを求める
    std::pair<tree, tree> split_key(tree t, const S& x) {
        if (!t) return {NIL, NIL};

        return split(t, [&](int cur) -> int {
            // !comp(d[cur].val, x)  <=>  !(d[cur].val < x)  <=>  x <=
            // d[cur].val
            return !comp(d[cur].val, x) ? -1 : 1;
        });
    }

    std::pair<tree, tree> split(tree t, auto cond) {
        static std::vector<tree> lefts, rights;
        lefts.clear();
        rights.clear();

        tree cur = t;
        while (cur) {
            auto dir = cond(cur);
            if (dir < 0) {
                // 左に進む -> cur は 仕切りより右 -> rights
                rights.emplace_back(cur);
                cur = d[cur].lch;
            }
            else {
                lefts.emplace_back(cur);
                cur = d[cur].rch;
            }
        }

        if (rights.empty()) return {t, NIL};
        if (lefts.empty()) return {NIL, t};

        tree r = NIL;
        for (tree rgt : rights | std::views::reverse) {
            d[rgt].lch = r;
            update(rgt);
            r = rgt;
        }
        tree l = NIL;
        for (tree lft : lefts | std::views::reverse) {
            d[lft].rch = l;
            update(lft);
            l = lft;
        }
        return {l, r};
    }
};

}  // namespace internal

template <monoid M, typename Compare>
    requires requires(const Compare& comp,
                      const typename M::S& a,
                      const typename M::S& b) {
        { comp(a, b) } -> std::same_as<bool>;
    }
class sorted_treap {
public:
    static void init(const M& m, const Compare& comp) {
        assert(!impl);
        if (!impl) {
            impl = std::make_unique<treap>(m, comp);
        }
    }

private:
    using treap = internal::sorted_treap_impl<M, Compare>;
    using tree = treap::tree;
    using S = M::S;

    static inline std::unique_ptr<treap> impl;
    int id;

    //------------------------------------
    //  constructor
    //------------------------------------
public:
    explicit sorted_treap() {
        assert(impl);
        id = impl->build();
    }
    // TODO いろんなコンストラクタを作る

    sorted_treap(const sorted_treap&) = delete;
    sorted_treap& operator=(const sorted_treap&) = delete;

    //------------------------------------
    //  utility
    //------------------------------------
public:
    int size() const {
        return impl->size(id);
    }
    bool empty() const {
        return impl->empty(id);
    }

    S at(int p) const {
        return impl->get_val(impl->at(id, p));
    }

    S prod(int l, int r) {
        return impl->prod(id, l, r);
    }
    S all_prod() {
        return impl->all_prod(id);
    }
    //------------------------------------
    //  insert
    //------------------------------------
public:
    void insert(const S& x) {
        auto [l, r] = impl->split_key(id, x);
        tree m = impl->build(x);
        id = impl->merge3(l, m, r);
    }

    void insert_unique(const S& x) {
        auto [t, p, prod] = impl->search(x);
        if (impl->equal(impl->get_val(t), x)) return;
        insert(x);
    }

    //------------------------------------
    //  erase
    //------------------------------------
public:
    void erase(const S& x) {
        auto [l, mr] = impl->split_key(id, x);
        if (impl->size(mr) == 0) {
            id = impl->merge(l, mr);
            return;
        }
        auto [m, r] = impl->split_at(mr, 1);
        if (impl->equal(impl->get_val(m), x)) {
            id = impl->merge(l, r);
            return;
        }
        id = impl->merge3(l, m, r);
    }
};

}  // namespace gwen