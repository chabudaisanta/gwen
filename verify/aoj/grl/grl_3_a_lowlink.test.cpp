#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=GRL_3_A"

// clang-format off
#include "gwen/graph/lowlink.hpp"
// clang-format on

#include <iostream>

using namespace gwen;

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    i32 n, m;
    std::cin >> n >> m;
    NonDirectedGraph<i32> g(n);
    for (i32 i = 0; i < m; ++i) {
        i32 u, v;
        std::cin >> u >> v;
        g.add_edge(u, v);
    }
    g.build();

    const Lowlink lowlink(g);
    for (i32 vertex : lowlink.articulation_points()) std::cout << vertex << '\n';
}
