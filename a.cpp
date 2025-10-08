// # pragma GCC target("avx2")
#pragma GCC optimize("O2")
#pragma GCC optimize("unroll-loops")
#include <iostream>
#include <vector>

#include "gwen/types.hpp"
#include "gwen/dump.hpp"
#include "gwen/io.hpp"
using i32 = gwen::i32;
using u32 = gwen::u32;
using i64 = gwen::i64;
using u64 = gwen::u64;
using gwen::cin;
using gwen::cout;
#define rp(i,n)     for(i32 i = 0; i < (i32)(n); ++i)
constexpr char EL = '\n';
#define BAR std::cerr << "-------------------------\n"
#include "gwen/container/reversible_splay_tree.hpp"
#include "gwen/algebra/monoid.hpp"
#include <atcoder/modint>

using mint = atcoder::modint998244353;

struct Monoid {
    struct S {
        mint sum;
        i32 len;
        S() : sum(0), len(0) {}
        S(mint x) : sum(x), len(1) {}
        S(mint x, i32 l) : sum(x), len(l) {}
    };
    S e = S();
    S op(S a, S b) const {
        return S(a.sum + b.sum, a.len + b.len);
    }
};
struct Act {
    struct S {
        mint b, c;
        S() : b(1), c(0) {}
        S(mint b_, mint c_) : b(b_), c(c_) {}
    };
    S e = S();
    S op(S f, S g) const {
        return S(g.b * f.b, g.c * f.b + f.c);
    }
};
struct Mapping {
    using S = typename Monoid::S;
    using F = typename Act::S;
    S operator()(F f, S x) const {
        return S(f.c * x.len + f.b * x.sum, x.len);
    }
};
using AM = gwen::ActedMonoid<Monoid,Act,Mapping>;
using revsplaytree = gwen::reversible_splay_tree<AM>;
void solve() {
    revsplaytree::init(AM{});
    i32 N, Q; cin >> N >> Q;
    std::vector<Monoid::S> A(N);
    rp(i,N) {
        i32 a; cin >> a;
        A[i] = Monoid::S(mint(a));
    }
    revsplaytree T(A);
    while(Q--){
        i32 t; cin >> t;
        if(t==0) {
            i32 i, x; cin >> i >> x;
            T.insert_at(i, Monoid::S{mint(x)});
        }
        else if(t==1) {
            i32 i; cin >> i;
            T.erase_at(i);
        }
        else if(t==2) {
            i32 l, r; cin >> l >> r;
            T.reverse(l, r);
        }
        else if(t==3) {
            i32 l, r, b, c; cin >> l >> r >> b >> c;
            T.apply(l, r, Act::S(mint(b), mint(c)));
        }
        else {
            i32 l, r; cin >> l >> r;
            auto res = T.prod(l, r);
            cout << res.sum.val() << EL;
        }
    }
}

int main() {
    int t = 1;
    // std::cin >> t;
    while (t--) solve();
    std::cerr << "execution time: " << 1.0 * clock() / CLOCKS_PER_SEC * 1000
              << "(ms)\n";
    return 0;
}