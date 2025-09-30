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
using MULTISET = gwen::sorted_treap<M, Compare>;
void solve() {
    MULTISET::init(M{}, Compare{});
    MULTISET ms;
    int N;
    cin >> N;
    std::vector<int> A(N);
    cin >> A;
    for (int a : A) {
        ms.insert(a);
        for (int j = 0; j < ms.size(); ++j) {
            std::cerr << (j == 0 ? "" : " ") << ms.at(j);
        }
        std::cerr << '\n';
    }

    int M;
    cin >> M;
    std::vector<int> B(M);
    cin >> B;
    for (int b : B) {
        ms.erase(b);
        for (int j = 0; j < ms.size(); ++j) {
            std::cerr << (j == 0 ? "" : " ") << ms.at(j);
        }
        std::cerr << '\n';
        DUMP(ms.prod(0, 2));
        DUMP(ms.all_prod());
    }
}

/*
5
5 2 4 3 1
3
1 5 3

5
2 5
2 4 5
2 3 4 5
1 2 3 4 5
2 3 4 5
ms.prod(0, 2): 5
ms.all_prod(): 14
2 3 4
ms.prod(0, 2): 5
ms.all_prod(): 9
2 4
ms.prod(0, 2): 6
ms.all_prod(): 6
execution time: 1.261(ms)
*/

int main() {
    int t = 1;
    // std::cin >> t;
    while (t--) solve();
    std::cerr << "execution time: " << 1.0 * clock() / CLOCKS_PER_SEC * 1000
              << "(ms)\n";
    return 0;
}