#include "gwen/my_template.hpp"

using namespace std;
using namespace gwen;

void solve() {
    i32 n;
    cin >> n;
    
    vector<i32> fac;
    for(i32 i = 2; (i64)i * i <= n; ++i) {
        while(n % i == 0) {
            fac.emplace_back(i);
            n /= i;
        }
    }
    if (n > 1) fac.emplace_back(n);

    for(i32 i = 0; i < (i32)fac.size(); ++i) {
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
