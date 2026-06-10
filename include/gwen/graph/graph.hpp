#pragma once

#include <cassert>
#include <concepts>
#include <ranges>
#include <span>
#include <vector>

#include "gwen/graph/csr.hpp"
#include "gwen/graph/edge.hpp"
#include "gwen/types.hpp"

namespace gwen {

template <typename T>
concept graph_concept = requires(const T& t, i32 u) {
    typename T::edge_type;
    typename T::weight_type;
    { t.N } -> std::convertible_to<i32>;
    { t[u] } -> std::ranges::range;
    requires std::convertible_to<std::ranges::range_reference_t<decltype(t[u])>, const typename T::edge_type&>;
};

template <edge_concept Edge, bool isDirected = true> struct graph_base {
public:
    using edge_type = Edge;
    using weight_type = typename Edge::weight_type;

    const i32 N;

private:
    bool built;
    std::vector<Edge> edge_buf;
    csr<Edge> G;

public:
    graph_base() : graph_base(0) {}
    explicit graph_base(i32 N_) : N(N_), built(false) {}
    graph_base(i32 N_, const std::vector<Edge>& E) : N(N_), built(true), edge_buf(E), G(N_, E, isDirected) {}

    void add_edge(const Edge& e) {
        assert(!built);
        edge_buf.push_back(e);
    }
    void add_edge(i32 u, i32 v) { add_edge(Edge(u, v)); }
    void add_edge(i32 u, i32 v, weight_type w) { add_edge(Edge(u, v, w)); }

    void build() {
        assert(!built);
        G = csr<Edge>(N, edge_buf, isDirected);
        built = true;
    }

    const std::vector<Edge>& edges() const { return edge_buf; }

    std::span<const Edge> operator[](i32 u) const {
        assert(built);
        return G[u];
    }
};

template <typename T> using DirectedGraph = graph_base<edge<T>, true>;
template <typename T> using NonDirectedGraph = graph_base<edge<T>, false>;

}  // namespace gwen
