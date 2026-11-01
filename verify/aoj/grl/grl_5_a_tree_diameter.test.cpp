#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=GRL_5_A"

// clang-format off
#include "gwen/graph/tree_diameter.hpp"
// clang-format on

#include <iostream>

using namespace gwen;

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    i32 n;
    std::cin >> n;
    NonDirectedGraph<i64> g(n);
    for (i32 i = 0; i < n - 1; ++i) {
        i32 u, v;
        i64 w;
        std::cin >> u >> v >> w;
        g.add_edge(u, v, w);
    }
    g.build();

    std::cout << tree_diameter(g).distance << '\n';
}
