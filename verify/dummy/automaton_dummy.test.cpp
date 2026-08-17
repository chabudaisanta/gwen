#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"

// clang-format off
#include "gwen/automaton/automaton.hpp"
// clang-format on
#include <iostream>
#include "gwen/types.hpp"

using namespace gwen;

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int A, B;
    if (!(std::cin >> A >> B)) return 0;
    
    // オートマトンモジュールが正常にコンパイルされることを確認
    using M = automaton_monoid<i32, 2>;
    auto e = M::e();
    auto prod = M::op(e, e);
    
    std::cout << A + B << '\n';
    return 0;
}
