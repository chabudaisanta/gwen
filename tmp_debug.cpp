#include "gwen/automaton/weighted_digit_dp.hpp"
#include "gwen/alge/monoid.hpp"
#include <iostream>
using namespace gwen;
int main() {
    std::vector<i32> N = {3, 0};
    using M = sum_monoid<i32>;
    WeightedAutomaton<10, M> a(2, 1);
    a.add_init(0, 1);
    a.set_condition(0, 0ULL);
    a.set_condition(1, 1ULL);
    for (i32 c = 0; c < 10; ++c) {
        a.set_edge(0, c, (c == 3) ? 1 : 0, 1);
        a.set_edge(1, c, 1, 1);
    }
    WeightedAutomaton<10, M> b(3, 1);
    b.add_init(0, 1);
    for (i32 i = 0; i < 3; ++i) {
        b.set_condition(i, (i == 0) ? 1ULL : 0ULL);
        for (i32 c = 0; c < 10; ++c) {
            b.set_edge(i, c, (i + c) % 3, 1);
        }
    }
    auto ab = a * b;
    auto f = [](i32 dp_val, u64 condition) {
        std::cout << "condition: " << condition << " val: " << dp_val << std::endl;
        return condition == 0b11ULL ? dp_val : 0;
    };
    i32 ans = run_weighted_digit_dp<i32>(N, ab, f);
    std::cout << "ans: " << ans << std::endl;
}
