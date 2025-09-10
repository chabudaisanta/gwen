#pragma once

#include <algorithm>
#include <bit>
#include <cassert>
#include <vector>

#include "gwen/types.hpp"

namespace gwen {

class doubling_tree {
private:
    int n, log, root;
    std::vector<int> anc;
    std::vector<int> d;

public:
    explicit doubling_tree(int n_,
                           int root_,
                           const std::vector<std::vector<int>>& G)
        : n(n_),
          log(n_ > 0 ? std::bit_width(static_cast<u32>(n_)) : 0),
          root(root_) {
        assert(n == ssize(G));
        assert(0 <= root && root < n);

        anc.assign(log * n, -1);
        d.assign(n, -1);

        std::vector<int> q;
        q.reserve(n);
        q.emplace_back(root);

        d[root] = 0;

        int head = 0;
        while (head < ssize(q)) {
            int v = q[head++];
            for (int u : G[v]) {
                if (u != get_anc(v, 0)) {
                    d[u] = d[v] + 1;
                    set_anc(u, 0) = v;
                    q.emplace_back(u);
                }
            }
        }

        for (int i = 1; i < log; ++i) {
            for (int v = 0; v < n; ++v) {
                int parent_half = get_anc(v, i - 1);
                if (parent_half != -1) {
                    set_anc(v, i) = get_anc(parent_half, i - 1);
                }
            }
        }
    }

    int kth_anc(int v, int k) const {
        assert(0 <= v && v < n);
        assert(k >= 0);
        if (k > d[v]) return -1;

        while (k > 0 && v != -1) {
            int i = std::countr_zero(static_cast<u32>(k));
            v = get_anc(v, i);
            k &= k - 1;
        }
        return v;
    }

    inline int depth(int v) const {
        assert(0 <= v && v < n);
        return d[v];
    }

    int lca(int u, int v) const {
        assert(0 <= u && u < n);
        assert(0 <= v && v < n);

        if (d[u] < d[v]) std::swap(u, v);
        u = kth_anc(u, d[u] - d[v]);

        if (u == v) return u;
        for (int i = log - 1; i >= 0; --i) {
            if (get_anc(u, i) != get_anc(v, i)) {
                u = get_anc(u, i);
                v = get_anc(v, i);
            }
        }
        return get_anc(u, 0);
    }

    int len(int u, int v) const {
        assert(0 <= u && u < n);
        assert(0 <= v && v < n);

        return depth(u) + depth(v) - 2 * depth(lca(u, v)) + 1;
    }

private:
    inline int get_anc(int v, int i) const {
        return anc[static_cast<size_t>(i) * n + v];
    }

    inline int& set_anc(int v, int i) {
        return anc[static_cast<size_t>(i) * n + v];
    }
};

}  // namespace gwen
