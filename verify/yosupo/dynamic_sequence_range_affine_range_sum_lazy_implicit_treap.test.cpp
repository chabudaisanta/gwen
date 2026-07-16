#define PROBLEM "https://judge.yosupo.jp/problem/dynamic_sequence_range_affine_range_sum"

#include <iostream>

#include "gwen/alge/range_affine_range_sum_monoid.hpp"
#include "gwen/bbst/lazy_implicit_treap.hpp"
#include "gwen/types.hpp"

using namespace gwen;
using namespace std;

#define USE_ACL_MODINT 0
#if USE_ACL_MODINT
#include <atcoder/modint>
using mint = atcoder::modint998244353;
#else
#include "gwen/mod/modint.hpp"
using mint = gwen::DynamicModInt64;
auto gwen_dummy_setmod = []() {
    mint::set_mod(998244353);
    return 0;
}();
#endif
using Monoid = range_affine_range_sum_monoid<mint>;
using Treap = LazyImplicitTreap<Monoid>;

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    i32 N, Q;
    cin >> N >> Q;
    vector<Monoid::S> A(N, Monoid::e());
    for (i32 i = 0; i < N; ++i) {
        i32 a;
        cin >> a;
        A[i] = {a, 1};
    }
    Treap T(A);
    while (Q--) {
        i32 t;
        cin >> t;
        if (t == 0) {
            i32 p, x;
            cin >> p >> x;
            T.insert(p, {x, 1});
        }
        else if (t == 1) {
            i32 p;
            cin >> p;
            T.erase(p);
        }
        else if (t == 2) {
            i32 l, r;
            cin >> l >> r;
            T.reverse(l, r);
        }
        else if (t == 3) {
            i32 l, r, b, c;
            cin >> l >> r >> b >> c;
            T.apply(l, r, {b, c});
        }
        else {
            i32 l, r;
            cin >> l >> r;
            cout << T.prod(l, r).val.val() << '\n';
        }
    }

    return 0;
}
