// # pragma GCC target("avx2")
#pragma GCC optimize("O3")
#pragma GCC optimize("unroll-loops")

#include <iostream>
#include <vector>

#include "gwen/dump.hpp"
#include "gwen/graph/doubling_tree.hpp"
#include "gwen/io.hpp"
#include "gwen/types.hpp"

using i32 = gwen::i32;
using u32 = gwen::u32;
using i64 = gwen::i64;
using u64 = gwen::u64;
using gwen::cin;
using gwen::cout;

void solve() {
    auto x = std::make_tuple(std::make_tuple(1, 2));
    auto y = std::map<int, int>{{1, 1}, {2, 5}, {-1, 35}};
    auto z = std::vector<int>{3, 1, 4, 1, 5};
    DUMP(x, y, z);
}

int main() {
    int t = 1;
    // std::cin >> t;
    while (t--) solve();
    return 0;
}