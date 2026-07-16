#pragma once

#include <algorithm>
#include <utility>
#include <vector>

#include "gwen/graph/graph.hpp"
#include "gwen/types.hpp"

namespace gwen {

/**
 * @brief Heavy-Light Decomposition (重軽分解) を行うクラス
 * @tparam G 対象とするグラフの型 (graph コンセプトを満たす必要がある)
 */
template <graph G> class Hld {
public:
    i32 n;
    i32 root;
    std::vector<i32> parent;  // parent[v]: 親（根は -1）
    std::vector<i32> depth;   // depth[v]: 根からの深さ
    std::vector<i32> heavy;   // heavy[v]: 重い子の 1 つ（いなければ -1）
    std::vector<i32> head;    // head[v]: v が属する heavy path の先頭（根側）
    std::vector<i32> in;      // in[v]: 行きがけ順（euler tour の index、セグ木用）
    std::vector<i32> out;     // out[v]: 帰りがけ順（部分木は [in[v], out[v])）
    std::vector<i32> order;   // order[i]: in-index i の頂点

    /**
     * @brief 構築済みグラフと根の頂点番号から HLD を構築する
     * @param g 対象のグラフ
     * @param root_ 根とする頂点の番号 (デフォルト 0)
     */
    explicit Hld(const G& g, i32 root_ = 0) : n(g.size()), root(root_) {
        parent.assign(n, -1);
        depth.assign(n, 0);
        heavy.assign(n, -1);
        head.assign(n, -1);
        in.assign(n, 0);
        out.assign(n, 0);
        order.assign(n, 0);

        std::vector<i32> sz(n, 1);

        auto dfs_size = [&](i32 v, i32 p, auto self) -> void {
            parent[v] = p;
            if (p != -1) depth[v] = depth[p] + 1;
            for (i32 u : g.adjacent(v)) {
                if (u == p) continue;
                self(u, v, self);
                sz[v] += sz[u];
            }
            i32 max_sz = 0;
            for (i32 u : g.adjacent(v)) {
                if (u == p) continue;
                if (sz[u] > max_sz) {
                    max_sz = sz[u];
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
            for (i32 u : g.adjacent(v)) {
                if (u == parent[v] || u == heavy[v]) continue;
                self(u, u, self);
            }
            out[v] = cur;
        };
        dfs_hld(root, root, dfs_hld);
    }

    /**
     * @brief 頂点 u と v の最小共通祖先 (LCA) を求める
     * @param u 頂点
     * @param v 頂点
     * @return i32 LCA となる頂点
     */
    i32 get_lca(i32 u, i32 v) const {
        while (head[u] != head[v]) {
            if (depth[head[u]] < depth[head[v]]) std::swap(u, v);
            u = parent[head[u]];
        }
        return depth[u] < depth[v] ? u : v;
    }

    /**
     * @brief 頂点 u と v の間の距離 (エッジ数) を求める
     * @param u 頂点
     * @param v 頂点
     * @return i32 距離
     */
    i32 get_dist(i32 u, i32 v) const { return depth[u] + depth[v] - 2 * depth[get_lca(u, v)]; }

    /**
     * @brief パス u -> v をセグメント木などで管理しやすいように [inの区間] の列に分解する
     * @param u 始点
     * @param v 終点
     * @return std::vector<std::pair<i32, i32>> 半開区間 [l, r) のリスト
     */
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

    /**
     * @brief パス u -> v 上の k 番目の頂点（u が 0 番目）を取得する
     * @param u 始点
     * @param v 終点
     * @param k インデックス
     * @return i32 頂点番号。k がパス長を超える場合は -1。
     */
    i32 kth_on_path(i32 u, i32 v, i32 k) const {
        i32 l = get_lca(u, v);
        i32 d_u = depth[u] - depth[l];
        if (k <= d_u) return kth_ancestor(u, k);
        i32 d_v = depth[v] - depth[l];
        if (k <= d_u + d_v) return kth_ancestor(v, d_u + d_v - k);
        return -1;
    }

    /**
     * @brief 頂点 v の k 個上の祖先を取得する
     * @param v 頂点
     * @param k 遡る世代数
     * @return i32 頂点番号。存在しなければ -1。
     */
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

    /**
     * @brief 頂点 v を根とする部分木の in 区間 [in[v], out[v]) を取得する
     * @param v 頂点
     * @return std::pair<i32, i32> 半開区間 [l, r)
     */
    std::pair<i32, i32> subtree(i32 v) const { return {in[v], out[v]}; }

    /**
     * @brief 頂点 v の in-index（セグ木の index）を取得する
     * @param v 頂点
     * @return i32 インデックス
     */
    i32 idx(i32 v) const { return in[v]; }

    /**
     * @brief in-index から頂点番号を取得する
     * @param i インデックス
     * @return i32 頂点番号
     */
    i32 vertex(i32 i) const { return order[i]; }
};

}  // namespace gwen
