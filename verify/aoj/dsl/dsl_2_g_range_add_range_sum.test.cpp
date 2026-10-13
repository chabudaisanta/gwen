#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/problems/DSL_2_G"

#include <iostream>

#include "gwen/query/range_add_range_sum.hpp"
#include "gwen/types.hpp"

using namespace gwen;

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    i32 n, q;
    std::cin >> n >> q;
    RangeAddRangeSum<i64> seg(n);

    while (q--) {
        i32 type;
        std::cin >> type;
        if (type == 0) {
            i32 l, r;
            i64 x;
            std::cin >> l >> r >> x;
            seg.add(l - 1, r, x);
        }
        else {
            i32 l, r;
            std::cin >> l >> r;
            std::cout << seg.prod(l - 1, r) << '\n';
        }
    }
}
