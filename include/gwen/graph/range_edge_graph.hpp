#pragma once

#include <algorithm>
#include <bit>
#include <cassert>
#include <optional>
#include <vector>

#include "gwen/graph/graph.hpp"

namespace gwen {

template <typename T>
class range_edge_graph {
public:
    using edge_type = edge<T>;
    using weight_type = T;

    i32 N;

private:
    i32 original_N;
    i32 C;
    i32 aux_count;
    std::vector<edge_type> edge_buf;
    bool built;
    std::optional<DirectedGraph<T>> G;

    i32 up_node(i32 idx) const {
        return idx >= C ? idx - C : idx + C;
    }
    i32 down_node(i32 idx) const {
        return idx >= C ? idx - C : idx + 2 * C;
    }

public:
    explicit range_edge_graph(i32 N_) : N(N_), original_N(N_), C(std::bit_ceil(static_cast<u32>(std::max(1, N_)))), aux_count(0), built(false) {
        for (i32 i = 0; i < original_N; ++i) {
            i32 idx = i + C;
            i32 p = idx / 2;
            edge_buf.emplace_back(i, p + C, 0);
            edge_buf.emplace_back(p + 2 * C, i, 0);
        }
        for (i32 idx = 2; idx < C; ++idx) {
            i32 p = idx / 2;
            edge_buf.emplace_back(idx + C, p + C, 0);
            edge_buf.emplace_back(p + 2 * C, idx + 2 * C, 0);
        }
    }

    void add_edge(i32 u, i32 v, T w) {
        assert(!built);
        edge_buf.emplace_back(u, v, w);
    }

    void add_range_edge(i32 l0, i32 r0, i32 l1, i32 r1, T w) {
        assert(!built);
        i32 m = 3 * C + aux_count++;
        
        i32 l = l0 + C, r = r0 + C;
        while (l < r) {
            if (l & 1) edge_buf.emplace_back(up_node(l++), m, w);
            if (r & 1) edge_buf.emplace_back(up_node(--r), m, w);
            l >>= 1; r >>= 1;
        }

        l = l1 + C; r = r1 + C;
        while (l < r) {
            if (l & 1) edge_buf.emplace_back(m, down_node(l++), 0);
            if (r & 1) edge_buf.emplace_back(m, down_node(--r), 0);
            l >>= 1; r >>= 1;
        }
    }

    void build() {
        assert(!built);
        N = 3 * C + aux_count;
        G.emplace(N, edge_buf);
        built = true;
    }

    std::span<const edge_type> operator[](i32 u) const {
        assert(built);
        return (*G)[u];
    }

    const std::vector<edge_type>& edges() const { return edge_buf; }
};

} // namespace gwen
