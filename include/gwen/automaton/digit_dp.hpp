#pragma once

#include <algorithm>
#include <cassert>
#include <concepts>
#include <utility>
#include <vector>

#include "gwen/automaton/automaton.hpp"
#include "gwen/types.hpp"

namespace gwen {

/**
 * @brief 桁 DP の値型に必要な操作を表す Concept
 * @tparam T DP値型
 */
template <typename T>
concept digit_dp_value = std::copy_constructible<T> && requires(T value, const T other) {
    { T(0) } -> std::same_as<T>;
    { T(1) } -> std::same_as<T>;
    { value = other } -> std::same_as<T&>;
    { value += other } -> std::same_as<T&>;
    { value == other } -> std::convertible_to<bool>;
};

/**
 * @brief オートマトン上の桁 DP を実行する
 * @tparam T DP値型 (`digit_dp_value` を満たすこと)
 * @tparam base N の進数
 * @param N 上限の値 (各桁の値が上位から順に格納されている配列)
 * @param a 条件を表すオートマトン
 * @return T 条件を満たす N 以下の値の総和 (オートマトンの遷移コストの積の和)
 */
template <digit_dp_value T, i32 base> T run_digit_dp(const std::vector<i32>& N, const Automaton<base>& a) {
    assert(a.valid());
    std::vector<T> dp_tight(a.n, T(0));
    std::vector<T> dp_loose(a.n, T(0));
    std::vector<T> next_tight(a.n, T(0));
    std::vector<T> next_loose(a.n, T(0));

    for (i32 s : a.init) {
        dp_tight[s] += T(1);
    }

    for (i32 x : N) {
        assert(0 <= x && x < base);
        std::fill(next_tight.begin(), next_tight.end(), T(0));
        std::fill(next_loose.begin(), next_loose.end(), T(0));

        for (i32 u = 0; u < a.n; ++u) {
            const T loose_val = dp_loose[u];
            const T tight_val = dp_tight[u];

            if (loose_val == T(0) && tight_val == T(0)) {
                continue;
            }

            if (!(loose_val == T(0))) {
                for (i32 c = 0; c < base; ++c) {
                    i32 v = a.edge(u, c);
                    if (v != -1) next_loose[v] += loose_val;
                }
            }

            if (!(tight_val == T(0))) {
                for (i32 c = 0; c < x; ++c) {
                    i32 v = a.edge(u, c);
                    if (v != -1) next_loose[v] += tight_val;
                }
                if (x < base) {
                    i32 v = a.edge(u, x);
                    if (v != -1) next_tight[v] += tight_val;
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

}  // namespace gwen
