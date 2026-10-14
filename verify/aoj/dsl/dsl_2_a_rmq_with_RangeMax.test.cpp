#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=DSL_2_A"

#include <iostream>
#include <limits>
#include <vector>

#include "gwen/query/range_max.hpp"
#include "gwen/types.hpp"

using namespace gwen;

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    i32 n, q;
    std::cin >> n >> q;

    RangeMax<i32> seg(std::vector<i32>(n, -std::numeric_limits<i32>::max()));
    while (q--) {
        i32 type, a, b;
        std::cin >> type >> a >> b;
        if (type == 0) {
            seg.set(a, -b);
        }
        else {
            std::cout << -seg.prod(a, b + 1) << '\n';
        }
    }
}
