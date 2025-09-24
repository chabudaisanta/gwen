#pragma once

#include <cassert>
#include <ranges>
#include <vector>

#include "gwen/algebra/monoid.hpp"
#include "gwen/misc/xorshift.hpp"

namespace gwen {

template <acted_monoid AM>
class treap {
private:
    using S = AM::S;
    using F = AM::F;

    using tree = int;
    struct node {
        tree lch;
        tree rch;
        S val;
        S prod;
        F lz;
        int cnt;
        u32 pri;
    };
    std::vector<node> d;
    static constexpr tree NIL = 0;
    AM am;

public:
    explicit treap(const AM& am_) : am(am_) {
        // NIL
        // 書き換え厳禁
        new_node(am.monoid.e, 0, 0u);
    }

    tree build() { return NIL; }
    tree build(const S& x) { return new_node(x); }
    tree build(const std::vector<S>& vec) {
        if (vec.empty()) return NIL;

        int n = vec.size();
        std::vector<tree> nodes(n);
        for (int i = 0; i < n; ++i) nodes[i] = new_node(vec[i]);

        std::vector<tree> st;
        for (int i = 0; i < n; ++i) {
            tree curr = nodes[i];
            tree last_popped = NIL;
            while (!st.empty() && d[st.back()].pri < d[curr].pri) {
                last_popped = st.back();
                st.pop_back();
            }

            d[curr].lch = last_popped;
            if (!st.empty()) {
                d[st.back()].rch = curr;
            }

            st.push_back(curr);
        }
        tree root = st[0];
        auto f = [&](tree cur, auto self) -> void {
            if (!cur) return;
            self(d[cur].lch);
            self(d[cur].rch);
            update(cur);
        };
        f(root, f);

        return root;
    }

    inline int size(tree t) const { return d[t].cnt; }
    inline bool empty(tree t) const { return !d[t].cnt; }

    // merge
    tree merge(tree l, tree r) {
        if (!l || !r) return l ? l : r;

        static std::vector<std::pair<tree, bool>> path;
        path.clear();

        while (l && r) {
            if (d[l].pri >= d[r].pri) {
                push(l);
                path.emplace_back(l, true);

                l = d[l].rch;
            }
            else {
                push(r);
                path.emplace_back(r, false);
                r = d[r].lch;
            }
        }
        // l/r どちらかが NIL
        tree cur = l ? l : r;
        for (auto [par, dir] : path | std::views::reverse) {
            // par の子が cur になる
            // dir なら par の右子が cur, !dir なら par の左子が cur
            (dir ? d[par].rch : d[par].lch) = cur;
            update(par);
            cur = par;
        }
        return cur;
    }
    tree merge3(tree l, tree m, tree r) { return merge(l, merge(m, r)); }

    // split
    std::pair<tree, tree> split(tree t, int p) {
        assert(0 <= p && p <= size(t));
        if (p == 0) return {NIL, t};
        if (p == size(t)) return {t, NIL};

        static std::vector<tree> lefts, rights;
        lefts.clear();
        rights.clear();
        tree cur = t;
        while (size(d[cur].lch) != p) {
            push(cur);
            if (size(d[cur].lch) < p) {
                // cur の左の仕切りが p 未満 -> cur は lefts
                lefts.emplace_back(cur);
                p -= size(d[cur].lch) + 1;
                cur = d[cur].rch;
            }
            else {  // p < size(d[cur].lch)
                rights.emplace_back(cur);
                cur = d[cur].lch;
            }
        }
        // cur のちょうど一つ左が p の仕切り
        // lefts[i] は lefts[i+1] より右 & pri も高い
        // lefts[i+1] を lefts[i] の右子につけ続ける & lefts[0] が根になる
        // {NIL, t}, {t, NIL}, {NIL, NIL}
        // のパターンは排除されているので、どちらも少なくとも1要素持つはず
        // -> cur は rights 最左の子
        // -> cur の lch は lefts の最右の子
        push(cur);
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
    std::tuple<tree, tree, tree> split3(tree t, int p, int q) {
        assert(0 <= p && p <= q && q <= size(t));
        auto [l, mr] = split(t, p);
        auto [m, r] = split(mr, q - p);
        return {l, m, r};
    }

    // insert
    void insert(tree& t, int p, const S& x) {
        assert(0 <= p && p <= size(t));
        auto [l, r] = split(t, p);
        tree m = new_node(x);
        t = merge3(l, m, r);
    }

    // erase
    void erase(tree& t, int p) {
        assert(0 <= p && p < size(t));
        auto [l, m, r] = split3(t, p, p + 1);
        t = merge(l, r);
    }

    // O(1), 副作用なし
    inline S all_prod(tree t) const { return am.mapping(d[t].lz, d[t].prod); }

    S prod(tree t, int l, int r) {
        assert(0 <= l && l <= r && r <= size(t));
        auto [lt, m, rt] = split3(t, l, r);
        S ret = all_prod(m);
        merge3(lt, m, rt);
        return ret;
    }

    void all_apply(tree t, const F& f) {
        if (t) d[t].lz = am.act.op(f, d[t].lz);
    }

    void apply(tree t, int l, int r, const F& f) {
        assert(0 <= l && l <= r && r <= size(t));
        auto [lt, m, rt] = split3(t, l, r);
        all_apply(m, f);
        merge3(lt, m, rt);
    }

    // O(N)
    std::vector<S> to_vec(tree t) {
        static std::vector<S> ret;
        ret.clear();
        if (t) dfs_to_vec(t, ret);
        return ret;
    }
    void dfs_to_vec(int cur, std::vector<S>& v) {
        push(cur);
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
            .lz = am.act.e,
            .cnt = c,
            .pri = p,
        });
        return ret;
    }

    // t.val を評価して、 t.lz を子に伝播させる。
    // 操作後に t.lz = a.act.e (=id) を保証する
    // t.prod の値も保証される。
    void push(tree t) {
        node& nt = d[t];
        nt.val = am.mapping(nt.lz, nt.val);
        nt.prod = am.mapping(nt.lz, nt.prod);
        if (nt.lch) {  // NILチェック
            d[nt.lch].lz = am.act.op(nt.lz, d[nt.lch].lz);
        }
        if (nt.rch) {  // NILチェック
            d[nt.rch].lz = am.act.op(nt.lz, d[nt.rch].lz);
        }
        nt.lz = am.act.e;
    }

    // t の子から t.prod, t.cnt を再計算する。
    // t.lz = id の状態が前提
    void update(tree t) {
        node& nt = d[t];
        nt.cnt = size(nt.lch) + 1 + size(nt.rch);
        nt.prod = am.monoid.op(am.monoid.op(all_prod(nt.lch), nt.val),
                               all_prod(nt.rch));
    }
};

}  // namespace gwen