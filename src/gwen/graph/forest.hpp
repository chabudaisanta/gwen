#pragma once

#include <vector>

#include "gwen/types.hpp"
namespace gwen {

struct forest {
    i32 N;
    i32 lg;
    std::vector<i32> root;
    std::vector<i32> depth;
    std::vector<i32> parent;

    explicit forest(const std::vector<std::vector<i32>>& G) : N(G.size()) {
        lg = 0;
        while((1 << lg) <= N) lg++;
        root.assign(N, -1);
        depth.assign(N, -1);
        parent.assign(lg * N, -1);
        
        auto dfs = [&](i32 cur, i32 par, i32 r, i32 d, auto self) -> i32 {
            if(cur != r) root[cur] = r;
            depth[cur] = d;
            parent[0 * N + cur] = par;
            i32 sz = 1;
            for(i32 nxt : G[cur]) if(nxt != par) {
                sz += self(nxt, cur, r, d + 1, self);
            }
            return sz;
        };

        for(i32 i = 0; i < N; ++i) if(depth[i] == -1) {
            root[i] = -1 * dfs(i, -1, i, 0, dfs);
        }

        for(i32 k = 0; k < lg - 1; ++k) {
            for(i32 i = 0; i < N; ++i) {
                i32 p = parent[k * N + i];
                if(p != -1) {
                    parent[(k + 1) * N + i] = parent[k * N + p];
                }
            }
        }
    }

    i32 get_root(i32 p) const {
        return root[p] < 0 ? p : root[p];
    }
    i32 get_size(i32 p) const {
        return -root[get_root(p)];
    }
    bool is_same(i32 u, i32 v) const {
        return get_root(u) == get_root(v);
    }
    i32 get_depth(i32 p) const {
        return depth[p];
    }
    i32 get_kth_ancestor(i32 v, i32 k) const {
        if (k < 0 || k > depth[v]) return -1;
        for (i32 i = 0; i < lg; ++i) {
            if ((k >> i) & 1) {
                v = parent[i * N + v];
                if (v == -1) break;
            }
        }
        return v;
    }
    i32 get_lca(i32 u, i32 v) const {
        if (!is_same(u, v)) return -1;
        if (depth[u] < depth[v]) std::swap(u, v);
        
        u = get_kth_ancestor(u, depth[u] - depth[v]);
        if (u == v) return u;

        for (i32 i = lg - 1; i >= 0; --i) {
            i32 pu = parent[i * N + u];
            i32 pv = parent[i * N + v];
            if (pu != pv) {
                u = pu;
                v = pv;
            }
        }
        return parent[0 * N + u];
    }
    i32 get_dist(i32 u, i32 v) const {
        if (!is_same(u, v)) return -1;
        return depth[u] + depth[v] - 2 * depth[get_lca(u, v)];
    }
};

}