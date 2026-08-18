#pragma once

#include <span>
#include <vector>
#include <bit>

#include "gwen/automaton/weighted_automaton.hpp"
#include "gwen/types.hpp"
#include "gwen/alge/monoid.hpp"

namespace gwen::automaton {

/**
 * @brief ちょうど k 種類の数字が使われるオートマトンを生成する
 * @tparam base N の進数
 * @tparam WeightMonoid 重みのモノイド
 * @param k 使用される数字の種類の目標数
 * @return WeightedAutomaton<base, WeightMonoid>
 */
template <i32 base, monoid WeightMonoid>
WeightedAutomaton<base, WeightMonoid> used_digits_count_exact(i32 k) {
    WeightedAutomaton<base, WeightMonoid> a(1 << base, 1);
    a.add_init(0, WeightMonoid::e());
    for (i32 u = 0; u < (1 << base); ++u) {
        if (std::popcount(static_cast<u32>(u)) == static_cast<u32>(k)) {
            a.set_condition(u, 1ULL);
        }
        for (i32 c = 0; c < base; ++c) {
            a.set_edge(u, c, u | (1 << c), WeightMonoid::e());
        }
    }
    return a;
}

/**
 * @brief k 種類以下の数字が使われるオートマトンを生成する
 * @tparam base N の進数
 * @tparam WeightMonoid 重みのモノイド
 * @param k 使用される数字の種類の目標数
 * @return WeightedAutomaton<base, WeightMonoid>
 */
template <i32 base, monoid WeightMonoid>
WeightedAutomaton<base, WeightMonoid> used_digits_count_leq(i32 k) {
    WeightedAutomaton<base, WeightMonoid> a(1 << base, 1);
    a.add_init(0, WeightMonoid::e());
    for (i32 u = 0; u < (1 << base); ++u) {
        if (std::popcount(static_cast<u32>(u)) <= static_cast<u32>(k)) {
            a.set_condition(u, 1ULL);
        }
        for (i32 c = 0; c < base; ++c) {
            a.set_edge(u, c, u | (1 << c), WeightMonoid::e());
        }
    }
    return a;
}

/**
 * @brief 指定した文字列(数字列)を含むオートマトンを生成する
 * @tparam base N の進数
 * @tparam WeightMonoid 重みのモノイド
 * @param pattern 含まれるべき文字列(数字列)のリスト
 * @return WeightedAutomaton<base, WeightMonoid>
 */
template <i32 base, monoid WeightMonoid>
WeightedAutomaton<base, WeightMonoid> contains_pattern(std::span<const i32> pattern) {
    i32 m = pattern.size();
    WeightedAutomaton<base, WeightMonoid> a(m + 1, 1);
    a.add_init(0, WeightMonoid::e());
    
    if (m > 0) {
        for (i32 c = 0; c < base; ++c) {
            a.set_edge(0, c, (pattern[0] == c) ? 1 : 0, WeightMonoid::e());
        }
        i32 x = 0;
        for (i32 u = 1; u < m; ++u) {
            for (i32 c = 0; c < base; ++c) {
                if (pattern[u] == c) {
                    a.set_edge(u, c, u + 1, WeightMonoid::e());
                } else {
                    a.set_edge(u, c, a.edge(x, c).first, WeightMonoid::e());
                }
            }
            x = a.edge(x, pattern[u]).first;
        }
    }
    
    for (i32 c = 0; c < base; ++c) {
        a.set_edge(m, c, m, WeightMonoid::e());
    }
    
    a.set_condition(m, 1ULL);
    
    return a;
}

} // namespace gwen::automaton
