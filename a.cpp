// # pragma GCC target("avx2")
#pragma GCC optimize("O2")
#pragma GCC optimize("unroll-loops")
#include <iostream>
#include <vector>

#include "gwen/dump.hpp"
#include "gwen/io.hpp"
using i32 = gwen::i32;
using u32 = gwen::u32;
using i64 = gwen::i64;
using u64 = gwen::u64;
using gwen::cin;
using gwen::cout;

#include "gwen/query/mo.hpp"
template <std::integral T> constexpr T choose2(T x) { return x * (x - 1) / 2; }

void solve() {
    std::vector<int> d{1, 2, 3};
}

int main() {
    int t = 1;
    // std::cin >> t;
    while (t--) solve();
    std::cerr << "execution time: " << 1.0 * clock() / CLOCKS_PER_SEC * 1000
              << "(ms)\n";
    return 0;
}