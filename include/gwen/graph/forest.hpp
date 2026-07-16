#pragma once

#include <vector>
#include <algorithm>

#include "gwen/graph/graph.hpp"
#include "gwen/types.hpp"

namespace gwen {

/**
 * @brief 複数の木（森）に対する LCA や距離、k 番目の祖先を求めるデータ構造
 * @tparam Edge エッジの型 (gwen::edge コンセプトを満たす)
 * @tparam IsDirected グラフが有向か無向か
 * @details 全頂点に対して DFS を行い、ダブリングのテーブルを構築します。
 */
template <edge Edge, bool IsDirected>
class Forest {
private:
    i32 n_;
    i32 lg_;
    std::vector<i32> root_;
    std::vector<i32> depth_;
    std::vector<i32> parent_;

public:
    /**
     * @brief コンストラクタ。与えられたグラフからダブリング配列を構築します。
     * @param G グラフ
     */
    explicit Forest(const GraphBase<Edge, IsDirected>& G) : n_(G.size()) {
        lg_ = 1;
        while ((1 << lg_) <= n_) lg_++;
        root_.assign(n_, -1);
        depth_.assign(n_, -1);
        parent_.assign(lg_ * n_, -1);

        auto dfs = [&](i32 cur, i32 par, i32 r, i32 d, auto& self) -> i32 {
            if (cur != r) root_[cur] = r;
            depth_[cur] = d;
            parent_[cur * lg_ + 0] = par;
            i32 sz = 1;
            for (i32 nxt : G.adjacent(cur)) {
                if (nxt != par) {
                    sz += self(nxt, cur, r, d + 1, self);
                }
            }
            return sz;
        };

        for (i32 i = 0; i < n_; ++i) {
            if (depth_[i] == -1) {
                root_[i] = -1 * dfs(i, -1, i, 0, dfs);
            }
        }

        for (i32 k = 0; k < lg_ - 1; ++k) {
            for (i32 i = 0; i < n_; ++i) {
                i32 p = parent_[i * lg_ + k];
                if (p != -1) {
                    parent_[i * lg_ + k + 1] = parent_[p * lg_ + k];
                }
            }
        }
    }

    /**
     * @brief 頂点 p の属する木の根を返します
     * @param p 頂点
     * @return 根の頂点番号
     */
    i32 get_root(i32 p) const { return root_[p] < 0 ? p : root_[p]; }

    /**
     * @brief 頂点 p の属する木の頂点数を返します
     * @param p 頂点
     * @return 木のサイズ
     */
    i32 get_size(i32 p) const { return -root_[get_root(p)]; }

    /**
     * @brief 頂点 u と v が同じ木に属するかを判定します
     * @param u 頂点
     * @param v 頂点
     * @return 同じ木なら true
     */
    bool is_same(i32 u, i32 v) const { return get_root(u) == get_root(v); }

    /**
     * @brief 頂点 p の深さ（根からの距離）を返します
     * @param p 頂点
     * @return 深さ
     */
    i32 get_depth(i32 p) const { return depth_[p]; }

    /**
     * @brief 頂点 v の k 番目の祖先を返します
     * @param v 頂点
     * @param k 何個上の祖先か
     * @return 祖先の頂点番号。存在しない場合は -1
     */
    i32 get_kth_ancestor(i32 v, i32 k) const {
        if (k < 0 || k > depth_[v]) return -1;
        for (i32 i = 0; i < lg_; ++i) {
            if ((k >> i) & 1) {
                v = parent_[v * lg_ + i];
                if (v == -1) break;
            }
        }
        return v;
    }

    /**
     * @brief 頂点 u と v の LCA (最小共通祖先) を返します
     * @param u 頂点
     * @param v 頂点
     * @return LCA の頂点番号。同じ木にない場合は -1
     */
    i32 get_lca(i32 u, i32 v) const {
        if (!is_same(u, v)) return -1;
        if (depth_[u] < depth_[v]) std::swap(u, v);

        u = get_kth_ancestor(u, depth_[u] - depth_[v]);
        if (u == v) return u;

        for (i32 i = lg_ - 1; i >= 0; --i) {
            i32 pu = parent_[u * lg_ + i];
            i32 pv = parent_[v * lg_ + i];
            if (pu != pv) {
                u = pu;
                v = pv;
            }
        }
        return parent_[u * lg_ + 0];
    }

    /**
     * @brief 頂点 u と v の距離 (エッジ数) を返します
     * @param u 頂点
     * @param v 頂点
     * @return 距離。同じ木にない場合は -1
     */
    i32 get_dist(i32 u, i32 v) const {
        if (!is_same(u, v)) return -1;
        return depth_[u] + depth_[v] - 2 * depth_[get_lca(u, v)];
    }
};

} // namespace gwen
