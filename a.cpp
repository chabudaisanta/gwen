#ifdef LOCAL
#else
#define NDEBUG
#endif
// # pragma GCC target("avx2")
#pragma GCC optimize("O2")
#pragma GCC optimize("unroll-loops")
#include "gwen/geo/graham_scan.hpp"
#include <iostream>
#include <vector>
#include <ctime>

#include "gwen/dump.hpp"
#include "gwen/io.hpp"
#include "gwen/types.hpp"
#define rp(i, n) for (i32 i = 0; i < (i32)(n); ++i)
constexpr char EL = '\n';
#define BAR std::cerr << "-------------------------\n"
#include <atcoder/modint>
#include "gwen/algebra/basic_monoid.hpp"
#include "gwen/query/swag.hpp"
namespace gwen {
void solve() {
    i32 N; input >> N;
    std::vector<i32> A(N); input >> A;
    auto S = sliding_window_aggregation<sum_monoid<i32>>(A);
    i32 Q; input >> Q;
    while(Q--) {
        i32 l, r; input >> l >> r; --l;
        S.add_query(l, r);
    }
    auto res = S.solve();
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