#define PROBLEM "https://judge.yosupo.jp/problem/queue_operate_all_composite"

#include <iostream>

#include "gwen/mod/modint.hpp"
#include "gwen/query/swag.hpp"
#include "gwen/types.hpp"

using namespace gwen;

using mint = DynamicModInt64;

// Affine Monoid for f(x) = ax + b
// f(g(x)) = a(cx + d) + b = acx + ad + b
// So f \circ g = (ac, ad + b)
// The problem asks for f_k(f_{k-1}(...f_1(x)))
// This is exactly the product f_k * f_{k-1} * ... * f_1 in our Monoid.
// So, the order of application is right to left if we consider it as functions,
// but SWAG folds from front to back of the deque (i.e. f_1 * f_2 * ... * f_k).
// If we want fold() to compute f_k \circ ... \circ f_1,
// op(f_left, f_right) must be f_right \circ f_left.
// op((a_1, b_1), (a_2, b_2)) = (a_2 * a_1, a_2 * b_1 + b_2)
struct AffineMonoid {
    struct S {
        mint a, b;
    };
    static S op(const S& l, const S& r) { return {r.a * l.a, r.a * l.b + r.b}; }
    static S e() { return {mint(1), mint(0)}; }
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    mint::set_mod(998244353);

    i32 q;
    if (!(std::cin >> q)) return 0;

    SwagDeque<AffineMonoid> swag;

    for (i32 i = 0; i < q; ++i) {
        i32 type;
        std::cin >> type;
        if (type == 0) {
            i64 a, b;
            std::cin >> a >> b;
            swag.push_back({mint(a), mint(b)});
        }
        else if (type == 1) {
            swag.pop_front();
        }
        else if (type == 2) {
            i64 x;
            std::cin >> x;
            auto f = swag.fold();
            mint ans = f.a * mint(x) + f.b;
            std::cout << ans.val() << "\n";
        }
    }

    return 0;
}
