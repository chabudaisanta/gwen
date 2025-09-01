# pragma GCC target("avx2")
# pragma GCC optimize("O3")
# pragma GCC optimize("unroll-loops")

#include <algorithm>
#include <iostream>
#include <limits>
#include <vector>
#include <queue>

#include "gwen/graph/doubling_tree.hpp"

using namespace std;
using namespace gwen;

void solve() {
    int N, Q; cin >> N >> Q;
    vector<vector<int>> G(N);
    for(int i = 1; i < N; ++i) {
        int p; cin >> p;
        G[p].emplace_back(i);
        G[i].emplace_back(p);
    }
    doubling_tree T(N, 0, G);
    while(Q--) {
        int u, v; cin >> u >> v;
        cout << T.lca(u, v) << '\n';
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    int t = 1;
    // std::cin >> t;
    while(t--) solve();
    return 0;
}