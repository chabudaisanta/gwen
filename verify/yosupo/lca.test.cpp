#define PROBLEM "https://judge.yosupo.jp/problem/lca"

#include <iostream>
#include <vector>

#include "gwen/graph/tree_lca.hpp"

using namespace gwen;

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, q;
    if (!(std::cin >> n >> q)) return 0;

    DirectedGraph<int> G(n);
    for (int i = 1; i < n; ++i) {
        int p;
        std::cin >> p;
        G.add_edge(p, i);
    }
    G.build();

    TreeLca<Edge<int>, true> lca(0, G);

    for (int i = 0; i < q; ++i) {
        int u, v;
        std::cin >> u >> v;
        std::cout << lca.lca(u, v) << "\n";
    }

    return 0;
}
