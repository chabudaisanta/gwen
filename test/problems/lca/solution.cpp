#include "gwen/graph/graph.hpp"
#include "gwen/graph/tree_lca.hpp"
#include "gwen/my_template.hpp"

using namespace std;
using namespace gwen;

void solve() {
    i32 n, q;
    cin >> n >> q;
    NonDirectedGraph<i32> g(n);
    for (i32 i = 1; i < n; ++i) {
        i32 p;
        cin >> p;
        g.add_edge(p, i);
    }
    g.build();
    tree_lca lca(0, g);
    while (q--) {
        i32 u, v;
        cin >> u >> v;
        cout << lca.lca(u, v) << '\n';
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t = 1;
    while (t--) solve();
    return 0;
}
