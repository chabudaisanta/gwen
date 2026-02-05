#pragma once

#include <utility>
#include <vector>

#include "gwen/types.hpp"

namespace gwen {

template <typename Edge>
struct csr {
    std::vector<i32> start;
    std::vector<Edge> elist;

    csr() = default;
    csr(i32 n, const std::vector<Edge>& edges, bool directed = true)
        : start(n + 1, 0) {
        std::vector<Edge> tmp;
        if (directed) {
            tmp = edges;
        } else {
            tmp.reserve(edges.size() * 2);
            for (const auto& e : edges) {
                tmp.push_back(e);
                Edge rev = e;
                rev.u = e.v;
                rev.v = e.u;
                tmp.push_back(rev);
            }
        }
        for (const auto& e : tmp) {
            start[e.u + 1]++;
        }
        for (i32 i = 1; i <= n; ++i) {
            start[i] += start[i - 1];
        }
        elist.resize(tmp.size());
        auto counter = start;
        for (const auto& e : tmp) {
            elist[counter[e.u]++] = e;
        }
    }

    i32 size() const { return static_cast<i32>(start.size()) - 1; }
    i32 num_edges() const { return static_cast<i32>(elist.size()); }

    // [l, r) = edges from vertex v
    std::pair<i32, i32> operator[](i32 v) const {
        return {start[v], start[v + 1]};
    }
};

}  // namespace gwen
