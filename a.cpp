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
template <std::integral T>
constexpr T choose2(T x) { return x * (x - 1) / 2; }

void solve() {
    i32 N, Q;
    cin >> N >> Q;
    std::vector<i64> A(N), B(N);
    cin >> A >> B;

    gwen::mo_algorithm mo(N);
    while (Q--) {
        i32 x, y;
        cin >> x >> y;
        // i < x, j < y
        mo.add_query(x, y);
    }

    auto f0 = [&](i32 x, i32 y) {

    };
    auto f1 = [&](i32 l, i32) {

    };
    auto f2 = [&](i32, i32 r) {

    };
    auto f3 = [&](i32, i32 r) {

    };
    auto get_res = [&]() {
    };
    auto res = mo.solve(f0, f1, f2, f3, get_res);
    for (auto x : res) {
        cout << x << '\n';
    }
}

int main() {
    int t = 1;
    // std::cin >> t;
    while (t--) solve();
    std::cerr << "execution time: " << 1.0 * clock() / CLOCKS_PER_SEC * 1000
              << "(ms)\n";
    return 0;
}