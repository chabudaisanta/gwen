#define PROBLEM "https://judge.yosupo.jp/problem/inverse_matrix"

#include <iostream>

#include "gwen/math/matrix.hpp"
#include "gwen/mod/modint.hpp"
#include "gwen/types.hpp"

using namespace gwen;

using mint = DynamicModInt64;

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    mint::set_mod(998244353);

    i32 n;
    if (!(std::cin >> n)) return 0;

    Matrix<mint> a(n);
    for (i32 i = 0; i < n; ++i) {
        for (i32 j = 0; j < n; ++j) {
            i64 x;
            std::cin >> x;
            a[i][j] = mint(x);
        }
    }

    auto inv = a.inverse();
    if (inv.rows() == 0) {
        std::cout << "-1\n";
    }
    else {
        for (i32 i = 0; i < n; ++i) {
            for (i32 j = 0; j < n; ++j) {
                std::cout << inv[i][j].val() << (j + 1 == n ? "" : " ");
            }
            std::cout << "\n";
        }
    }

    return 0;
}
