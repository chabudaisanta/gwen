// https://judge.yosupo.jp/problem/jump_on_tree
// # pragma GCC target("avx2")
#pragma GCC optimize("O3")
#pragma GCC optimize("unroll-loops")

#include <vector>

#include "gwen/graph/tree_lca.hpp"
#include "gwen/graph/graph.hpp"
#include "gwen/io.hpp"
#include "gwen/types.hpp"

using i32 = gwen::i32;
using gwen::cin;
using gwen::cout;

void solve() {
    int N, Q;
    cin >> N >> Q;
    gwen::NonDirectedGraph<i32> g(N);
    for (int i = 0; i < N - 1; ++i) {
        int u, v;
        cin >> u >> v;
        g.add_edge(u, v);
    }
    g.build();
    gwen::tree_lca T(0, g);
    while (Q--) {
        int s, t, i;
        cin >> s >> t >> i;

        int lca = T.lca(s, t);
        int ds = T.depth(s), dt = T.depth(t), dl = T.depth(lca);

        int len = ds + dt - 2 * dl;
        if (len < i)
            cout << "-1\n";
        else {
            if (ds - dl == i)
                cout << lca << '\n';
            else if (i < ds - dl)
                cout << T.kth_anc(s, i) << '\n';
            else
                cout << T.kth_anc(t, len - i) << '\n';
        }
    }
}

int main() {
    int t = 1;
    // std::cin >> t;
    while (t--) solve();
    return 0;
}
