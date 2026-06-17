#pragma once

#include <cassert>
#include <concepts>
#include <limits>
#include <queue>
#include <vector>

#include "gwen/graph/graph.hpp"
#include "gwen/types.hpp"

namespace gwen {

template <typename W>
concept dijkstra_weight_concept = std::totally_ordered<W> && std::default_initializable<W> && requires {
    { std::numeric_limits<W>::max() } -> std::convertible_to<W>;
} && requires(W a, W b) {
    { a + b } -> std::convertible_to<W>;
    { W(0) } -> std::same_as<W>;
};

template <typename G>
concept dijkstra_graph_concept = graph_concept<G> && dijkstra_weight_concept<typename G::weight_type> &&
                                 std::default_initializable<typename G::edge_type>;

template <dijkstra_graph_concept Graph> struct dijkstra {
public:
    using weight_type = typename Graph::weight_type;
    using edge_type = typename Graph::edge_type;
    static constexpr weight_type Inf = std::numeric_limits<weight_type>::max();

private:
    const i32 N;
    const Graph& G;
    std::vector<edge_type> spt;
    std::vector<weight_type> dist;

public:
    explicit dijkstra(const Graph& G_) : N(G_.N), G(G_), spt(N), dist(N) {}

    const std::vector<weight_type>& solve(i32 s) {
        assert(0 <= s && s < N);
        spt.assign(N, edge_type());
        dist.assign(N, Inf);

        using Node = std::pair<weight_type, i32>;
        std::priority_queue<Node, std::vector<Node>, std::greater<Node>> PQ;
        dist[s] = weight_type(0);
        PQ.emplace(weight_type(0), s);

        while (!PQ.empty()) {
            auto [d, cur] = PQ.top();
            PQ.pop();

            if (dist[cur] != d) continue;
            for (const edge_type& e : G[cur]) {
                if (e.w > Inf - d) continue;
                weight_type nd = d + e.w;
                if (dist[e.v] <= nd) continue;
                dist[e.v] = nd;
                spt[e.v] = e;
                PQ.emplace(nd, e.v);
            }
        }
        return dist;
    }

    const std::vector<weight_type>& get_dist() const { return dist; }
    const std::vector<edge_type>& get_spt() const { return spt; }
};

}  // namespace gwen
