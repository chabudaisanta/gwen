#ifdef LOCAL
#else
#define NDEBUG
#endif
// # pragma GCC target("avx2")
#pragma GCC optimize("O2")
#pragma GCC optimize("unroll-loops")
#include <iostream>
#include <vector>
#include <ctime>

#include "gwen/dump.hpp"
#include "gwen/io.hpp"
#include "gwen/types.hpp"
#define rp(i, n) for (i32 i = 0; i < (i32)(n); ++i)
constexpr char EL = '\n';
#define BAR std::cerr << "-------------------------\n"
#include "gwen/graph/doubling_tree.hpp"
#include "gwen/graph/edge.hpp"
namespace gwen {
void solve() {
    i32 N, Q; input >> N >> Q;
    std::vector<edge> E(N - 1);
    for(i32 i = 0; i < N - 1; ++i) {
        i32 p; input >> p;
        E[i] = {i + 1, p};
    }
    doubling_tree T(N, 0, E);
    while(Q--) {
        i32 u, v; input >> u >> v;
        output << T.lca(u, v) << EL;
    }
}

}

int main() {
    int t = 1;
    // std::cin >> t;
    while (t--) gwen::solve();
    std::cerr << "execution time: " << 1.0 * clock() / CLOCKS_PER_SEC * 1000
              << "(ms)\n";
    return 0;
}