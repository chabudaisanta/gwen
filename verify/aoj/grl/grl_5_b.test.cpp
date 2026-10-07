#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=GRL_5_B"

// clang-format off
#include "gwen/graph/rerooting.hpp"
// clang-format on

#include <algorithm>
#include <iostream>

using namespace gwen;

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    i32 n;
    if (!(std::cin >> n)) return 0;
    NonDirectedGraph<i64> g(n);
    for (i32 i = 1; i < n; ++i) {
        i32 u, v;
        i64 w;
        std::cin >> u >> v >> w;
        g.add_edge(u, v, w);
    }
    g.build();

    const auto answer = rerooting<max_monoid<i64>>(
        g, [](i64 value, i32) { return std::max(value, i64{0}); },
        [](i64 value, const Edge<i64>& e) { return value + e.w; });
    for (i64 value : answer) std::cout << value << '\n';
}
