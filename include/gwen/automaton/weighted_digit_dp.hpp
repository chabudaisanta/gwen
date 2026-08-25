#pragma once

#include <algorithm>
#include <cassert>
#include <concepts>
#include <functional>
#include <limits>
#include <type_traits>
#include <utility>
#include <vector>

#include "gwen/alge/monoid.hpp"
#include "gwen/alge/ring.hpp"
#include "gwen/alge/semiring.hpp"
#include "gwen/automaton/automaton.hpp"
#include "gwen/automaton/weighted_automaton.hpp"
#include "gwen/types.hpp"

namespace gwen {

/**
 * @brief 重み付き桁 DP の値型に必要な操作を表す Concept
 * @tparam T DP値型
 */
template <typename T>
concept weighted_digit_dp_value = std::copy_constructible<T> && requires(T value, const T other) {
    { T(0) } -> std::same_as<T>;
    { value = other } -> std::same_as<T&>;
    { value += other } -> std::same_as<T&>;
    { value == other } -> std::convertible_to<bool>;
};

/**
 * @brief 桁 DP で整数値の総和を求める値型に必要な操作を表す Concept
 * @tparam T 集計値型
 */
template <typename T>
concept digit_dp_sum_value =
    semiring<T> && std::copyable<T> && std::constructible_from<T, i32> && requires(T value, const T other) {
        { value += other } -> std::same_as<T&>;
        { value == other } -> std::convertible_to<bool>;
    };

namespace automaton_detail {

template <digit_dp_sum_value T> struct DigitDpSumState {
    T count;
    T sum;

    DigitDpSumState(i32 value) : count(value), sum(0) {}
    DigitDpSumState(T count, T sum) : count(std::move(count)), sum(std::move(sum)) {}

    DigitDpSumState& operator+=(const DigitDpSumState& rhs) {
        count += rhs.count;
        sum += rhs.sum;
        return *this;
    }

    bool operator==(const DigitDpSumState&) const = default;
};

template <semiring T> struct append_affine_monoid {
    using S = typename affine_monoid<T>::S;

    static constexpr S op(S first, S second) { return affine_monoid<T>::op(second, first); }
    static constexpr S e() { return affine_monoid<T>::e(); }
};

template <digit_dp_sum_value T>
DigitDpSumState<T> apply_digit_dp_sum_affine(const DigitDpSumState<T>& value,
                                             const typename append_affine_monoid<T>::S& affine) {
    return {value.count, affine.a * value.sum + affine.b * value.count};
}

template <digit_dp_sum_value T>
DigitDpSumState<T> multiply_digit_dp_sum_state(DigitDpSumState<T> value, u64 multiplicity) {
    DigitDpSumState<T> result(0);
    while (multiplicity != 0) {
        if (multiplicity & 1ULL) result += value;
        multiplicity >>= 1;
        if (multiplicity != 0) {
            const DigitDpSumState<T> copy = value;
            value += copy;
        }
    }
    return result;
}

template <typename F, typename T>
concept digit_dp_aggregator =
    std::invocable<F&, const T&, u64> && std::convertible_to<std::invoke_result_t<F&, const T&, u64>, T>;

template <typename OpAct, typename T, typename Weight>
concept digit_dp_action = std::invocable<OpAct&, const T&, const Weight&> &&
                          std::convertible_to<std::invoke_result_t<OpAct&, const T&, const Weight&>, T>;

template <weighted_digit_dp_value T, i32 base, monoid WeightMonoid, typename Init, typename F, typename OpAct>
    requires std::invocable<Init&, const typename WeightMonoid::S&> &&
             std::convertible_to<std::invoke_result_t<Init&, const typename WeightMonoid::S&>, T> &&
             digit_dp_aggregator<F, T> && digit_dp_action<OpAct, T, typename WeightMonoid::S>
T run_weighted_digit_dp_impl(const std::vector<i32>& upper_bound,
                             const WeightedAutomaton<base, WeightMonoid>& a,
                             Init init_value,
                             F f,
                             OpAct op_act) {
    assert(a.valid());
    std::vector<T> dp_tight(a.n, T(0));
    std::vector<T> dp_loose(a.n, T(0));
    std::vector<T> next_tight(a.n, T(0));
    std::vector<T> next_loose(a.n, T(0));

    for (const auto& entry : a.init) {
        dp_tight[entry.first] += static_cast<T>(std::invoke(init_value, entry.second));
    }

    for (i32 x : upper_bound) {
        assert(0 <= x && x < base);
        std::fill(next_tight.begin(), next_tight.end(), T(0));
        std::fill(next_loose.begin(), next_loose.end(), T(0));

        for (i32 u = 0; u < a.n; ++u) {
            const T loose_val = dp_loose[u];
            const T tight_val = dp_tight[u];

            if (!(loose_val == T(0))) {
                for (i32 c = 0; c < base; ++c) {
                    const i32 v = a.edge_to(u, c);
                    if (v != -1) {
                        next_loose[v] += static_cast<T>(std::invoke(op_act, loose_val, a.edge_weight(u, c)));
                    }
                }
            }

            if (!(tight_val == T(0))) {
                for (i32 c = 0; c < x; ++c) {
                    const i32 v = a.edge_to(u, c);
                    if (v != -1) {
                        next_loose[v] += static_cast<T>(std::invoke(op_act, tight_val, a.edge_weight(u, c)));
                    }
                }
                const i32 v = a.edge_to(u, x);
                if (v != -1) {
                    next_tight[v] += static_cast<T>(std::invoke(op_act, tight_val, a.edge_weight(u, x)));
                }
            }
        }

        std::swap(dp_tight, next_tight);
        std::swap(dp_loose, next_loose);
    }

    T ans = T(0);
    for (i32 u = 0; u < a.n; ++u) {
        T state_value = dp_tight[u];
        state_value += dp_loose[u];
        ans += static_cast<T>(std::invoke(f, state_value, a.condition[u]));
    }
    return ans;
}

}  // namespace automaton_detail

/**
 * @brief 重み付きオートマトン上の桁 DP を実行する
 * @tparam T DPで保持する値の型（`weighted_digit_dp_value` を満たすこと）
 * @tparam base n の進数
 * @tparam WeightMonoid オートマトンの重みのモノイド
 * @tparam F 最終的な値の集約関数の型。シグネチャは `T(T dp_val, u64 condition_mask)` を想定。
 * @tparam OpAct 遷移重みをDP値に作用させる関数の型。シグネチャは `T(T dp_val, Weight w)` を想定。
 * @param upper_bound 上限の値 (各桁の値が上位から順に格納されている配列)
 * @param a 条件と重み遷移を表すオートマトン
 * @param f 最終的な集約関数。各状態について得られたDP値と、その状態の達成条件マスクを受け取り、最終的なスコアを返す。
 * @param op_act DP値に対して遷移の重みを作用させる関数。デフォルトは乗算を想定。
 * @return T 集約された総和
 */
template <weighted_digit_dp_value T, i32 base, monoid WeightMonoid, typename F, typename OpAct = std::multiplies<>>
    requires requires(const typename WeightMonoid::S& weight) {
        { static_cast<T>(weight) } -> std::same_as<T>;
    } && automaton_detail::digit_dp_aggregator<F, T> &&
             automaton_detail::digit_dp_action<OpAct, T, typename WeightMonoid::S>
T run_weighted_digit_dp(const std::vector<i32>& upper_bound,
                        const WeightedAutomaton<base, WeightMonoid>& a,
                        F f,
                        OpAct op_act = OpAct{}) {
    auto init_value = [](const typename WeightMonoid::S& weight) -> T { return static_cast<T>(weight); };
    return automaton_detail::run_weighted_digit_dp_impl<T>(upper_bound, a, init_value, std::move(f), std::move(op_act));
}

/**
 * @brief 初期DP値を指定して重み付きオートマトン上の桁DPを実行する
 * @details 初期状態 `(s, w)` は `op_act(initial_value, w)` で初期化される。
 * @tparam T DP値型
 * @tparam base 進数
 * @tparam WeightMonoid 遷移重みのモノイド
 * @tparam F 最終集約関数
 * @tparam OpAct DP値への重みの作用
 * @param upper_bound 上限を上位桁から格納した配列
 * @param a 重み付きオートマトン
 * @param initial_value 重みを適用する前の初期DP値
 * @param f 最終集約関数
 * @param op_act DP値への重みの作用
 * @return 集約結果
 */
template <weighted_digit_dp_value T, i32 base, monoid WeightMonoid, typename F, typename OpAct>
    requires automaton_detail::digit_dp_aggregator<F, T> &&
             automaton_detail::digit_dp_action<OpAct, T, typename WeightMonoid::S>
T run_weighted_digit_dp(const std::vector<i32>& upper_bound,
                        const WeightedAutomaton<base, WeightMonoid>& a,
                        const T& initial_value,
                        F f,
                        OpAct op_act) {
    auto init_value = [&](const typename WeightMonoid::S& weight) -> T {
        return static_cast<T>(std::invoke(op_act, initial_value, weight));
    };
    return automaton_detail::run_weighted_digit_dp_impl<T>(upper_bound, a, init_value, std::move(f), std::ref(op_act));
}

/**
 * @brief オートマトンが受理する上限以下の整数の総和を求める
 * @tparam T 集計値型
 * @tparam base 進数
 * @param upper_bound 上限を上位桁から格納した配列
 * @param a 条件を表すオートマトン
 * @return `a` が受理する `0` 以上 `upper_bound` 以下の整数の総和
 */
template <digit_dp_sum_value T, i32 base>
T run_digit_dp_sum(const std::vector<i32>& upper_bound, const Automaton<base>& a) {
    assert(a.valid());
    using State = automaton_detail::DigitDpSumState<T>;
    using WeightMonoid = automaton_detail::append_affine_monoid<T>;

    WeightedAutomaton<base, WeightMonoid> weighted(a.n, 64);
    for (i32 state : a.init) {
        weighted.add_init(state, WeightMonoid::e());
    }

    std::vector<u64> accept_multiplicity(static_cast<usize>(a.n), 0ULL);
    for (i32 state : a.accept) {
        assert(accept_multiplicity[static_cast<usize>(state)] < std::numeric_limits<u64>::max());
        ++accept_multiplicity[static_cast<usize>(state)];
    }

    for (i32 state = 0; state < a.n; ++state) {
        weighted.set_condition(state, accept_multiplicity[static_cast<usize>(state)]);
        for (i32 digit = 0; digit < base; ++digit) {
            const i32 next = a.edge(state, digit);
            if (next != -1) {
                weighted.set_edge(state, digit, next, {T(base), T(digit)});
            }
        }
    }

    const auto aggregate = [](const State& value, u64 multiplicity) {
        return automaton_detail::multiply_digit_dp_sum_state(value, multiplicity);
    };
    const auto act = [](const State& value, const typename WeightMonoid::S& affine) {
        return automaton_detail::apply_digit_dp_sum_affine(value, affine);
    };

    const State result = run_weighted_digit_dp<State>(upper_bound, weighted, State{T(1), T(0)}, aggregate, act);
    return result.sum;
}

}  // namespace gwen
