// # pragma GCC target("avx2")
#pragma GCC optimize("O2")
#pragma GCC optimize("unroll-loops")
#include <iostream>
#include <vector>

#include "gwen/misc/xorshift.hpp"
#include "gwen/dump.hpp"
#include "gwen/io.hpp"
using i32 = gwen::i32;
using u32 = gwen::u32;
using i64 = gwen::i64;
using u64 = gwen::u64;
using gwen::cin;
using gwen::cout;

#include "gwen/algebra/basic_monoid.hpp"
#include "gwen/container/sorted_treap.hpp"
#include "gwen/container/mergesort_tree.hpp"
using M = gwen::sum_monoid<i64>;
using COMP = std::less<i64>;
using MULTISET = gwen::sorted_treap<M, COMP>;
void solve() {
    i32 N; cin >> N;
    std::vector<i64> A(N); cin >> A;
    gwen::mergesort_tree<M> MT(A);

    i32 Q; cin >> Q;
    u64 ans = 0;
    while(Q--) {
        u64 al, bt, gm; cin >> al >> bt >> gm;
        al ^= ans;
        bt ^= ans;
        gm ^= ans;
        DUMP(al-1, bt, gm);
        ans = MT.upper_prod(al-1, bt, gm);
        DUMP(ans);
        cout << ans << '\n';
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

/*
*/