#include "gwen/math/prime.hpp"
#include "gwen/my_template.hpp"

using namespace std;
using namespace gwen;

void solve() {
    i32 n;
    cin >> n;
    auto fac = factorize(n);
    rep(i, 0, (i32)fac.size()) {
        if (i) cout << ' ';
        cout << fac[i];
    }
    cout << '\n';
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t = 1;
    while (t--) solve();
    return 0;
}
