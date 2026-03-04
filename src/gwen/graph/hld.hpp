#pragma once

#include <algorithm>
#include <utility>
#include <vector>

#include "gwen/types.hpp"

namespace gwen {

// Heavy-Light Decomposition (重軽分解)
// 根付き木を heavy edge の連鎖に分解し、任意パスを O(log N) 個の区間に分割する。
// 1 つの根のみをサポート（森の場合は各連結成分ごとに HLD を構築すること）。
struct hld {
    i32 n;
    i32 root;
    std::vector<i32> parent;   // parent[v]: 親（根は -1）
    std::vector<i32> depth;    // depth[v]: 根からの深さ
    std::vector<i32> heavy;    // heavy[v]: 重い子の 1 つ（いなければ -1）
    std::vector<i32> head;     // head[v]: v が属する heavy path の先頭（根側）
    std::vector<i32> in;       // in[v]: 行きがけ順（euler tour の index、セグ木用）
    std::vector<i32> out;      // out[v]: 帰りがけ順（部分木は [in[v], out[v])）
    std::vector<i32> order;    // order[i]: in-index i の頂点

    explicit hld(i32 n_, i32 root_, const std::vector<std::vector<i32>>& G)
        : n(n_), root(root_) {
        parent.assign(n, -1);
        depth.assign(n, 0);
        heavy.assign(n, -1);
        head.assign(n, -1);
        in.assign(n, 0);
        out.assign(n, 0);
        order.assign(n, 0);

        std::vector<i32> size(n, 1);

        auto dfs_size = [&](i32 v, i32 p, auto self) -> void {
            parent[v] = p;
            if (p != -1) depth[v] = depth[p] + 1;
            for (i32 u : G[v]) {
                if (u == p) continue;
                self(u, v, self);
                size[v] += size[u];
            }
            i32 max_sz = 0;
            for (i32 u : G[v]) {
                if (u == p) continue;
                if (size[u] > max_sz) {
                    max_sz = size[u];
                    heavy[v] = u;
                }
            }
        };
        dfs_size(root, -1, dfs_size);

        i32 cur = 0;
        auto dfs_hld = [&](i32 v, i32 h, auto self) -> void {
            head[v] = h;
            in[v] = cur++;
            order[in[v]] = v;
            if (heavy[v] != -1) self(heavy[v], h, self);
            for (i32 u : G[v]) {
                if (u == parent[v] || u == heavy[v]) continue;
                self(u, u, self);
            }
            out[v] = cur;
        };
        dfs_hld(root, root, dfs_hld);
    }

    // 辺リストから構築（0-indexed, 無向）
    template <typename Edge>
    explicit hld(i32 n_, i32 root_, const std::vector<Edge>& edges)
        : hld(n_, root_, [&] {
              std::vector<std::vector<i32>> G(n_);
              for (const auto& e : edges) {
                  G[e.u].push_back(e.v);
                  G[e.v].push_back(e.u);
              }
              return G;
          }()) {}

    i32 get_lca(i32 u, i32 v) const {
        while (head[u] != head[v]) {
            if (depth[head[u]] < depth[head[v]]) std::swap(u, v);
            u = parent[head[u]];
        }
        return depth[u] < depth[v] ? u : v;
    }

    i32 get_dist(i32 u, i32 v) const {
        return depth[u] + depth[v] - 2 * depth[get_lca(u, v)];
    }

    // パス u -> v を [in の区間] の列に分解する。各要素は [l, r) (半開区間)。
    // セグ木で path クエリするときに使う。
    std::vector<std::pair<i32, i32>> path(i32 u, i32 v) const {
        std::vector<std::pair<i32, i32>> res;
        while (head[u] != head[v]) {
            if (depth[head[u]] < depth[head[v]]) std::swap(u, v);
            res.emplace_back(in[head[u]], in[u] + 1);
            u = parent[head[u]];
        }
        if (depth[u] > depth[v]) std::swap(u, v);
        res.emplace_back(in[u], in[v] + 1);
        return res;
    }

    // パス u -> v 上の k 番目の頂点（u が 0 番目）。k がパス長以上なら -1。
    i32 kth_on_path(i32 u, i32 v, i32 k) const {
        i32 l = get_lca(u, v);
        i32 d_u = depth[u] - depth[l];
        if (k <= d_u) return kth_ancestor(u, k);
        i32 d_v = depth[v] - depth[l];
        if (k <= d_u + d_v) return kth_ancestor(v, d_u + d_v - k);
        return -1;
    }

    // v の k 個上の祖先。存在しなければ -1。
    i32 kth_ancestor(i32 v, i32 k) const {
        if (k < 0 || k > depth[v]) return -1;
        while (k > 0) {
            i32 h = head[v];
            i32 step = depth[v] - depth[h];
            if (k <= step) return order[in[v] - k];
            k -= step + 1;
            v = parent[h];
        }
        return v;
    }

    // 部分木 v の in 区間 [in[v], out[v])。
    std::pair<i32, i32> subtree(i32 v) const { return {in[v], out[v]}; }

    // 頂点 v の in-index（セグ木の index）。
    i32 idx(i32 v) const { return in[v]; }

    // in-index から頂点へ。
    i32 vertex(i32 i) const { return order[i]; }
};

}  // namespace gwen
