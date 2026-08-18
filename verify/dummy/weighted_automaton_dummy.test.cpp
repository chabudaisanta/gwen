#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"

// clang-format off
#include "gwen/automaton/weighted_automaton.hpp"
// clang-format on
#include <iostream>

#include "gwen/types.hpp"
#include "gwen/alge/monoid.hpp"

using namespace gwen;

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int A, B;
    if (!(std::cin >> A >> B)) return 0;

    // 重み付きオートマトンモジュールが正常にコンパイルされることを確認
    using M = weighted_automaton_monoid<2, sum_monoid<i32>>;
    auto e = M::e();
    auto prod = M::op(e, e);
    
    std::vector<WeightedAutomaton<2, sum_monoid<i32>>> automata = {e, e};
    auto all = build_weighted_automaton(automata);

    std::cout << A + B << '\n';
    return 0;
}
