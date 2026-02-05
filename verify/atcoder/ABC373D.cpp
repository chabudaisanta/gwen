// #pragma GCC target("avx2")
#pragma GCC optimize("Ofast")
#pragma GCC optimize("unroll-loops")
#include "gwen/my_template.hpp"
#include "gwen/graph/weighted_dsu.hpp"
#include "gwen/algebra/basic_abel.hpp"
using namespace std;
using Abel = gwen::sum_abel<i64>;
void solve() {
    i32 N, M; cin >> N >> M;
    gwen::weighted_dsu<Abel> UF(N);
    rp(i,M) {
        i32 u, v; i64 w; cin >> u >> v >> w;
        UF.merge(v - 1, u - 1, w);
    }
    rp(i,N) {
        cout << UF.potential(i) << ' ';
    }
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