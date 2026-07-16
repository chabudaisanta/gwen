#define PROBLEM "https://judge.yosupo.jp/problem/point_add_range_sum"

#include <iostream>
#include "gwen/ds/segment_tree.hpp"
#include "gwen/alge/monoid.hpp"

using namespace std;
using namespace gwen;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, q;
    if (!(cin >> n >> q)) return 0;
    vector<i64> a(n);
    for (int i = 0; i < n; i++) cin >> a[i];

    SegmentTree<sum_monoid<i64>> seg(a);

    while (q--) {
        int t;
        cin >> t;
        if (t == 0) {
            int p;
            i64 x;
            cin >> p >> x;
            seg.set(p, seg.get(p) + x);
        } else {
            int l, r;
            cin >> l >> r;
            cout << seg.prod(l, r) << "\n";
        }
    }
    return 0;
}
