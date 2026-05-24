#pragma once

#include <cassert>
#include <span>
#include <vector>

#include "gwen/graph/csr.hpp"
#include "gwen/graph/edge.hpp"
#include "gwen/types.hpp"

namespace gwen {

template <edge_type Edge> struct NonDirectedGraph {
    using weight_type = typename Edge::weight_type;
    i32 N;
    bool built;
    std::vector<Edge> edge_buf;
    csr<Edge> G;

    NonDirectedGraph() : NonDirectedGraph(0) {}
    explicit NonDirectedGraph(i32 N_) : N(N_), built(false) {}
    NonDirectedGraph(i32 N_, const std::vector<Edge>& E) : N(N_), built(true), G(N_, E, false) {}

    void add_edge(const Edge& e) {
        assert(!built);
        edge_buf.push_back(e);
    }
    void add_edge(i32 u, i32 v) { add_edge(Edge(u, v)); }
    void add_edge(i32 u, i32 v, weight_type w) { add_edge(Edge(u, v, w)); }

    void build() {
        assert(!built);
        G = csr<Edge>(N, edge_buf, false);
        built = true;
    }

    std::span<const Edge> operator[](i32 u) const {
        assert(built);
        return G[u];
    }
};

}  // namespace gwen
