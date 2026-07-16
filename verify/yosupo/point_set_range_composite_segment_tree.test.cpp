#define PROBLEM "https://judge.yosupo.jp/problem/point_set_range_composite"

#include <iostream>
#include "gwen/ds/segment_tree.hpp"
#include "gwen/alge/monoid.hpp"
#include "gwen/mod/modint.hpp"

using namespace std;
using namespace gwen;
using mint = DynamicModInt64;

struct affine_monoid_mint {
    struct S {
        mint a, b;
    };
    static constexpr S op(S f, S g) { return {f.a * g.a, g.a * f.b + g.b}; }
    static constexpr S e() { return {1, 0}; }
};

int main() {
    mint::set_mod(998244353);
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, q;
    if (!(cin >> n >> q)) return 0;
    vector<affine_monoid_mint::S> a(n);
    for (int i = 0; i < n; i++) {
        i64 c, d;
        cin >> c >> d;
        a[i] = {mint(c), mint(d)};
    }

    SegmentTree<affine_monoid_mint> seg(a);

    while (q--) {
        int t;
        cin >> t;
        if (t == 0) {
            int p;
            i64 c, d;
            cin >> p >> c >> d;
            seg.set(p, {mint(c), mint(d)});
        } else {
            int l, r;
            i64 x;
            cin >> l >> r >> x;
            auto f = seg.prod(l, r);
            cout << (f.a * mint(x) + f.b).val() << "\n";
        }
    }
    return 0;
}
