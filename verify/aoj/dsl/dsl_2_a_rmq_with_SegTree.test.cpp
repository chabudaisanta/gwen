#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=DSL_2_A"

#include <iostream>
#include <string>

#include "gwen/query/range_min.hpp"
#include "gwen/types.hpp"

using namespace std;
using namespace gwen;

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    i32 N, Q;
    cin >> N >> Q;

    RangeMin<i32> Seg(N);
    while (Q--) {
        i32 t, a, b;
        cin >> t >> a >> b;
        if (t == 0) {
            Seg.set(a, b);
        }
        else {
            b++;
            cout << Seg.prod(a, b) << '\n';
        }
    }

    return 0;
}
