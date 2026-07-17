#define PROBLEM "https://judge.yosupo.jp/problem/primality_test"
#include <iostream>

#include "gwen/io/yesno.hpp"
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
        i64 n;
        cin >> n;
        yon(is_prime(n));
    }
    return 0;
}
