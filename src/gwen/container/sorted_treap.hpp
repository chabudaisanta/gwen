#pragma once

#include <vector>
#include <ranges>
#include <cassert>
#include <concepts>

#include "gwen/types.hpp"
#include "gwen/algebra/monoid.hpp"
#include "gwen/misc/xorshift.hpp"

namespace gwen {

// 必要なものリスト
// - merge
// - split_at
// - split_key
// - insert
// - erase_at
// - erase_key
// - count
// - contains
// - find
// - lower_bound
// - upper_bound
template <monoid M, typename Compare = std::less<typename M::S>>
requires requires(const Compare& comp, const typename M::S& a, const typename M::S& b) {
    { comp(a, b) } -> std::same_as<bool>;
}
class sorted_treap {
private:
    using S = M::S;

    using tree = int;
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
    bool equal(const S& a, const S& b) const {
        return !comp(a, b) && !comp(b, a);
    }

public:
    explicit sorted_treap(const M& m_) : m(m_) {
        // NIL
        // 書き換え厳禁
        new_node(m.e, 0, 0u);
    }

    tree build() { return NIL; }
    tree build(const S& x) { return new_node(x); }
        
    inline int size(tree t) const { return d[t].cnt; }
    inline bool empty(tree t) const { return !d[t].cnt; }

    void insert(tree& t, const S& x) {
        auto [l, r] = split_key(t, x);
        auto m = new_node(x);
        t = merge3(l, m, r);
    }
    void erase_key(tree& t, const S& x) {
        auto [l, mr] = split_key(t, x);
        if(!mr) {
            t = l;
            return;
        }
        auto [m, r] = split_at(mr, 1);
        if(m && equal(x, d[m].val)) t = merge(l, r);
        else t = merge3(l, m, r);
    }
    void erase_at(tree& t, int p) {
        assert(0 <= p && p <= size(t));
        auto [l, mr] = split_at(t, p);
        auto [m, r] = split_at(mr, 1);
        t = merge(l, r);
    }
    void erase_range(tree& t, int p, int q) {
        assert(0 <= p && p <= q && q <= size(t));
        auto [l, mr] = split_at(t, p);
        auto [m, r] = split_at(mr, q - p);
        t = merge(l, r);
    }

    // O(N)
    std::vector<S> to_vec(tree t) {
        static std::vector<S> ret;
        ret.clear();
        if (t) dfs_to_vec(t, ret);
        return ret;
    }
    void dfs_to_vec(int cur, std::vector<S>& v) {
        if (d[cur].lch) dfs_to_vec(d[cur].lch, v);
        v.emplace_back(d[cur].val);
        if (d[cur].rch) dfs_to_vec(d[cur].rch, v);
    }

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

    // t の子から t.prod, t.cnt を再計算する。
    // t.lz = id の状態が前提
    void update(tree t) {
        node& nt = d[t];
        nt.cnt = size(nt.lch) + 1 + size(nt.rch);
        nt.prod = m.op(m.op(d[nt.lch].prod, nt.val),
                               d[nt.rch].prod);
    }

    // merge
    // l.back() <= r.front()
    // l, r はソート済み
    tree merge(tree l, tree r) {
        if (!l || !r) return l ? l : r;

        static std::vector<std::pair<tree, bool>> path;
        path.clear();

        while (l && r) {
            if (d[l].pri >= d[r].pri) {
                path.emplace_back(l, true);
                l = d[l].rch;
            } else {
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

    // split
    std::pair<tree, tree> split_at(tree t, int p) {
        assert(0 <= p && p <= size(t));
        if (p == 0) return {NIL, t};
        if (p == size(t)) return {t, NIL};

        static std::vector<tree> lefts, rights;
        lefts.clear();
        rights.clear();
        tree cur = t;
        while (size(d[cur].lch) != p) {
            if (size(d[cur].lch) < p) {
                // cur の左の仕切りが p 未満 -> cur は lefts
                lefts.emplace_back(cur);
                p -= size(d[cur].lch) + 1;
                cur = d[cur].rch;
            } else {  // p < size(d[cur].lch)
                rights.emplace_back(cur);
                cur = d[cur].lch;
            }
        }
        
        tree r = cur;
        tree l = d[cur].lch;
        d[cur].lch = NIL;
        update(cur);
        for (tree rgt : rights | std::views::reverse) {
            d[rgt].lch = r;
            update(rgt);
            r = rgt;
        }
        for (tree lft : lefts | std::views::reverse) {
            d[lft].rch = l;
            update(lft);
            l = lft;
        }
        return {l, r};
    }

    // comp(d[cur].val, x) (d[cur].val < x) を満たさない (x <= d[cur].val) 最左のcurを求める
    std::pair<tree, tree> split_key(tree t, const S& x) {
        if(!t) return {NIL, NIL};
        static std::vector<tree> lefts, rights;
        lefts.clear();
        rights.clear();

        tree cur = t;
        while(cur) {
            if(comp(d[cur].val, x)) {
                lefts.emplace_back(cur);
                cur = d[cur].rch;
            }
            else {
                rights.emplace_back(cur);
                cur = d[cur].lch;
            }
        }
        // cur = NIL
        // rights.back() が目的の仕切りの左のノード
        // rights が空なら?
        // -> return {t, NIL}
        // lefts が空なら?
        // -> return {NIL, t}
        if(rights.empty()) return {t, NIL};
        if(lefts.empty()) return {NIL, t};

        // rights.back() が右側の最左かつ pri 最小のノード
        // lefts.back() が左側の最右かつ pri 最小のノード
        tree r = NIL;
        for(tree rgt : rights | std::views::reverse) {
            // rights[i]の左子をrights[i+1]にする
            d[rgt].lch = r;
            update(rgt);
            r = rgt;
        }
        tree l = NIL;
        for(tree lft : lefts | std::views::reverse) {
            //lefts[i]の右子をlefts[i+1]にする
            d[lft].rch = l;
            update(lft);
            l = lft;
        }
        return {l, r};
    }
};

}// namespace gwen