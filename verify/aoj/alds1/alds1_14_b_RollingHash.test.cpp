#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ALDS1_14_B&lang=jp"

#include <iostream>

#include "gwen/types.hpp"
#include "gwen/hash/rolling_hash.hpp"

using namespace std;
using namespace gwen;

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    string T, P; cin >> T >> P;
    const i32 N = T.size(), M = P.size();

    auto rh = RollingHash(T + "#" + P);
    for(i32 i = 0; i < N - M + 1; ++i) {
        if (rh.equal(i, i + M, N + 1, N + 1 + M)) {
            cout << i << '\n';
        }
    }

    return 0;
}
