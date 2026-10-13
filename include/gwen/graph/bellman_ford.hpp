#pragma once

#include <algorithm>
#include <cassert>
#include <concepts>
#include <limits>
#include <queue>
#include <utility>
#include <vector>

#include "gwen/graph/graph.hpp"
#include "gwen/types.hpp"

namespace gwen {

/**
 * @brief 負閉路を考慮した単一始点最短距離の結果
 * @tparam T 辺重みと距離の型
 */
template <std::signed_integral T> struct BellmanFordResult {
    /// @brief 各頂点への距離。到達不能は最大値、負閉路の影響下は無効な 0。
    std::vector<T> dist;
    /// @brief 始点から到達可能な負閉路から到達できる頂点。
    std::vector<bool> negative_infinite;
};

namespace internal {

struct BellmanFordEdge {
    i32 u, v;
    i128 w;
};

constexpr i128 bellman_ford_inf = i128{1} << 120;

template <graph G> std::vector<BellmanFordEdge> bellman_ford_edges(const G& g, bool reverse = false) {
    std::vector<BellmanFordEdge> edges;
    for (i32 u = 0; u < g.size(); ++u) {
        for (const auto& e : g.edges(u)) {
            if (reverse)
                edges.push_back({e.v, u, static_cast<i128>(e.w)});
            else
                edges.push_back({u, e.v, static_cast<i128>(e.w)});
        }
    }
    return edges;
}

struct BellmanFordState {
    std::vector<i128> dist;
    std::vector<bool> updated;
};

inline BellmanFordState bellman_ford_relax(i32 n,
                                           const std::vector<BellmanFordEdge>& edges,
                                           std::vector<i128> dist,
                                           i32 passes) {
    std::vector<bool> updated(n);
    for (i32 i = 0; i < passes; ++i) {
        std::fill(updated.begin(), updated.end(), false);
        bool changed = false;
        for (const auto& e : edges) {
            if (dist[e.u] == bellman_ford_inf) continue;
            i128 next = dist[e.u] + e.w;
            if (dist[e.v] <= next) continue;
            dist[e.v] = next;
            updated[e.v] = true;
            changed = true;
        }
        if (!changed) break;
    }
    return {std::move(dist), std::move(updated)};
}

inline std::vector<bool> bellman_ford_reachable(i32 n,
                                                const std::vector<BellmanFordEdge>& edges,
                                                std::vector<bool> seeds) {
    std::vector<std::vector<i32>> adj(n);
    for (const auto& e : edges) adj[e.u].push_back(e.v);
    std::queue<i32> q;
    for (i32 v = 0; v < n; ++v) {
        if (seeds[v]) q.push(v);
    }
    while (!q.empty()) {
        i32 u = q.front();
        q.pop();
        for (i32 v : adj[u]) {
            if (seeds[v]) continue;
            seeds[v] = true;
            q.push(v);
        }
    }
    return seeds;
}

template <std::signed_integral T>
std::vector<T> bellman_ford_dist(const std::vector<i128>& dist, const std::vector<bool>* invalid = nullptr) {
    std::vector<T> result(dist.size());
    for (usize i = 0; i < dist.size(); ++i) {
        if (invalid && (*invalid)[i]) {
            result[i] = T{0};
        }
        else if (dist[i] == bellman_ford_inf) {
            result[i] = std::numeric_limits<T>::max();
        }
        else {
            assert(static_cast<i128>(std::numeric_limits<T>::min()) <= dist[i]);
            assert(dist[i] < static_cast<i128>(std::numeric_limits<T>::max()));
            result[i] = static_cast<T>(dist[i]);
        }
    }
    return result;
}

}  // namespace internal

/**
 * @brief 負閉路の検出をせず単一始点最短距離を求める
 * @tparam G グラフ型
 * @param g 構築済みグラフ
 * @param s 始点
 * @return 各頂点への距離。到達不能は重み型の最大値。
 */
template <graph G>
    requires std::signed_integral<typename G::weight_type> && (sizeof(typename G::weight_type) <= sizeof(i64))
std::vector<typename G::weight_type> bellman_ford(const G& g, i32 s) {
    assert(0 <= s && s < g.size());
    auto edges = internal::bellman_ford_edges(g);
    std::vector<i128> dist(g.size(), internal::bellman_ford_inf);
    dist[s] = 0;
    auto state = internal::bellman_ford_relax(g.size(), edges, std::move(dist), g.size() - 1);
    return internal::bellman_ford_dist<typename G::weight_type>(state.dist);
}

/**
 * @brief 負閉路の影響を判定して単一始点最短距離を求める
 * @tparam G グラフ型
 * @param g 構築済みグラフ
 * @param s 始点
 * @return 距離と負閉路の影響下にある頂点の配列
 */
template <graph G>
    requires std::signed_integral<typename G::weight_type> && (sizeof(typename G::weight_type) <= sizeof(i64))
BellmanFordResult<typename G::weight_type> bellman_ford_with_negative_cycle(const G& g, i32 s) {
    assert(0 <= s && s < g.size());
    auto edges = internal::bellman_ford_edges(g);
    std::vector<i128> dist(g.size(), internal::bellman_ford_inf);
    dist[s] = 0;
    auto state = internal::bellman_ford_relax(g.size(), edges, std::move(dist), g.size());
    auto affected = internal::bellman_ford_reachable(g.size(), edges, std::move(state.updated));
    auto result_dist = internal::bellman_ford_dist<typename G::weight_type>(state.dist, &affected);
    return {std::move(result_dist), std::move(affected)};
}

/**
 * @brief 負閉路へ到達できる頂点をグラフ全体で求める
 * @tparam G グラフ型
 * @param g 構築済みグラフ
 * @return 各頂点が負閉路へ到達できるかを表す配列
 */
template <graph G>
    requires std::signed_integral<typename G::weight_type> && (sizeof(typename G::weight_type) <= sizeof(i64))
std::vector<bool> vertices_reaching_negative_cycle(const G& g) {
    auto edges = internal::bellman_ford_edges(g, true);
    auto state = internal::bellman_ford_relax(g.size(), edges, std::vector<i128>(g.size(), 0), g.size());
    return internal::bellman_ford_reachable(g.size(), edges, std::move(state.updated));
}

/**
 * @brief 負閉路から到達できる頂点をグラフ全体で求める
 * @tparam G グラフ型
 * @param g 構築済みグラフ
 * @return 各頂点へ負閉路から到達できるかを表す配列
 */
template <graph G>
    requires std::signed_integral<typename G::weight_type> && (sizeof(typename G::weight_type) <= sizeof(i64))
std::vector<bool> vertices_reachable_from_negative_cycle(const G& g) {
    auto edges = internal::bellman_ford_edges(g);
    auto state = internal::bellman_ford_relax(g.size(), edges, std::vector<i128>(g.size(), 0), g.size());
    return internal::bellman_ford_reachable(g.size(), edges, std::move(state.updated));
}

}  // namespace gwen
