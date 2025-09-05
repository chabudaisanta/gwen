// # pragma GCC target("avx2")
# pragma GCC optimize("O3")
# pragma GCC optimize("unroll-loops")

#include <iostream>
#include <vector>
#include "gwen/types.hpp"
#include "gwen/graph/doubling_tree.hpp"
#include "gwen/misc/io.hpp"

using i32 = gwen::i32;
using u32 = gwen::u32;
using i64 = gwen::i64;
using u64 = gwen::u64;
using gwen::cin;
using gwen::cout;

void solve() {
    int n;
    cin >> n;
    for(int i = 0; i < n; ++i) {
        u64 a, b;
        cin >> a >> b;
        cout << a + b << '\n';
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    int t = 1;
    // std::cin >> t;
    while(t--) solve();
    return 0;
}