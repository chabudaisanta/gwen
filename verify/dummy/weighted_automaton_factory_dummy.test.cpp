#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include <iostream>
#include <vector>
#include "gwen/automaton/weighted_automaton_factory.hpp"
#include "gwen/alge/monoid.hpp"
#include "gwen/types.hpp"

using namespace gwen;

int main() {
    using M = sum_monoid<i32>;
    auto a = automaton::used_digits_count_exact<10, M>(2);
    auto b = automaton::used_digits_count_leq<10, M>(3);
    
    std::vector<i32> pattern = {1, 2, 3};
    auto c = automaton::contains_pattern<10, M>(pattern);
    
    std::cout << "Hello World\n";
    return 0;
}
