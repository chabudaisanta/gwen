#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=DPL_1_D"

#include <iostream>
#include <vector>

#include "gwen/algo/lis.hpp"
#include "gwen/types.hpp"

using namespace gwen;

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    i32 n;
    if (!(std::cin >> n)) return 0;

    std::vector<i32> a(n);
    for (i32 i = 0; i < n; ++i) {
        std::cin >> a[i];
    }

    auto ans = lis_strict(a);
    std::cout << ans.size() << "\n";

    return 0;
}
