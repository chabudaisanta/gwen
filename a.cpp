// # pragma GCC target("avx2")
#pragma GCC optimize("O2")
#pragma GCC optimize("unroll-loops")

#include <atcoder/modint>  // static_modintがまだないため
#include <iostream>
#include <vector>

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
    gwen::sorted_treap<M, Compare> T(M{});
    auto arr = T.build();
    for (int i = 0; i < 10; ++i) {
        int a = gwen::rand32(1, 20);
        T.insert(arr, a);
        DUMP(a);
        int p = gwen::rand32(0, T.size(arr));
        DUMP(p, T.at(arr, p));
        int l = gwen::rand32(0, T.size(arr));
        int r = gwen::rand32(l, T.size(arr));
        DUMP(l, r, T.size(arr));
        DUMP(T.prod(arr, l, r));
        DUMP(T.to_vec(arr));
        int x = gwen::rand32(1, 20);
        DUMP(T.lower_bound(arr, x));
        DUMP(T.upper_bound(arr, x));
        DUMP(T.count(arr, x));
        DUMP(T.contains(arr, x));
        std::cerr << "-----------------\n";
    }
}
/*
p, T.at(arr, p): 0, 15
l, r, T.size(arr): 0, 0, 1
T.prod(arr, l, r): 0
T.to_vec(arr): [15]
T.lower_bound(arr, x): (0, 15)
T.upper_bound(arr, x): (0, 15)
T.count(arr, x): 0
T.contains(arr, x): false
-----------------
p, T.at(arr, p): 1, 15
l, r, T.size(arr): 0, 0, 2
T.prod(arr, l, r): 0
T.to_vec(arr): [15, 15]
T.lower_bound(arr, x): (0, 15)
T.upper_bound(arr, x): (0, 15)
T.count(arr, x): 0
T.contains(arr, x): false
-----------------
p, T.at(arr, p): 2, 15
l, r, T.size(arr): 2, 2, 3
T.prod(arr, l, r): 0
T.to_vec(arr): [9, 15, 15]
T.lower_bound(arr, x): (0, 9)
T.upper_bound(arr, x): (1, 15)
T.count(arr, x): 1
T.contains(arr, x): true
-----------------
p, T.at(arr, p): 3, 19
l, r, T.size(arr): 0, 3, 4
T.prod(arr, l, r): 39
T.to_vec(arr): [9, 15, 15, 19]
T.lower_bound(arr, x): (3, 19)
T.upper_bound(arr, x): (3, 19)
T.count(arr, x): 0
T.contains(arr, x): false
-----------------
p, T.at(arr, p): 2, 15
l, r, T.size(arr): 3, 4, 5
T.prod(arr, l, r): 15
T.to_vec(arr): [4, 9, 15, 15, 19]
T.lower_bound(arr, x): (4, 19)
T.upper_bound(arr, x): (5, 0)
T.count(arr, x): 1
T.contains(arr, x): true
-----------------
p, T.at(arr, p): 1, 9
l, r, T.size(arr): 5, 5, 6
T.prod(arr, l, r): 0
T.to_vec(arr): [4, 9, 13, 15, 15, 19]
T.lower_bound(arr, x): (1, 9)
T.upper_bound(arr, x): (2, 13)
T.count(arr, x): 1
T.contains(arr, x): true
-----------------
p, T.at(arr, p): 3, 13
l, r, T.size(arr): 5, 6, 7
T.prod(arr, l, r): 15
T.to_vec(arr): [4, 9, 9, 13, 15, 15, 19]
T.lower_bound(arr, x): (6, 19)
T.upper_bound(arr, x): (6, 19)
T.count(arr, x): 0
T.contains(arr, x): false
-----------------
p, T.at(arr, p): 3, 13
l, r, T.size(arr): 7, 7, 8
T.prod(arr, l, r): 0
T.to_vec(arr): [4, 9, 9, 13, 14, 15, 15, 19]
T.lower_bound(arr, x): (0, 4)
T.upper_bound(arr, x): (1, 9)
T.count(arr, x): 1
T.contains(arr, x): true
-----------------
p, T.at(arr, p): 4, 13
l, r, T.size(arr): 2, 4, 9
T.prod(arr, l, r): 18
T.to_vec(arr): [4, 7, 9, 9, 13, 14, 15, 15, 19]
T.lower_bound(arr, x): (2, 9)
T.upper_bound(arr, x): (4, 13)
T.count(arr, x): 2
T.contains(arr, x): true
-----------------
p, T.at(arr, p): 4, 9
l, r, T.size(arr): 9, 9, 10
T.prod(arr, l, r): 0
T.to_vec(arr): [4, 7, 9, 9, 9, 13, 14, 15, 15, 19]
T.lower_bound(arr, x): (5, 13)
T.upper_bound(arr, x): (5, 13)
T.count(arr, x): 0
T.contains(arr, x): false
-----------------
execution time: 1.475(ms)
*/

int main() {
    int t = 1;
    // std::cin >> t;
    while (t--) solve();
    std::cerr << "execution time: " << 1.0 * clock() / CLOCKS_PER_SEC * 1000
              << "(ms)\n";
    return 0;
}