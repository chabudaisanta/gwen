#ifdef LOCAL
#else
#define NDEBUG
#endif
// # pragma GCC target("avx2")
#pragma GCC optimize("O2")
#pragma GCC optimize("unroll-loops")
#include <ctime>
#include <iostream>
#include <vector>

#include "gwen/dump.hpp"
#include "gwen/io.hpp"
#include "gwen/types.hpp"
#define rp(i, n) for (i32 i = 0; i < (i32)(n); ++i)
constexpr char EL = '\n';
#define BAR std::cerr << "-------------------------\n"
#include <atcoder/modint>
using mint998 = atcoder::modint998244353;
#include "gwen/algebra/basic_monoid.hpp"
#include "gwen/query/functional_query.hpp"
namespace gwen {

void solve() {
    i32 N, Q;
    i64 X;
    input >> N >> Q >> X;
    std::vector<i64> W(N);
    input >> W;

    std::vector<i64> PS(2 * N + 1);
    rp(i, 2 * N) { PS[i + 1] = PS[i] + W[i % N]; }
    std::vector<i64> A(N, X / PS[N] * N);
    X %= PS[N];
    std::vector<i32> P(N);
    rp(i, N) {
        P[i] = std::lower_bound(PS.begin() + i, PS.end(), PS[i] + X) - PS.begin();
        A[i] += P[i] - i;
        P[i] %= N;
    }
    DUMP(X);
    DUMP(P);
    DUMP(A);
    auto F = functional_query<sum_monoid<i64>>(P, 1ll << 41, [&](i32, i32) { return 1; });
    while (Q--) {
        i64 k;
        input >> k;
        auto [f, s] = F.query(0, k - 1, 0);
        DUMP(f, s);
        output << A[f] << EL;
    }

    for (auto v : F.pos) DUMP(v);
}

}  // namespace gwen

int main() {
    int t = 1;
    // std::cin >> t;
    while (t--) gwen::solve();
    std::cerr << "execution time: " << 1.0 * clock() / CLOCKS_PER_SEC * 1000 << "(ms)\n";
    return 0;
}