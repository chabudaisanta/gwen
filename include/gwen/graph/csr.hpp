#pragma once

#include <span>
#include <vector>

#include "gwen/graph/edge.hpp"
#include "gwen/types.hpp"

namespace gwen {

template <edge_concept Edge> struct csr {
    std::vector<i32> start;
    std::vector<Edge> elist;

    csr() = default;
    csr(i32 n, const std::vector<Edge>& edges, bool directed = true) : start(n + 1, 0) {
        if (directed) {
            for (const auto& e : edges) {
                start[e.u + 1]++;
            }
            for (i32 i = 1; i <= n; ++i) {
                start[i] += start[i - 1];
            }
            elist.resize(edges.size());
            auto counter = start;
            for (const auto& e : edges) {
                elist[counter[e.u]++] = e;
            }
        } else {
            for (const auto& e : edges) {
                start[e.u + 1]++;
                start[e.v + 1]++;
            }
            for (i32 i = 1; i <= n; ++i) {
                start[i] += start[i - 1];
            }
            elist.resize(edges.size() * 2);
            auto counter = start;
            for (const auto& e : edges) {
                elist[counter[e.u]++] = e;
                Edge rev = e;
                rev.u = e.v;
                rev.v = e.u;
                elist[counter[rev.u]++] = rev;
            }
        }
    }

    i32 size() const { return static_cast<i32>(start.size()) - 1; }
    i32 num_edges() const { return static_cast<i32>(elist.size()); }

    std::span<Edge> operator[](i32 v) {
        return std::span<Edge>(elist).subspan(start[v], start[v + 1] - start[v]);
    }
    std::span<const Edge> operator[](i32 v) const {
        return std::span<const Edge>(elist).subspan(start[v], start[v + 1] - start[v]);
    }
};

}  // namespace gwen
