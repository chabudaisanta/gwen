#pragma once

#include <algorithm>
#include <bit>
#include <cassert>
#include <vector>

#include "gwen/graph/graph.hpp"
#include "gwen/types.hpp"

namespace gwen {

/**
 * @brief 単一の木に対する LCA や距離を求めるデータ構造
 * @tparam Edge エッジの型 (gwen::edge コンセプトを満たす)
 * @tparam IsDirected グラフが有向か無向か
 * @details ダブリングを用いて各クエリに O(log N) で応答します。
 */
template <edge Edge, bool IsDirected>
class TreeLca {
private:
    i32 n_, log_, root_;
    std::vector<i32> anc_;
    std::vector<i32> d_;

    i32 get_anc(i32 v, i32 i) const { return anc_[static_cast<size_t>(i) * n_ + v]; }
    i32& set_anc(i32 v, i32 i) { return anc_[static_cast<size_t>(i) * n_ + v]; }

public:
    /**
     * @brief コンストラクタ。与えられた木からダブリング配列を構築します。
     * @param root_ 根とする頂点
     * @param G グラフ
     */
    explicit TreeLca(i32 root, const GraphBase<Edge, IsDirected>& G) {
        n_ = G.size();
        root_ = root;
        log_ = n_ > 0 ? std::bit_width(static_cast<u32>(n_)) : 0;
        anc_.assign(static_cast<size_t>(log_) * n_, -1);
        d_.assign(n_, 0);

        auto dfs = [&](i32 cur, i32 par, i32 depth, auto self) -> void {
            if (par != -1) {
                set_anc(cur, 0) = par;
            }
            d_[cur] = depth;
            for (i32 nxt : G.adjacent(cur)) {
                if (nxt != par) {
                    self(nxt, cur, depth + 1, self);
                }
            }
        };

        if (n_ > 0) {
            dfs(root_, -1, 0, dfs);
        }

        for (i32 i = 1; i < log_; ++i) {
            for (i32 v = 0; v < n_; ++v) {
                i32 mid = get_anc(v, i - 1);
                if (mid != -1) set_anc(v, i) = get_anc(mid, i - 1);
            }
        }
    }

    /**
     * @brief 頂点 v の k 番目の祖先を返します
     * @param v 頂点
     * @param k 何個上の祖先か
     * @return 祖先の頂点番号。存在しない場合は -1
     */
    i32 kth_anc(i32 v, i32 k) const {
        assert(0 <= v && v < n_);
        assert(k >= 0);
        if (k > d_[v]) return -1;

        while (k > 0 && v != -1) {
            i32 i = std::countr_zero(static_cast<u32>(k));
            v = get_anc(v, i);
            k &= k - 1;
        }
        return v;
    }

    /**
     * @brief 頂点 v の深さ（根からの距離）を返します
     * @param v 頂点
     * @return 深さ
     */
    i32 depth(i32 v) const {
        assert(0 <= v && v < n_);
        return d_[v];
    }

    /**
     * @brief 頂点 u と v の LCA (最小共通祖先) を返します
     * @param u 頂点
     * @param v 頂点
     * @return LCA の頂点番号
     */
    i32 lca(i32 u, i32 v) const {
        assert(0 <= u && u < n_);
        assert(0 <= v && v < n_);

        if (d_[u] < d_[v]) std::swap(u, v);
        u = kth_anc(u, d_[u] - d_[v]);

        if (u == v) return u;
        for (i32 i = log_ - 1; i >= 0; --i) {
            if (get_anc(u, i) != get_anc(v, i)) {
                u = get_anc(u, i);
                v = get_anc(v, i);
            }
        }
        return get_anc(u, 0);
    }

    /**
     * @brief 頂点 u と v のパスに含まれる頂点の数を返します
     * @param u 頂点
     * @param v 頂点
     * @return 頂点数 (距離 + 1)
     */
    i32 len(i32 u, i32 v) const { return depth(u) + depth(v) - 2 * depth(lca(u, v)) + 1; }
};

} // namespace gwen
