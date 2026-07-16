#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ALDS1_4_C"

#include <iostream>
#include <string>

#include "gwen/ds/trie.hpp"
#include "gwen/types.hpp"

using namespace gwen;

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    i32 n;
    if (!(std::cin >> n)) return 0;

    UpperAlphaTrie trie;
    for (i32 i = 0; i < n; ++i) {
        std::string op, str;
        std::cin >> op >> str;
        if (op == "insert") {
            trie.insert(str);
        } else if (op == "find") {
            if (trie.contains(str)) {
                std::cout << "yes\n";
            } else {
                std::cout << "no\n";
            }
        }
    }

    return 0;
}
