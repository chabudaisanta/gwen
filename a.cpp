// # pragma GCC target("avx2")
# pragma GCC optimize("O3")
# pragma GCC optimize("unroll-loops")

#include <iostream>
#include <vector>
#include "gwen/types.hpp"
#include "gwen/graph/doubling_tree.hpp"
#include "gwen/io.hpp"
#include "gwen/dump.hpp"

using i32 = gwen::i32;
using u32 = gwen::u32;
using i64 = gwen::i64;
using u64 = gwen::u64;
using gwen::cin;
using gwen::cout;

#include <set>
void solve() {
    // auto x = std::make_tuple(3, 1, 4, 1, 5, 9, 2);
    // auto x = "nekomugi tororo";
    // auto x = std::make_pair("nekomugi tororo", 3939ll);
    // auto x = std::deque<int>{3,1,4,1,5,9,2};
    auto x = std::map<int,int>{std::make_pair(1, 5), std::make_pair(10, -1), std::make_pair(-100, 3939)};
    dump(x);
}

int main() {
    int t = 1;
    // std::cin >> t;
    while(t--) solve();
    return 0;
}