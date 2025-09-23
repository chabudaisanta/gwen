// # pragma GCC target("avx2")
#pragma GCC optimize("O2")
#pragma GCC optimize("unroll-loops")

#include <iostream>
#include <vector>

#include <atcoder/modint>   // static_modintがまだないため
#include "gwen/algebra/basic_monoid.hpp"
#include "gwen/container/sorted_treap.hpp"
#include "gwen/dump.hpp"
#include "gwen/io.hpp"
using i32 = gwen::i32;
using u32 = gwen::u32;
using i64 = gwen::i64;
using u64 = gwen::u64;
using gwen::cin;
using gwen::cout;

using M = gwen::sum_monoid<int>;
using Compare = std::less<int>;
void solve() {
    gwen::sorted_treap<M,Compare> T(M{});
    auto arr = T.build();
    for(int i = 0; i < 10; ++i) {
        int a = gwen::rand32(100);
        DUMP(a);
        T.insert(arr, a);
        DUMP(T.to_vec(arr));
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