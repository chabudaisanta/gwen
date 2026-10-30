#define PROBLEM "https://judge.yosupo.jp/problem/static_range_sum"

#include <iostream>
#include <vector>

#include "gwen/alge/monoid.hpp"
#include "gwen/ds/disjoint_sparse_table.hpp"
#include "gwen/types.hpp"

using namespace gwen;

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    i32 n, q;
    std::cin >> n >> q;
    std::vector<i64> values(usize(n), 0);
    for (auto& value : values) std::cin >> value;

    DisjointSparseTable<sum_monoid<i64>> table(values);
    while (q--) {
        i32 l, r;
        std::cin >> l >> r;
        std::cout << table.prod(l, r) << '\n';
    }
}
