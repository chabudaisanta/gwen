// # pragma GCC target("avx2")
#pragma GCC optimize("O2")
#pragma GCC optimize("unroll-loops")

#include <iostream>
#include <vector>

#include "gwen/dump.hpp"
#include "gwen/io.hpp"
#include "gwen/math/prime.hpp"
#include "gwen/mod/modint.hpp"
using i32 = gwen::i32;
using u32 = gwen::u32;
using i64 = gwen::i64;
using u64 = gwen::u64;
using gwen::input;
using gwen::output;
using mint = gwen::dynamic_modint64;

void solve() {
    i32 Q;
    input >> Q;
    while (Q--) {
        u64 n;
        input >> n;
        output << (gwen::miller(n) ? "Yes\n" : "No\n");
    }
}

int main() {
    int t = 1;
    // std::cin >> t;
    while (t--) solve();
    return 0;
}
