# pragma GCC target("avx2")
# pragma GCC optimize("O3")
# pragma GCC optimize("unroll-loops")

#include <iostream>
#include <gwen/types.hpp>
#include <gwen/misc/io.hpp>
using gwen::cin;
using i32 = gwen::i32;
void solve() {
    i32 x;
    cin >> x;
    std::cout << x << '\n';
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    int t = 1;
    // std::cin >> t;
    while(t--) solve();
    return 0;
}