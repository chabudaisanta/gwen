// # pragma GCC target("avx2")
#pragma GCC optimize("O2")
#pragma GCC optimize("unroll-loops")

#include <iostream>
#include <vector>

#include "gwen/algebra/basic_monoid.hpp"
#include "gwen/container/treap.hpp"
#include "gwen/dump.hpp"
#include "gwen/io.hpp"
using i32 = gwen::i32;
using u32 = gwen::u32;
using i64 = gwen::i64;
using u64 = gwen::u64;
using gwen::cin;
using gwen::cout;

template <typename T> struct MAPPING {
    T operator()(const T& a, const T& b) const { return a ^ b; }
};
template <typename T> struct or_monoid {
    using S = T;
    S e = 0;
    S op(const S& a, const S& b) const { return a | b; }
};
template <typename T> struct xor_monoid {
    using S = T;
    S e = 0;
    S op(const S& a, const S& b) const { return a ^ b; }
};
using AM = gwen::ActedMonoid<xor_monoid<int>, xor_monoid<int>, MAPPING<int>>;

// 合わない...
void solve() {
    int N, Q;
    cin >> N >> Q;
    std::vector<int> A(N, 0);
    gwen::treap<AM> T(AM{});
    auto t = T.build(A);
    while (Q--) {
        int l, r;
        cin >> l >> r;
        T.apply(t, l - 1, r, 1);
        // DUMP(T.to_vec(t));
    }
    auto v = T.to_vec(t);
    for (auto x : v) cout << (x ? '1' : '0');
    cout << '\n';
}

int main() {
    int t = 1;
    // std::cin >> t;
    while (t--) solve();
    std::cerr << "execution time: " << 1.0 * clock() / CLOCKS_PER_SEC * 1000
              << "(ms)\n";
    return 0;
}