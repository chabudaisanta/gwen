#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=GRL_1_B"

// clang-format off
#include "gwen/graph/bellman_ford.hpp"
// clang-format on

#include <iostream>
#include <limits>

using namespace gwen;

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    i32 n, m, s;
    std::cin >> n >> m >> s;
    DirectedGraph<i64> g(n);
    for (i32 i = 0; i < m; ++i) {
        i32 u, v;
        i64 w;
        std::cin >> u >> v >> w;
        g.add_edge(u, v, w);
    }
    g.build();

    auto result = bellman_ford_with_negative_cycle(g, s);
    for (bool affected : result.negative_infinite) {
        if (affected) {
            std::cout << "NEGATIVE CYCLE\n";
            return 0;
        }
    }
    for (i64 distance : result.dist) {
        if (distance == std::numeric_limits<i64>::max())
            std::cout << "INF\n";
        else
            std::cout << distance << '\n';
    }
}
