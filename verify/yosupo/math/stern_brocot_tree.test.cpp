#define PROBLEM "https://judge.yosupo.jp/problem/stern_brocot_tree"

#include <iostream>
#include <string>
#include <vector>

#include "gwen/math/stern_brocot_tree.hpp"

int main() {
    using Tree = gwen::SternBrocotTree<gwen::i64>;

    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    gwen::i32 t;
    std::cin >> t;
    while (t--) {
        std::string type;
        std::cin >> type;
        if (type == "ENCODE_PATH") {
            gwen::i64 p, q;
            std::cin >> p >> q;
            const auto path = Tree::encode_path(p, q);
            std::cout << path.size();
            for (const auto& [direction, count] : path) std::cout << ' ' << direction << ' ' << count;
            std::cout << '\n';
        } else if (type == "DECODE_PATH") {
            gwen::i32 k;
            std::cin >> k;
            std::vector<std::pair<char, gwen::i64>> path(k);
            for (auto& [direction, count] : path) std::cin >> direction >> count;
            const auto [p, q] = Tree::decode_path(path);
            std::cout << p << ' ' << q << '\n';
        } else if (type == "LCA") {
            gwen::i64 p, q, r, s;
            std::cin >> p >> q >> r >> s;
            const auto [a, b] = Tree::lca(p, q, r, s);
            std::cout << a << ' ' << b << '\n';
        } else if (type == "ANCESTOR") {
            gwen::i64 k, p, q;
            std::cin >> k >> p >> q;
            const auto [a, b] = Tree::ancestor(k, p, q);
            if (a == 0) {
                std::cout << -1 << '\n';
            } else {
                std::cout << a << ' ' << b << '\n';
            }
        } else if (type == "RANGE") {
            gwen::i64 p, q;
            std::cin >> p >> q;
            const auto [a, b, c, d] = Tree::range(p, q);
            std::cout << a << ' ' << b << ' ' << c << ' ' << d << '\n';
        }
    }
}
