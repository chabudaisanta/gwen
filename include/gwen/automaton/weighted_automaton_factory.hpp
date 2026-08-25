#pragma once

#include <span>
#include <utility>

#include "gwen/alge/monoid.hpp"
#include "gwen/automaton/factory.hpp"
#include "gwen/automaton/weighted_automaton.hpp"
#include "gwen/types.hpp"

namespace gwen::automaton {

namespace detail {

template <i32 base, monoid WeightMonoid> WeightedAutomaton<base, WeightMonoid> make_weighted(Automaton<base> source) {
    assert(source.valid());
    WeightedAutomaton<base, WeightMonoid> result;
    result.n = source.n;
    result.condition_count = 1;
    result.condition.assign(static_cast<usize>(source.n), 0ULL);
    result.edges = std::move(source.edges);
    result.edge_weights.assign(result.edges.size(), WeightMonoid::e());
    result.init.reserve(source.init.size());
    for (i32 state : source.init) {
        result.add_init(state, WeightMonoid::e());
    }
    for (i32 state : source.accept) {
        result.set_condition(state, 1ULL);
    }
    assert(result.valid());
    return result;
}

}  // namespace detail

/**
 * @brief ちょうど k 種類の数字が使われる重み付きオートマトンを生成する。
 * @tparam base 進数
 * @tparam WeightMonoid 重みのモノイド
 * @param k 使用される数字の種類数
 * @return 生成した重み付きオートマトン
 */
template <i32 base, monoid WeightMonoid> WeightedAutomaton<base, WeightMonoid> used_digits_count_exact(i32 k) {
    return detail::make_weighted<base, WeightMonoid>(gwen::automaton::used_digits_count_exact<base>(k));
}

/**
 * @brief k 種類以下の数字が使われる重み付きオートマトンを生成する。
 * @tparam base 進数
 * @tparam WeightMonoid 重みのモノイド
 * @param k 使用される数字の種類数の上限
 * @return 生成した重み付きオートマトン
 */
template <i32 base, monoid WeightMonoid> WeightedAutomaton<base, WeightMonoid> used_digits_count_leq(i32 k) {
    return detail::make_weighted<base, WeightMonoid>(gwen::automaton::used_digits_count_leq<base>(k));
}

/**
 * @brief 指定した数字列を含む重み付きオートマトンを生成する。
 * @tparam base 進数
 * @tparam WeightMonoid 重みのモノイド
 * @param pattern 検索する数字列
 * @return 生成した重み付きオートマトン
 */
template <i32 base, monoid WeightMonoid>
WeightedAutomaton<base, WeightMonoid> contains_pattern(std::span<const i32> pattern) {
    return detail::make_weighted<base, WeightMonoid>(gwen::automaton::contains_pattern<base>(pattern));
}

/**
 * @brief 各桁の和がちょうど M になる重み付きオートマトンを生成する。
 * @tparam base 進数
 * @tparam WeightMonoid 重みのモノイド
 * @param M 桁和
 * @return 生成した重み付きオートマトン
 */
template <i32 base, monoid WeightMonoid> WeightedAutomaton<base, WeightMonoid> digit_sum_exact(i32 M) {
    return detail::make_weighted<base, WeightMonoid>(gwen::automaton::digit_sum_exact<base>(M));
}

/**
 * @brief 各桁の和が M 以下になる重み付きオートマトンを生成する。
 * @tparam base 進数
 * @tparam WeightMonoid 重みのモノイド
 * @param M 桁和の上限
 * @return 生成した重み付きオートマトン
 */
template <i32 base, monoid WeightMonoid> WeightedAutomaton<base, WeightMonoid> digit_sum_leq(i32 M) {
    return detail::make_weighted<base, WeightMonoid>(gwen::automaton::digit_sum_leq<base>(M));
}

/**
 * @brief 各桁の和を M で割った余りが K になる重み付きオートマトンを生成する。
 * @tparam base 進数
 * @tparam WeightMonoid 重みのモノイド
 * @param M 法
 * @param K 余り
 * @return 生成した重み付きオートマトン
 */
template <i32 base, monoid WeightMonoid> WeightedAutomaton<base, WeightMonoid> digit_sum_mod(i32 M, i32 K) {
    return detail::make_weighted<base, WeightMonoid>(gwen::automaton::digit_sum_mod<base>(M, K));
}

/**
 * @brief 指定した数字を含まない重み付きオートマトンを生成する。
 * @tparam base 進数
 * @tparam WeightMonoid 重みのモノイド
 * @param digits 除外する数字
 * @return 生成した重み付きオートマトン
 */
template <i32 base, monoid WeightMonoid>
WeightedAutomaton<base, WeightMonoid> exclude_digits(std::span<const i32> digits) {
    return detail::make_weighted<base, WeightMonoid>(gwen::automaton::exclude_digits<base>(digits));
}

/**
 * @brief 指定した数字をすべて含む重み付きオートマトンを生成する。
 * @tparam base 進数
 * @tparam WeightMonoid 重みのモノイド
 * @param digits 必須の数字
 * @return 生成した重み付きオートマトン
 */
template <i32 base, monoid WeightMonoid>
WeightedAutomaton<base, WeightMonoid> include_all_digits(std::span<const i32> digits) {
    return detail::make_weighted<base, WeightMonoid>(gwen::automaton::include_all_digits<base>(digits));
}

/**
 * @brief 数値を M で割った余りが K になる重み付きオートマトンを生成する。
 * @tparam base 進数
 * @tparam WeightMonoid 重みのモノイド
 * @param M 法
 * @param K 余り
 * @return 生成した重み付きオートマトン
 */
template <i32 base, monoid WeightMonoid> WeightedAutomaton<base, WeightMonoid> value_mod(i32 M, i32 K) {
    return detail::make_weighted<base, WeightMonoid>(gwen::automaton::value_mod<base>(M, K));
}

/**
 * @brief 0 以外の数字がちょうど M 個現れる重み付きオートマトンを生成する。
 * @tparam base 進数
 * @tparam WeightMonoid 重みのモノイド
 * @param M 目標個数
 * @return 生成した重み付きオートマトン
 */
template <i32 base, monoid WeightMonoid> WeightedAutomaton<base, WeightMonoid> non_zero_count_exact(i32 M) {
    return detail::make_weighted<base, WeightMonoid>(gwen::automaton::non_zero_count_exact<base>(M));
}

/**
 * @brief 0 以外の数字が M 個以下現れる重み付きオートマトンを生成する。
 * @tparam base 進数
 * @tparam WeightMonoid 重みのモノイド
 * @param M 個数の上限
 * @return 生成した重み付きオートマトン
 */
template <i32 base, monoid WeightMonoid> WeightedAutomaton<base, WeightMonoid> non_zero_count_leq(i32 M) {
    return detail::make_weighted<base, WeightMonoid>(gwen::automaton::non_zero_count_leq<base>(M));
}

}  // namespace gwen::automaton
