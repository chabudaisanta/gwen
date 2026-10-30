#define PROBLEM "https://judge.yosupo.jp/problem/staticrmq"

#include <iostream>
#include <vector>

#include "gwen/alge/monoid.hpp"
#include "gwen/ds/sparse_table.hpp"
#include "gwen/types.hpp"

using namespace gwen;

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    i32 n, q;
    std::cin >> n >> q;
    std::vector<i32> values(usize(n), 0);
    for (auto& value : values) std::cin >> value;

    SparseTable<min_monoid<i32>> table(values);
    while (q--) {
        i32 l, r;
        std::cin >> l >> r;
        std::cout << table.prod(l, r) << '\n';
    }
}
