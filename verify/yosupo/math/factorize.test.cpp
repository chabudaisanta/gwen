#define PROBLEM "https://judge.yosupo.jp/problem/factorize"
#include <iostream>

#include "gwen/math/prime.hpp"
#include "gwen/types.hpp"

using namespace gwen;
using namespace std;

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    i32 Q;
    cin >> Q;
    while (Q--) {
        i64 x;
        cin >> x;
        auto p = factorize(x);
        cout << p.size() << (p.size() > 0 ? ' ' : '\n');
        for (i32 sz = p.size(), cnt = 0; i64 y : p) cout << y << (++cnt < sz ? ' ' : '\n');
    }
    return 0;
}
