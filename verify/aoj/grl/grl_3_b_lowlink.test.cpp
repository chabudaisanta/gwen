#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=GRL_3_B"

// clang-format off
#include "gwen/graph/lowlink.hpp"
// clang-format on

#include <algorithm>
#include <iostream>
#include <utility>
#include <vector>

using namespace gwen;

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    i32 n, m;
    std::cin >> n >> m;
    NonDirectedGraph<i32> g(n);
    std::vector<std::pair<i32, i32>> edges(m);
    for (auto& [u, v] : edges) {
        std::cin >> u >> v;
        g.add_edge(u, v);
    }
    g.build();

    const Lowlink lowlink(g);
    std::vector<std::pair<i32, i32>> bridges;
    for (usize id : lowlink.bridges()) {
        auto [u, v] = edges[id];
        if (u > v) std::swap(u, v);
        bridges.emplace_back(u, v);
    }
    std::sort(bridges.begin(), bridges.end());
    for (auto [u, v] : bridges) std::cout << u << ' ' << v << '\n';
}
