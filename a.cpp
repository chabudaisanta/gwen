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
#include "gwen/container/splay_tree.hpp"
#include "gwen/algebra/monoid.hpp"
struct MAPPING {
    using S = i64;
    using F = i64;
    S operator()(const F& f, const S& x) const {
        return f + x;
    }
};
using AM = gwen::ActedMonoid<gwen::Max<i64>,gwen::Sum<i64>,MAPPING>;
using splaytree = gwen::splay_tree<AM>;
void solve() {
    splaytree::init(AM{gwen::Max<i64>{}, gwen::Sum<i64>{0},MAPPING{}});
}

int main() {
    int t = 1;
    // std::cin >> t;
    while (t--) solve();
    std::cerr << "execution time: " << 1.0 * clock() / CLOCKS_PER_SEC * 1000
              << "(ms)\n";
    return 0;
}