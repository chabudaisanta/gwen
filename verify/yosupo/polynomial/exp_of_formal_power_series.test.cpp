#define PROBLEM "https://judge.yosupo.jp/problem/exp_of_formal_power_series"

#include <iostream>
#include <vector>

#include "gwen/math/formal_power_series.hpp"
#include "gwen/mod/modint.hpp"
#include "gwen/types.hpp"

using namespace gwen;

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    i32 n;
    std::cin >> n;
    std::vector<modint998244353> coefficients(static_cast<usize>(n));
    for (auto& coefficient : coefficients) {
        i64 value;
        std::cin >> value;
        coefficient = modint998244353(value);
    }

    const auto result = FormalPowerSeries<modint998244353>(std::move(coefficients)).exp(n);
    for (i32 i = 0; i < n; ++i) {
        std::cout << result[i].val() << (i + 1 == n ? '\n' : ' ');
    }
}
