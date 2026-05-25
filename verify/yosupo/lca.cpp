#pragma GCC target("avx2")
#pragma GCC optimize("O3")
#pragma GCC optimize("unroll-loops")

#include <algorithm>
#include <iostream>
#include <limits>
#include <queue>
#include <vector>

#include "gwen/graph/tree_lca.hpp"
#include "gwen/graph/graph.hpp"

using namespace std;
using namespace gwen;

void solve() {
    int N, Q;
    cin >> N >> Q;
    NonDirectedGraph<i32> g(N);
    for (int i = 1; i < N; ++i) {
        int p;
        cin >> p;
        g.add_edge(p, i);
    }
    g.build();
    tree_lca T(0, g);
    while (Q--) {
        int u, v;
        cin >> u >> v;
        cout << T.lca(u, v) << '\n';
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    int t = 1;
    // std::cin >> t;
    while (t--) solve();
    return 0;
}
