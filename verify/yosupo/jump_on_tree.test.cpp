#define PROBLEM "https://judge.yosupo.jp/problem/jump_on_tree"

#include <iostream>

#include "gwen/graph/forest.hpp"

using namespace gwen;

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, q;
    if (!(std::cin >> n >> q)) return 0;

    NonDirectedGraph<int> G(n);
    for (int i = 0; i < n - 1; ++i) {
        int u, v;
        std::cin >> u >> v;
        G.add_edge(u, v);
        G.add_edge(v, u);
    }
    G.build();

    Forest<Edge<int>, false> forest(G);

    for (int i = 0; i < q; ++i) {
        int u, v, k;
        std::cin >> u >> v >> k;

        int lca = forest.get_lca(u, v);
        int dist_u = forest.get_depth(u) - forest.get_depth(lca);
        int dist_v = forest.get_depth(v) - forest.get_depth(lca);

        if (k <= dist_u) {
            std::cout << forest.get_kth_ancestor(u, k) << "\n";
        } else if (k <= dist_u + dist_v) {
            std::cout << forest.get_kth_ancestor(v, dist_u + dist_v - k) << "\n";
        } else {
            std::cout << -1 << "\n";
        }
    }

    return 0;
}
