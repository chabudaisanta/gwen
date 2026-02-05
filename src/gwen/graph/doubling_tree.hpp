#pragma once

#include <algorithm>
#include <bit>
#include <cassert>
#include <ranges>
#include <vector>

#include "gwen/types.hpp"
#include "gwen/graph/xor_linked_tree.hpp"

namespace gwen {

class doubling_tree {
private:
    i32 n, log, root;
    std::vector<i32> anc;
    std::vector<i32> d;

public:
    template <typename Edge>
    explicit doubling_tree(i32 n_, i32 root_, const std::vector<Edge>& E)
        : n(n_), log(n_ > 0 ? std::bit_width(static_cast<u32>(n_)) : 0), root(root_) {
        anc.assign(log * n, -1);
        d.assign(n, 0);

        auto [par, ord] = xor_tree_scan<Edge, true>(E, root);
        for (i32 v : ord | std::views::reverse) {
            i32 p = par[v];
            if(p != -1) {
                d[v] = d[p] + 1;
                set_anc(v, 0) = p;
            }
        }

        for (i32 i = 1; i < log; ++i) {
            for (i32 v = 0; v < n; ++v) {
                i32 parent_half = get_anc(v, i - 1);
                if (parent_half != -1) {
                    set_anc(v, i) = get_anc(parent_half, i - 1);
                }
            }
        }
    }

    template <typename T>
    explicit doubling_tree(i32 n_, i32 root_, const std::vector<std::vector<T>>& G)
        : doubling_tree(n_, root_, [&] {
              struct adj_edge { i32 u, v; };
              std::vector<adj_edge> E;
              E.reserve(n_ - 1);
              for (i32 v = 0; v < n_; ++v) {
                  for (T u : G[v]) {
                      if (v < static_cast<i32>(u)) E.emplace_back(adj_edge{v, static_cast<i32>(u)});
                  }
              }
              return E;
          }()) {}

    i32 kth_anc(i32 v, i32 k) const {
        assert(0 <= v && v < n);
        assert(k >= 0);
        if (k > d[v]) return -1;

        while (k > 0 && v != -1) {
            i32 i = std::countr_zero(static_cast<u32>(k));
            v = get_anc(v, i);
            k &= k - 1;
        }
        return v;
    }

    inline i32 depth(i32 v) const {
        assert(0 <= v && v < n);
        return d[v];
    }

    i32 lca(i32 u, i32 v) const {
        assert(0 <= u && u < n);
        assert(0 <= v && v < n);

        if (d[u] < d[v]) std::swap(u, v);
        u = kth_anc(u, d[u] - d[v]);

        if (u == v) return u;
        for (i32 i = log - 1; i >= 0; --i) {
            if (get_anc(u, i) != get_anc(v, i)) {
                u = get_anc(u, i);
                v = get_anc(v, i);
            }
        }
        return get_anc(u, 0);
    }

    i32 len(i32 u, i32 v) const {
        assert(0 <= u && u < n);
        assert(0 <= v && v < n);

        return depth(u) + depth(v) - 2 * depth(lca(u, v)) + 1;
    }

private:
    inline i32 get_anc(i32 v, i32 i) const { return anc[static_cast<size_t>(i) * n + v]; }

    inline i32& set_anc(i32 v, i32 i) { return anc[static_cast<size_t>(i) * n + v]; }
};

}  // namespace gwen
