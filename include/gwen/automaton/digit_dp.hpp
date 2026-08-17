#pragma once

#include <vector>
#include <utility>
#include <algorithm>

#include "gwen/types.hpp"
#include "gwen/alge/ring.hpp"
#include "gwen/automaton/automaton.hpp"

namespace gwen {

/**
 * @brief オートマトン上の桁 DP を実行する
 * @tparam T 遷移コストを表す型 (ring を満たすこと)
 * @tparam base N の進数
 * @param N 上限の値 (各桁の値が上位から順に格納されている配列)
 * @param a 条件を表すオートマトン
 * @return T 条件を満たす N 以下の値の総和 (オートマトンの遷移コストの積の和)
 */
template <ring T, i32 base>
T run_digit_dp(const std::vector<i32>& N, const Automaton<T, base>& a) {
    std::vector<T> dp_tight(a.n_states, T(0));
    std::vector<T> dp_loose(a.n_states, T(0));
    std::vector<T> next_tight(a.n_states, T(0));
    std::vector<T> next_loose(a.n_states, T(0));

    for (i32 s : a.init) {
        dp_tight[s] += T(1);
    }

    for (i32 x : N) {
        std::fill(next_tight.begin(), next_tight.end(), T(0));
        std::fill(next_loose.begin(), next_loose.end(), T(0));

        for (i32 u = 0; u < a.n_states; ++u) {
            const T loose_val = dp_loose[u];
            const T tight_val = dp_tight[u];

            if (loose_val == T(0) && tight_val == T(0)) {
                continue;
            }

            const auto* edges_u = &a.edges[u * base];

            if (loose_val != T(0)) {
                for (i32 c = 0; c < base; ++c) {
                    const auto& [v, cost] = edges_u[c];
                    next_loose[v] += loose_val * cost;
                }
            }

            if (tight_val != T(0)) {
                for (i32 c = 0; c < x; ++c) {
                    const auto& [v, cost] = edges_u[c];
                    next_loose[v] += tight_val * cost;
                }
                if (x < base) {
                    const auto& [v, cost] = edges_u[x];
                    next_tight[v] += tight_val * cost;
                }
            }
        }

        std::swap(dp_tight, next_tight);
        std::swap(dp_loose, next_loose);
    }

    T ans(0);
    for (i32 s : a.accept) {
        ans += dp_tight[s];
        ans += dp_loose[s];
    }
    return ans;
}

} // namespace gwen
