#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/problems/DSL_2_A"

#include <iostream>
#include <string>
#include <algorithm>
#include <limits>

#include "gwen/ds/lazy_segment_tree.hpp"
#include "gwen/types.hpp"

using namespace std;
using namespace gwen;

struct Monoid {
    using S = i32;
    using F = i32;
    static S op(S a, S b) { return min(a, b); }
    static S e() { return numeric_limits<i32>::max(); }
    static S mapping(F, S x) { return x; }
    static F composition(F, F) { return 0; }
    static F id() { return 0; }
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    i32 N, Q; cin >> N >> Q;
    
    LazySegmentTree<Monoid> Seg(N);
    while(Q--) {
        i32 t, a, b; cin >> t >> a >> b;
        if(t == 0) {
            Seg.set(a, b);
        }
        else {
            b++;
            cout << Seg.prod(a, b) << '\n';
        }
    }

    return 0;
}
