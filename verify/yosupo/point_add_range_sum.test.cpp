#define PROBLEM "https://judge.yosupo.jp/problem/point_add_range_sum"

#include <iostream>

#include "gwen/alge/abel.hpp"
#include "gwen/ds/fenwick_tree.hpp"

using namespace gwen;

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, q;
    if (!(std::cin >> n >> q)) return 0;

    std::vector<long long> a(n);
    for (int i = 0; i < n; ++i) {
        std::cin >> a[i];
    }

    FenwickTree<sum_abel<long long>> bit(a);

    for (int i = 0; i < q; ++i) {
        int type;
        std::cin >> type;
        if (type == 0) {
            int p;
            long long x;
            std::cin >> p >> x;
            bit.add(p, x);
        } else {
            int l, r;
            std::cin >> l >> r;
            std::cout << bit.sum(l, r) << "\n";
        }
    }

    return 0;
}
