#pragma once

// https://github.com/yosupo06/yosupo-library/blob/main/src/yosupo/container/splaytree.hpp

#include <vector>
#include <utility>

#include "gwen/algebra/monoid.hpp"
namespace gwen {

template <acted_monoid M>
class splay_tree {
public:
    using S = typename M::S;
    using F = typename M::F;

    struct node {
        int l, r;
        int len;
        S s, prod;
        F f;
    };

    std::vector<node> nodes;
    M m;

public:
    splay_tree(const M& m_) : m(m_) {
        // treat as a terminal node
        nodes.emplace_back(node{
            .l = -1,
            .r = -1,
            .len = 0,
            .s = m.monoid.e,
            .prod = m.monoid.e,
            .f = m.act.e,
        });
    }

    // subtree structure accessible via splay tree class
    struct tree {
        int id;
        tree() : id(0) {}
        tree(int id_) : id(id_) {}
        bool empty() const { return id == 0; }
    };

    std::size_t size(const tree& t) { return nodes[t.id].len; }
    std::ptrdiff_t ssize(const tree& t) { return size(t); }

    tree build() { return tree(); }
    tree build(const S& s) { return build(std::vector<S>{s}); }
    tree build(const std::vector<S>& v) {
        auto f = [&](auto self, int lidx, int ridx) -> int {
            if(lidx == ridx) return 0;
            assert(lidx <= ridx);
            if(ridx - lidx == 1) {
                int id = new_node(v[lidx], 0, 0);
                return id;
            }
            int mid = std::midpoint(lidx, ridx);
            int id = new_node(v[mid], self(self, lidx, mid), self(self, mid + 1, ridx));
            return id;
        };
        return tree(f(f, 0, int(v.size())));
    }

    S get(tree& t, int k) {
        assert(0 <= k && k < int(size(t)));
        t.id = splay_k(t.id, k);
        return nodes[t.id].s;
    }
    void set(tree& t, int k, S s) {
        assert(0 <= k && k < int(size(t)));
        t.id = splay_k(t.id, k);
        nodes[t.id].s = s;
        update(t.id);
    }

    void insert(tree& t, int k, S s) {
        assert(0 <= k && k <= int(size(t)));
        auto t2 = split(t, k);
        auto t1 = build(s);
        t = merge3(std::move(t), std::move(t1), std::move(t2));
    }
    void erase(tree& t, int k) {
        assert(0 <= k && k < ssize(t));
        auto t2 = split(t, k);
        auto t3 = split(t2, 1);
        t = merge(std::move(t), std::move(t3));
    }

    tree merge(tree&& l, tree&& r) {
        if (l.empty()) return r;
        if (r.empty()) return l;
        int rid = splay_k(r.id, 0);
        nodes[rid].l = l.id;
        update(rid);
        return tree(rid);
    }
    tree merge3(tree&& t1, tree&& t2, tree&& t3) {
        return merge(merge(std::move(t1), std::move(t2)), std::move(t3));
    }
    tree split(tree& t, int k) {
        assert(0 <= k && k <= int(size(t)));
        if (k == int(size(t))) return tree();
        int rid = splay_k(t.id, k);
        int lid = nodes[rid].l;
        nodes[rid].l = 0;
        update(rid);
        t.id = lid;
        return tree(rid);
    }
    std::array<tree, 3> split3(tree&& t, int l, int r) {
        assert(0 <= l && l <= r && r <= ssize(t));
        auto t3 = split(t, r);
        auto t2 = split(t, l);
        return {std::move(t), std::move(t2), std::move(t3)};
    }

    std::vector<S> to_vec(const tree& t) {
        std::vector<S> buf;
        buf.reserve(nodes[t.id].len);
        auto f = [&](auto self, int id) {
            if (id == 0) return;
            push(id);
            self(self, nodes[id].l);
            buf.push_back(nodes[id].s);
            self(self, nodes[id].r);
        };
        f(f, t.id);
        return buf;
    }

    S all_prod(const tree& t) { return nodes[t.id].prod; }
    S prod(tree& t, int l, int r) {
        assert(0 <= l && l <= r && r <= ssize(t));
        auto [t1, t2, t3] = split3(std::move(t), l, r);
        S s = all_prod(t2);
        t = merge3(std::move(t1), std::move(t2), std::move(t3));
        return s;
    }

