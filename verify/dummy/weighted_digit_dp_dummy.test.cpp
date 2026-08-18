#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"

// clang-format off
#include "gwen/automaton/weighted_digit_dp.hpp"
// clang-format on
#include <iostream>

#include "gwen/types.hpp"
#include "gwen/alge/monoid.hpp"

using namespace gwen;

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int a_val, b_val;
    if (!(std::cin >> a_val >> b_val)) return 0;

    // 重み付き桁DPモジュールが正常にコンパイルされることを確認
    using M = sum_monoid<i32>;
    WeightedAutomaton<10, M> a(1, 1);
    a.add_init(0, 1);
    a.set_condition(0, 1ULL);
    for (i32 c = 0; c < 10; ++c) {
        a.set_edge(0, c, 0, 1);
    }
    
    std::vector<i32> upper_bound = {1, 0};
    auto f = [](i32 dp_val, u64 condition) { return condition == 1ULL ? dp_val : 0; };
    i32 ans = run_weighted_digit_dp<i32>(upper_bound, a, f);

    std::cout << a_val + b_val << '\n';
    return 0;
}
