#define PROBLEM "https://yukicoder.me/problems/no/599"

#include <iostream>
#include <vector>

#include "gwen/types.hpp"
#include "gwen/hash/rolling_hash.hpp"
#include "gwen/mod/modint.hpp"
#include "gwen/core/constants.hpp"
#include "gwen/dump.hpp"

using namespace std;
using namespace gwen;

int main() {
    using mint = DynamicModInt64;
    mint::set_mod(mod107);
    string S; cin >> S;
    const i32 N = S.size();
    const i32 K = N / 2;

    auto rh = RollingHash(S);
    mint ans = 0;
    vector<mint> dp(K + 1);
    dp[0] = 1;
    for(i32 i = 0; i <= K; ++i) {
        ans += dp[i];
        for(i32 j = i + 1; j <= K; ++j) {
            bool ok = rh.equal(i, j, N-j, N-i);
            if(ok) dp[j] += dp[i];
        }
    }
    cout << ans.val();

    return 0;
}