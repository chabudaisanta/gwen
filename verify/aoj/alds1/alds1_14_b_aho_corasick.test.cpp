#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ALDS1_14_B&lang=jp"

#include <iostream>
#include <string>
#include <vector>

#include "gwen/automaton/aho_corasick.hpp"
#include "gwen/types.hpp"

int main() {
    using namespace gwen;
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::string text, pattern;
    std::cin >> text >> pattern;
    std::vector<std::vector<i32>> patterns(1);
    for (unsigned char c : pattern) patterns[0].push_back(c);
    const auto a = automaton::aho_corasick<128>(patterns);
    std::vector<bool> accepting(a.n);
    for (i32 state : a.accept) accepting[state] = true;

    i32 state = a.init.front();
    for (i32 i = 0; i < static_cast<i32>(text.size()); ++i) {
        state = a.edge(state, static_cast<unsigned char>(text[i]));
        if (accepting[state]) std::cout << i - static_cast<i32>(pattern.size()) + 1 << '\n';
    }
}