    void all_apply(tree& t, F f) { all_apply(t.id, f); }
    void apply(tree& t, int l, int r, F f) {
        assert(0 <= l && l <= r && r <= ssize(t));
        auto [t1, t2, t3] = split3(std::move(t), l, r);
        all_apply(t2, f);
        t = merge3(std::move(t1), std::move(t2), std::move(t3));
    }

    template <class F> int max_right(tree& t, F f) {
        assert(f(m.monoid.e));
        if (f(all_prod(t))) return nodes[t.id].len;
        S s = m.monoid.e;
        int r = 0;
        t.id = splay(t.id, [&](int lid, int id, int) {
            S s2 = m.monoid.op(s, all_prod(lid));
            if (!f(s2)) return -1;
            r += nodes[lid].len;
            s = s2;
            S s3 = m.monoid.op(s, nodes[id].s);
            if (!f(s3)) return 0;
            r++;
            s = s3;
            return 1;
        });
        return r;
    }

private:
    int new_node(const S& s, int l, int r) {
        int id = int(nodes.size());
        node n;
        n.l = l;
        n.r = r;
        n.s = s;
        n.f = m.act.e;
        nodes.emplace_back(n);
        update(id);
        return id;
    }

    // apply n.f to n.l / n.r
    void push(int id) {
        node& n = nodes[id];
        all_apply(n.l, n.f);
        all_apply(n.r, n.f);
        n.f = m.act.e;
    }

    // update len / prod of nodes[id]
    void update(int id) {
        node& n = nodes[id];
        n.len = nodes[n.l].len + nodes[n.r].len;
        n.prod = m.monoid.op(m.monoid.op(nodes[n.l].prod, n.prod), nodes[n.r].prod);
    }

    // apply f to nodes[id]
    // n is applied, n.l / n.r is not applied
    void all_apply(int id, const F& f) {
        if(id == 0) return;
        node& n = nodes[id];
        n.s = m.mapping(f, n.s);
        n.prod = m.mapping(f, n.prod);
        n.f = m.act.op(f, n.f);
    }

    // splay
    int splay_k(int id, int k) {
        return splay(id, [&](int l, int, int) {
            int lsz = nodes[l].len;
            if(k < lsz) return -1;
            k -= lsz;
            if(k == 0) return 0;
            // k--;
            return 1;
        });
    }

    int splay_with_key(int id, const S& key) {
        return splay(id, [&](int l, int id, int r) {
            const S& cur_key = nodes[id].s;
            if(!(current_key < key) && !(key < current_key))
        });
    }

    template <class Cond> int splay(int id, Cond cond) {
        static std::vector<int> lefts, rights;
        lefts.clear();
        rights.clear();

        int zig = 0;
        while (true) {
            push(id);
            int l = nodes[id].l, r = nodes[id].r;

            auto dir = cond(l, id, r);

            if (dir == 0) {
                {
                    int tmp = nodes[id].l;
                    for (auto i = std::ssize(lefts) - 1; i >= 0; i--) {
                        nodes[lefts[i]].r = std::exchange(tmp, lefts[i]);
                        update(lefts[i]);
                    }
                    nodes[id].l = tmp;
                }
                {
                    int tmp = nodes[id].r;
                    for (auto i = std::ssize(rights) - 1; i >= 0; i--) {
                        nodes[rights[i]].l = std::exchange(tmp, rights[i]);
                        update(rights[i]);
                    }
                    nodes[id].r = tmp;
                }
                update(id);
                return id;
            }
            if (dir < 0) {
                if (zig == -1) {
                    // zig-zig
                    int p = rights.back();
                    rights.pop_back();
                    nodes[p].l = r;
                    update(p);
                    nodes[id].r = p;
                    zig = 0;
                } else {
                    zig = -1;
                }
                rights.push_back(id);
                id = l;
            } else {
                if (zig == 1) {
                    // zig-zig
                    int p = lefts.back();
                    lefts.pop_back();
                    nodes[p].r = l;
                    update(p);
                    nodes[id].l = p;
                    zig = 0;
                } else {
                    zig = 1;
                }
                lefts.push_back(id);
                id = r;
            }
        }
    }
};

}  // namespace gwen