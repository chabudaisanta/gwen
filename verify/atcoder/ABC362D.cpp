// #pragma GCC target("avx2")
// #undef LOCAL
#pragma GCC optimize("Ofast")
#pragma GCC optimize("unroll-loops")
#include "gwen/graph/dijkstra.hpp"
#include "gwen/graph/graph.hpp"
#include "gwen/my_template.hpp"
using namespace std;
using namespace gwen;
void solve() {
    i32 N, M;
    cin >> N >> M;
    vector<i64> A(N);
    cin >> A;
    DirectedGraph<i64> G(N);
    rp(i, M) {
        i32 u, v;
        i64 b;
        cin >> u >> v >> b;
        --u;
        --v;
        G.add_edge(u, v, b + A[v]);
        G.add_edge(v, u, b + A[u]);
    }
    G.build();
    dijkstra solver(G);
    auto D = solver.solve(0);
    rep(i, 1, N) { cout << D[i] + A[0] << SPA; }
}

int main(void) {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    std::cout << std::fixed << std::setprecision(16);

    int t = 1;
    // cin >> t;
    while (t--) solve();

    std::cerr << "execution time: " << 1.0 * clock() / CLOCKS_PER_SEC * 1000 << "(ms)" << EL;
    return 0;
}