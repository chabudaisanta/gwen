#define PROBLEM "https://judge.yosupo.jp/problem/unionfind_with_potential"

#include <iostream>

#include "gwen/graph/weighted_dsu.hpp"
#include "gwen/mod/modint.hpp"

using namespace gwen;

using mint = DynamicModInt64;

struct MintAddAbel {
    using S = mint;
    static S op(S a, S b) { return a + b; }
    static S e() { return mint(0); }
    static S inv(S a) { return -a; }
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    mint::set_mod(998244353);

    int n, q;
    if (!(std::cin >> n >> q)) return 0;

    WeightedDsu<MintAddAbel> dsu(n);

    for (int i = 0; i < q; ++i) {
        int t;
        std::cin >> t;
        if (t == 0) {
            int u, v;
            int x;
            std::cin >> u >> v >> x;
            // p_v - p_u = x
            // merge(a, b, w) creates P(a) - P(b) = w
            // So merge(v, u, x)
            if(dsu.merge(v, u, mint(x)) == -1) {
                std::cout << 0 << '\n';
            }
            else {
                std::cout << 1 << '\n';
            }
        } else {
            int u, v;
            std::cin >> u >> v;
            if (dsu.same(u, v)) {
                std::cout << dsu.diff(v, u).val() << "\n";
            } else {
                std::cout << -1 << "\n";
            }
        }
    }

    return 0;
}
