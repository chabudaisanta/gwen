#ifdef LOCAL
#else
#define NDEBUG
#endif
// # pragma GCC target("avx2")
#pragma GCC optimize("O2")
#pragma GCC optimize("unroll-loops")
#include <iostream>
#include <vector>
#include <ctime>

#include "gwen/dump.hpp"
#include "gwen/io.hpp"
#include "gwen/types.hpp"
#include "gwen/container/hash_map.hpp"
using i32 = gwen::i32;
using u32 = gwen::u32;
using i64 = gwen::i64;
using u64 = gwen::u64;
using gwen::input;
using gwen::output;
#define rp(i, n) for (i32 i = 0; i < (i32)(n); ++i)
constexpr char EL = '\n';
#define BAR std::cerr << "-------------------------\n"
#include <unordered_map>
void solve() {
    std::unordered_map<u64,u64> MP(2e6);
    i32 Q; input >> Q;
    while(Q--) {
        i32 t; input >> t;
        if(t == 0) {
            u64 k, v; input >> k >> v;
            MP[k] = v;
        }
        else {
            u64 k; input >> k;
            output << MP[k] << EL;
        }
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