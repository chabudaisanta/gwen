#define PROBLEM "https://judge.yosupo.jp/problem/range_affine_range_sum"

#include <iostream>

#include "gwen/alge/range_affine_range_sum_monoid.hpp"
#include "gwen/ds/lazy_segment_tree.hpp"
#include "gwen/mod/modint.hpp"

using namespace std;
using namespace gwen;
using mint = DynamicModInt64;
using Monoid = range_affine_range_sum_monoid<mint>;

int main() {
    mint::set_mod(998244353);
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, q;
    if (!(cin >> n >> q)) return 0;
    vector<Monoid::S> a(n);
    for (int i = 0; i < n; i++) {
        i64 v;
        cin >> v;
        a[i] = {mint(v), mint(1)};
    }

    LazySegmentTree<Monoid> seg(a);

    while (q--) {
        int t;
        cin >> t;
        if (t == 0) {
            int l, r;
            i64 b, c;
            cin >> l >> r >> b >> c;
            seg.apply(l, r, {mint(b), mint(c)});
        }
        else {
            int l, r;
            cin >> l >> r;
            cout << seg.prod(l, r).val.val() << "\n";
        }
    }
    return 0;
}
