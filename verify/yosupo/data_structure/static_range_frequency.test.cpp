#define PROBLEM "https://judge.yosupo.jp/problem/static_range_frequency"

#include <iostream>
#include <vector>

#include "gwen/ds/wavelet_matrix.hpp"
#include "gwen/types.hpp"

using namespace gwen;

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, q;
    std::cin >> n >> q;

    std::vector<int> a(n);
    for (int i = 0; i < n; ++i) {
        std::cin >> a[i];
    }

    WaveletMatrix<int> wm(a);

    for (int i = 0; i < q; ++i) {
        int l, r, x;
        std::cin >> l >> r >> x;
        std::cout << wm.range_freq(l, r, x, x + 1) << "\n";
    }

    return 0;
}
