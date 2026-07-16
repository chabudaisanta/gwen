#pragma once

#include <cassert>
#include <concepts>
#include <limits>
#include <queue>
#include <vector>

#include "gwen/graph/graph.hpp"
#include "gwen/types.hpp"

namespace gwen {

/**
 * @brief Dijkstra法の重みとして要求されるコンセプト
 */
template <typename W>
concept dijkstra_weight = std::totally_ordered<W> && std::default_initializable<W> && requires {
    { std::numeric_limits<W>::max() } -> std::convertible_to<W>;
} && requires(W a, W b) {
    { a + b } -> std::convertible_to<W>;
    { W(0) } -> std::same_as<W>;
};

/**
 * @brief Dijkstra法を適用可能なグラフのコンセプト
 */
template <typename G>
concept dijkstra_graph = graph<G> && dijkstra_weight<typename G::weight_type> &&
                         std::default_initializable<typename G::edge_type>;

/**
 * @brief 単一始点最短経路を求める Dijkstra 法の実装
 * @tparam G 対象とするグラフの型
 */
template <dijkstra_graph G>
class Dijkstra {
public:
    using weight_type = typename G::weight_type;
    using edge_type = typename G::edge_type;
    static constexpr weight_type INF = std::numeric_limits<weight_type>::max();

private:
    const G& g_;
    i32 n_;
    std::vector<edge_type> spt_;
    std::vector<weight_type> dist_;

public:
    /**
     * @brief 構築済みグラフを元に初期化する
     * @param g 対象のグラフ
     */
    explicit Dijkstra(const G& g) : g_(g), n_(g.size()), spt_(n_), dist_(n_) {}

    /**
     * @brief 頂点 s を始点として最短経路を計算する
     * @param s 始点
     * @return const std::vector<weight_type>& 各頂点への最短距離の配列
     */
    const std::vector<weight_type>& solve(i32 s) {
        assert(0 <= s && s < n_);
        spt_.assign(n_, edge_type());
        dist_.assign(n_, INF);

        using Node = std::pair<weight_type, i32>;
        std::priority_queue<Node, std::vector<Node>, std::greater<Node>> pq;
        dist_[s] = weight_type(0);
        pq.emplace(weight_type(0), s);

        while (!pq.empty()) {
            auto [d, cur] = pq.top();
            pq.pop();

            if (dist_[cur] != d) continue;
            for (const edge_type& e : g_.edges(cur)) {
                if (e.w > INF - d) continue;
                weight_type nd = d + e.w;
                if (dist_[e.v] <= nd) continue;
                dist_[e.v] = nd;
                spt_[e.v] = e;
                pq.emplace(nd, e.v);
            }
        }
        return dist_;
    }

    /**
     * @brief 計算済みの最短距離配列を取得する
     * @return const std::vector<weight_type>&
     */
    const std::vector<weight_type>& get_dist() const { return dist_; }

    /**
     * @brief 計算済みの最短経路木(Shortest Path Tree) を構成する親へ向かうエッジの配列を取得する
     * @return const std::vector<edge_type>&
     */
    const std::vector<edge_type>& get_spt() const { return spt_; }
};

} // namespace gwen
