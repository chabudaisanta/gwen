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
#include "gwen/algebra/basic_monoid.hpp"
#include "gwen/query/swag.hpp"
namespace gwen {
using mint998 = atcoder::modint998244353;
using M = affine_monoid<mint998>;
void solve() {
    i32 Q; input >> Q;
    std::vector<i32> F(Q, 0);
    std::vector<M::S> A(Q, M::e());
    std::vector<std::tuple<i32,i32,mint998>> T;
    i32 r = Q, l = Q - 1;
    rp(i,Q) {
        i32 t; input >> t;
        if(t==0) {
            i32 a, b; input >> a >> b;
            A[Q - 1 - i] = { a, b };
            F[Q - 1 - i] = 1;
        }
        else if(t==1) {
            do {
                --r;
            } while(!F[r]);
        }
        else {
            i32 x; input >> x;
            T.emplace_back(l, r, x);
        }
        l--;
    }
    sliding_window_aggregation<M> SWAG(A);
    for(auto [l, r, x] : T) SWAG.add_query(l, r);
    auto res = SWAG.solve();
    rp(i,T.size()) {
        auto [l, r, x] = T[i];
        auto [a, b] = res[i];
        output << (a * x + b).val() << EL;
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