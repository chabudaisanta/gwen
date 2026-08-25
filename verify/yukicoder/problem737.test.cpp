#define PROBLEM "https://yukicoder.me/problems/no/737"

#include <algorithm>
#include <iostream>
#include <vector>

#include "gwen/automaton/factory.hpp"
#include "gwen/automaton/weighted_digit_dp.hpp"
#include "gwen/types.hpp"

#define USE_ACL_MODINT 0
#if USE_ACL_MODINT
#include <atcoder/modint>
using mint = atcoder::modint1000000007;
#else
#include "gwen/mod/modint.hpp"
using mint = gwen::DynamicModInt64;
auto gwen_dummy_setmod = []() {
    mint::set_mod(1000000007);
    return 0;
}();
#endif

using namespace gwen;

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    u64 n;
    std::cin >> n;

    std::vector<i32> upper_bound;
    while (n != 0) {
        upper_bound.push_back(static_cast<i32>(n & 1ULL));
        n >>= 1;
    }
    std::reverse(upper_bound.begin(), upper_bound.end());

    mint ans = 0;
    const i32 digit_count = static_cast<i32>(upper_bound.size());
    for (i32 k = 1; k <= digit_count; ++k) {
        const auto a = automaton::non_zero_count_exact<2>(k);
        ans += mint(k) * run_digit_dp_sum<mint>(upper_bound, a);
    }

    std::cout << ans.val() << '\n';
    return 0;
}
