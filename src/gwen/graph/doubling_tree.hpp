#pragma once

#include <vector>
#include <bit>
#include <cassert>
#include <algorithm>
#include "gwen/types.hpp"

namespace gwen {

class doubling_tree {
private:
    int n, log, root;
    std::vector<int> anc;
    std::vector<int> d;

public:
    explicit doubling_tree(int n_, int root_, const std::vector<std::vector<int>>& G) : n(n_), log(std::bit_width(static_cast<u32>(n_))), root(root_) {
        assert(n == ssize(G));
        assert(0 <= root && root < n);
        anc.assign(log * n, -1);
        d.assign(n, -1);

        d[root] = 0;
        build(root, -1, G);
        
    }

    int kth_anc(int v, int k) const {
        assert(0 <= v && v < n);
        for(int i = 0; i < log; ++i) {
            if(k & (1 << i)) {
                v = anc[v * log + i];
                if(v == -1) return v;
            }
        }
        return v;
    }

    int depth(int v) const {
        assert(0 <= v && v < n);
        return d[v];
    }

    int lca(int u, int v) const {
        assert(0 <= u && u < n);
        assert(0 <= v && v < n);

        if(d[u] < d[v]) std::swap(u, v);
        u = kth_anc(u, d[u] - d[v]);

        if(u == v) return u;
        for(int i = log - 1; i >= 0; --i) {
            if(get_anc(u, i) != get_anc(v, i)) {
                u = get_anc(u, i);
                v = get_anc(v, i);
            }
        }
        return get_anc(u, 0);
    }

private:
    void build(int v, int p, auto& G) {
        set_anc(v, 0) = p;
        for(int i = 1; i < log; ++i) {
            int ph = get_anc(v, i - 1);
            if(ph != -1) set_anc(v, i) = get_anc(ph, i - 1);
        }
        for(int u : G[v]) {
            if(u != p) {
                d[u] = d[v] + 1;
                build(u, v, G);
            }
        }
    }

    inline int get_anc(int v, int i) const {
        return anc[v * log + i];
    }

    inline int& set_anc(int v, int i) {
        return anc[v * log + i];
    }
};

} // namespace gwen