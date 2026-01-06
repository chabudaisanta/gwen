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
#include <atcoder/modint>
using mint998 = atcoder::modint998244353;
#include "gwen/container/wrapper/range_add_update_sum.hpp"
namespace gwen {

void solve() {
    i32 N, M; input >> N >> M;
    std::vector<mint998> A(N);
    rp(i,N) {
        i32 a; input >> a;
        A[i] = a;
    }
    range_add_update_sum<mint998> seg(A);
    while(M--) {
        i32 l, r, x; input >> l >> r >> x; --l;
        mint998 n = r - l, inv = n.inv();
        seg.affine(l, r, 1 - inv, inv * x);
    }
    rp(i,N) output << seg.get(i).val() << ' ';
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