#define PROBLEM "https://judge.yosupo.jp/problem/range_affine_range_sum"

#include <iostream>
#include <vector>

#include "gwen/mod/modint.hpp"
#include "gwen/query/range_add_update_sum.hpp"

using namespace gwen;
using mint = DynamicModInt64;

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    mint::set_mod(998244353);

    int n, q;
    std::cin >> n >> q;

    std::vector<mint> a(n);
    for (int i = 0; i < n; ++i) {
        int v;
        std::cin >> v;
        a[i] = v;
    }

    RangeAddUpdateSum<mint> seg(a);

    for (int i = 0; i < q; ++i) {
        int t;
        std::cin >> t;
        if (t == 0) {
            int l, r, b, c;
            std::cin >> l >> r >> b >> c;
            seg.affine(l, r, b, c);
        } else {
            int l, r;
            std::cin >> l >> r;
            std::cout << seg.prod(l, r).val() << "\n";
        }
    }

    return 0;
}
