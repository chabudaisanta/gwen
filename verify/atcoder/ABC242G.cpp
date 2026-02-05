// #pragma GCC target("avx2")
#pragma GCC optimize("Ofast")
#pragma GCC optimize("unroll-loops")
#include "gwen/my_template.hpp"
#include "gwen/query/mo.hpp"
using namespace std;

void solve() {
    i32 N; cin >> N;
    vector<i32> A(N); cin >> A;
    vector<i32> cnt(1.1e6);
    i32 ans = 0;
    auto Mo = gwen::mo_algorithm(N);
    i32 Q; cin >> Q;
    rp(i,Q) {
        i32 l, r; cin >> l >> r; --l;
        Mo.add_query(l, r);
    }
    auto il = [&](i32 l, i32) {
        cnt[A[l]] ^= 1;
        if(cnt[A[l]]) ans--;
    };
    auto dl = [&](i32 l, i32) {
        if(cnt[A[l]]) ans++;
        cnt[A[l]] ^= 1;
    };
    auto ir = [&](i32, i32 r) {
        if(cnt[A[r]]) ans++;
        cnt[A[r]] ^= 1;
    };
    auto dr = [&](i32, i32 r) {
        cnt[A[r]] ^= 1;
        if(cnt[A[r]]) ans--;
    };
    auto get_res = [&](i32) { return ans; };
    auto R = Mo.solve(il, dl, ir, dr, get_res);
    for(auto r : R) cout << r << EL;
}

int main(void) {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    std::cout << std::fixed << std::setprecision(16);

    int t = 1;
    // cin >> t;
    while (t--) solve();

    std::cerr << "execution time: " << 1.0 * clock() / CLOCKS_PER_SEC * 1000 << "(ms)" << EL;
    return 0;
}