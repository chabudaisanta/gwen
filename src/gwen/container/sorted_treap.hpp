#pragma once

#include <cassert>
#include <concepts>
#include <ranges>
#include <vector>

#include "gwen/algebra/monoid.hpp"
#include "gwen/misc/xorshift.hpp"
#include "gwen/types.hpp"

namespace gwen {

template <monoid M, typename Compare = std::less<typename M::S>>
    requires requires(const Compare& comp,
                      const typename M::S& a,
                      const typename M::S& b) {
        { comp(a, b) } -> std::same_as<bool>;
    }
class sorted_treap {
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
    bool equal(const S& a, const S& b) const {
        return !comp(a, b) && !comp(b, a);
    }

    //------------------------------------
    // constructor
    //------------------------------------
public:
    explicit sorted_treap(const M& m_) : m(m_) {
        // NIL 書き換え厳禁
        new_node(m.e, 0, 0u);
    }
    sorted_treap(const sorted_treap&) = delete;
    sorted_treap& operator=(const sorted_treap&) = delete;
    ~sorted_treap() = default;

    //------------------------------------
    // make node/tree
    //------------------------------------
public:
    tree build() { return NIL; }
    tree build(const S& x) { return new_node(x); }
    // TODO ソート済み配列から線形時間構築を書く

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
    // utility
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

    //------------------------------------
    // insert / erase
    //------------------------------------
public:
    void insert(tree& t, const S& x) {
        auto [l, r] = split_key(t, x);
        auto m = new_node(x);
        t = merge3(l, m, r);
    }

    void erase_key(tree& t, const S& x) {
        auto [l, mr] = split_key(t, x);
        if (!mr) {
            t = l;
            return;
        }
        auto [m, r] = split_at(mr, 1);
        if (m && equal(x, d[m].val))
            t = merge(l, r);
        else
            t = merge3(l, m, r);
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

    //------------------------------------
    // interval query
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
    // point query
    //------------------------------------
public:
    S at(tree t, int p) const {
        assert(0 <= p && p < size(t));
        auto [lower, res] = search(t, [&](int cur) -> int {
            if (p <= pos(cur)) {
                return -1;
            }
            else {
                p -= pos(cur) + 1;
                return 1;
            }
        });
        return d[res].val;
    }

    //------------------------------------
    // key query
    //------------------------------------
    // lower_bound(x) が end のとき、{size(t), e}を返す
    std::pair<int, S> lower_bound(tree t, const S& x) const {
        auto [lower, res] = search(t, [&](int cur) -> int {
            // !comp(d[cur].val, x)  <=>  !(d[cur].val < x)  <=>  x <=
            // d[cur].val
            return !comp(d[cur].val, x) ? -1 : 1;
        });
        return {lower, d[res].val};
    }

    std::pair<int, S> upper_bound(tree t, const S& x) const {
        auto [lower, res] = search(t, [&](int cur) -> int {
            // comp(x < d[cur].val)  <=>  x < d[cur].val
            return comp(x, d[cur].val) ? -1 : 1;
        });
        return {lower, d[res].val};
    }

    int rank(tree t, const S& x) const { return lower_bound(t, x).first; }

    int count(tree t, const S& x) const {
        auto lb = lower_bound(t, x);
        auto ub = upper_bound(t, x);
        return ub.first - lb.first;
    }

    bool contains(tree t, const S& x) const {
        return static_cast<bool>(count(t, x));
    }

    // f(prod(0, r-1)) = true
    // f(prod(0, r)) = false
    std::pair<int, S> max_right(tree t, auto f) const {
        assert(f(m.e));
        int lower = 0;
        S tot = m.e;
        // 左との積が true なら、自分もしくは右祖先
        while (t) {
            S withleft = m.op(tot, all_prod(d[t].lch));
            if (!f(withleft)) {
                t = d[t].lch;
                continue;
            }
            lower += pos(t);
            S withcur = m.op(withleft, d[t].val);
            if (!f(withcur)) {
                return {lower, withleft};
            }
            tot = withcur;
            lower += 1;
            t = d[t].rch;
        }
        return {lower, tot};
    }

    // TODO l まで降りてから探索するmax_right
    std::pair<int, S> max_right(tree t, int l, auto f) {
        assert(0 <= l && l <= size(t));
        auto [lt, rt] = split_at(t, l);
        S buf = all_prod(lt);
        auto [lower, tot] = max_right(
            tree rt, [&](const S& p) -> bool { return f(m.op(buf, p)); });
        merge(lt, rt);
        return {lower + l, m.op(buf, tot)};
    }

    // TODO min_left

    //------------------------------------
    // internal utils
    //------------------------------------
private:
    // t の子から t.prod, t.cnt を再計算する。
    // t.lz = id の状態が前提
    void update(tree t) {
        node& nt = d[t];
        nt.cnt = size(nt.lch) + 1 + size(nt.rch);
        nt.prod = m.op(m.op(d[nt.lch].prod, nt.val), d[nt.rch].prod);
    }

    inline int pos(tree t) const { return size(d[t].lch); }

    std::pair<int, tree> search(tree t, auto cond) const {
        tree ret = NIL;
        int lower = 0;
        while (t) {
            auto dir = cond(t);
            if (!dir) break;

            if (dir > 0) {
                lower += pos(t) + 1;
                t = d[t].rch;
            }
            else {
                ret = t;
                t = d[t].lch;
            }
        }
        return {lower, ret};
    }

    //------------------------------------
    // merge
    //------------------------------------
private:
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
    // treap の性質により、木構造は一意に定まるため根は変化しない
    tree merge3(tree l, tree m, tree r) { return merge(l, merge(m, r)); }

    //------------------------------------
    // split
    //------------------------------------
private:
    // cond(cur)
    // cond < 0 : go left
    // (cond = 0 : correct node) <-
    // よくない。NILノードまで進まないとlefts/rightsに終端が入らずバグる cond >
    // 0 : go right cond
    // に従って進み、仕切りから距離1のノード(真左/真右)にたどり着く必要がある
    // - cur の状態は左右どちらかの仕切りに最も近い NIL 、lefts/rights
    // の末尾は仕切りに最も近いかつ pri 最小
    // - i+1 番目を i 番目の子にするだけで新たな木の構築が終了し、lefts/rights
    // の先頭が新たな根になる
    std::pair<tree, tree> split(tree t, auto cond) {
        static std::vector<tree> lefts, rights;
        lefts.clear();
        rights.clear();

        tree cur = t;
        while (cur) {
            auto dir = cond(cur);
            // if(dir == 0) break; // correct node ではなく NIL
            // ノードで止まりたいため
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

    std::pair<tree, tree> split_at(tree t, int p) {
        assert(0 <= p && p <= size(t));
        if (p == 0) return {NIL, t};
        if (p == size(t)) return {t, NIL};

        return split(t, [&](int cur) -> int {
            // if(!cur) return 0; // split 内の cond 評価前に NIL
            // チェックを行うため

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
        static std::vector<tree> lefts, rights;
        lefts.clear();
        rights.clear();

        return split(t, [&](int cur) -> int {
            // !comp(d[cur].val, x)  <=>  !(d[cur].val < x)  <=>  x <=
            // d[cur].val
            return !comp(d[cur].val, x) ? -1 : 1;
        });
    }
};

}  // namespace gwen