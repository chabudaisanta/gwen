#define PROBLEM "https://judge.yosupo.jp/problem/range_add_range_min"

#include "gwen/query/range_add_range_max.hpp"

#include <iostream>
#include <vector>

#include "gwen/types.hpp"

using namespace gwen;

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    i32 n, q;
    std::cin >> n >> q;
    std::vector<i64> negated_values(n);
    for (auto& value : negated_values) {
        std::cin >> value;
        value = -value;
    }
    RangeAddRangeMax<i64> seg(negated_values);

    while (q--) {
        i32 type, l, r;
        std::cin >> type >> l >> r;
        if (type == 0) {
            i64 x;
            std::cin >> x;
            seg.add(l, r, -x);
        }
        else {
            std::cout << -seg.prod(l, r) << '\n';
        }
    }
}
