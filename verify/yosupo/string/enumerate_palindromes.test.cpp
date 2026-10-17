#define PROBLEM "https://judge.yosupo.jp/problem/enumerate_palindromes"

#include <iostream>
#include <string>

#include "gwen/algo/manacher.hpp"
#include "gwen/types.hpp"

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::string s;
    std::cin >> s;
    const auto [odd, even] = gwen::manacher(s);
    for (gwen::i32 i = 0; i < static_cast<gwen::i32>(s.size()); ++i) {
        if (i) std::cout << ' ' << 2 * even[i];
        std::cout << (i ? " " : "") << 2 * odd[i] - 1;
    }
    std::cout << '\n';
}
