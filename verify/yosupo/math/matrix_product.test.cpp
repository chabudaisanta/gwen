#define PROBLEM "https://judge.yosupo.jp/problem/matrix_product"

#include <iostream>

#include "gwen/math/matrix.hpp"
#include "gwen/mod/modint.hpp"
#include "gwen/types.hpp"

using namespace gwen;

using mint = modint998244353;

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);


    i32 n, m, k;
    if (!(std::cin >> n >> m >> k)) return 0;

    Matrix<mint> a(n, m);
    for (i32 i = 0; i < n; ++i) {
        for (i32 j = 0; j < m; ++j) {
            i64 x;
            std::cin >> x;
            a[i][j] = mint(x);
        }
    }

    Matrix<mint> b(m, k);
    for (i32 i = 0; i < m; ++i) {
        for (i32 j = 0; j < k; ++j) {
            i64 x;
            std::cin >> x;
            b[i][j] = mint(x);
        }
    }

    auto c = a * b;

    for (i32 i = 0; i < n; ++i) {
        for (i32 j = 0; j < k; ++j) {
            std::cout << c[i][j].val() << (j + 1 == k ? "" : " ");
        }
        std::cout << "\n";
    }

    return 0;
}
