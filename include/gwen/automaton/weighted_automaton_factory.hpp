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
    i32 lz_state = 1 << base;
    WeightedAutomaton<base, WeightMonoid> a(lz_state + 1, 1);
    a.add_init(lz_state, WeightMonoid::e());
    
    if (k == 0) {
        a.set_condition(lz_state, 1ULL);
    }
    a.set_edge(lz_state, 0, lz_state, WeightMonoid::e());
    for (i32 c = 1; c < base; ++c) {
        a.set_edge(lz_state, c, 1 << c, WeightMonoid::e());
    }

    for (i32 u = 0; u < (1 << base); ++u) {
        if (std::popcount(static_cast<u32>(u)) == k) {
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
    i32 lz_state = 1 << base;
    WeightedAutomaton<base, WeightMonoid> a(lz_state + 1, 1);
    a.add_init(lz_state, WeightMonoid::e());
    
    if (k >= 0) {
        a.set_condition(lz_state, 1ULL);
    }
    a.set_edge(lz_state, 0, lz_state, WeightMonoid::e());
    for (i32 c = 1; c < base; ++c) {
        a.set_edge(lz_state, c, 1 << c, WeightMonoid::e());
    }

    for (i32 u = 0; u < (1 << base); ++u) {
        if (std::popcount(static_cast<u32>(u)) <= k) {
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

/**
 * @brief 各位の数字の和がちょうど M になるオートマトンを生成する
 * @tparam base N の進数
 * @tparam WeightMonoid 重みのモノイド
 * @param M 目標となる和
 * @return WeightedAutomaton<base, WeightMonoid>
 */
template <i32 base, monoid WeightMonoid>
WeightedAutomaton<base, WeightMonoid> digit_sum_exact(i32 M) {
    WeightedAutomaton<base, WeightMonoid> a(M + 1, 1);
    a.add_init(0, WeightMonoid::e());
    a.set_condition(M, 1ULL);
    for (i32 u = 0; u <= M; ++u) {
        for (i32 c = 0; c < base; ++c) {
            if (u + c <= M) {
                a.set_edge(u, c, u + c, WeightMonoid::e());
            }
        }
    }
    return a;
}

/**
 * @brief 各位の数字の和が M 以下になるオートマトンを生成する
 * @tparam base N の進数
 * @tparam WeightMonoid 重みのモノイド
 * @param M 和の上限
 * @return WeightedAutomaton<base, WeightMonoid>
 */
template <i32 base, monoid WeightMonoid>
WeightedAutomaton<base, WeightMonoid> digit_sum_leq(i32 M) {
    WeightedAutomaton<base, WeightMonoid> a(M + 1, 1);
    a.add_init(0, WeightMonoid::e());
    for (i32 u = 0; u <= M; ++u) {
        a.set_condition(u, 1ULL);
        for (i32 c = 0; c < base; ++c) {
            if (u + c <= M) {
                a.set_edge(u, c, u + c, WeightMonoid::e());
            }
        }
    }
    return a;
}

/**
 * @brief 各位の数字の和を M で割った余りが K になるオートマトンを生成する
 * @tparam base N の進数
 * @tparam WeightMonoid 重みのモノイド
 * @param M 法
 * @param K 余り
 * @return WeightedAutomaton<base, WeightMonoid>
 */
template <i32 base, monoid WeightMonoid>
WeightedAutomaton<base, WeightMonoid> digit_sum_mod(i32 M, i32 K) {
    WeightedAutomaton<base, WeightMonoid> a(M, 1);
    a.add_init(0, WeightMonoid::e());
    a.set_condition(K, 1ULL);
    for (i32 u = 0; u < M; ++u) {
        for (i32 c = 0; c < base; ++c) {
            a.set_edge(u, c, (u + c) % M, WeightMonoid::e());
        }
    }
    return a;
}

/**
 * @brief 指定した文字を含まないオートマトンを生成する
 * @tparam base N の進数
 * @tparam WeightMonoid 重みのモノイド
 * @param S 含まれてはならない文字のリスト
 * @return WeightedAutomaton<base, WeightMonoid>
 */
template <i32 base, monoid WeightMonoid>
WeightedAutomaton<base, WeightMonoid> exclude_digits(std::span<const i32> S) {
    WeightedAutomaton<base, WeightMonoid> a(1, 1);
    a.add_init(0, WeightMonoid::e());
    a.set_condition(0, 1ULL);
    for (i32 c = 0; c < base; ++c) {
        a.set_edge(0, c, 0, WeightMonoid::e());
    }
    for (i32 c : S) {
        if (0 <= c && c < base) {
            a.set_edge(0, c, -1, WeightMonoid::e());
        }
    }
    return a;
}

/**
 * @brief 指定した文字をすべて含むオートマトンを生成する
 * @tparam base N の進数
 * @tparam WeightMonoid 重みのモノイド
 * @param S 含まれなければならない文字のリスト
 * @return WeightedAutomaton<base, WeightMonoid>
 */
template <i32 base, monoid WeightMonoid>
WeightedAutomaton<base, WeightMonoid> include_all_digits(std::span<const i32> S) {
    std::vector<i32> id(base, -1);
    i32 num_req = 0;
    for (i32 s : S) {
        if (0 <= s && s < base && id[s] == -1) {
            id[s] = num_req++;
        }
    }

    WeightedAutomaton<base, WeightMonoid> a(1 << num_req, 1);
    a.add_init(0, WeightMonoid::e());
    a.set_condition((1 << num_req) - 1, 1ULL);

    for (i32 u = 0; u < a.n; ++u) {
        for (i32 c = 0; c < base; ++c) {
            if (id[c] >= 0) a.set_edge(u, c, u | (1 << id[c]), WeightMonoid::e());
            else a.set_edge(u, c, u, WeightMonoid::e());
        }
    }
    return a;
}

/**
 * @brief 数値そのものを M で割った余りが K になるオートマトンを生成する
 * @tparam base N の進数
 * @tparam WeightMonoid 重みのモノイド
 * @param M 法
 * @param K 余り
 * @return WeightedAutomaton<base, WeightMonoid>
 */
template <i32 base, monoid WeightMonoid>
WeightedAutomaton<base, WeightMonoid> value_mod(i32 M, i32 K) {
    WeightedAutomaton<base, WeightMonoid> a(M, 1);
    a.add_init(0, WeightMonoid::e());
    a.set_condition(K, 1ULL);
    for (i32 u = 0; u < M; ++u) {
        for (i32 c = 0; c < base; ++c) {
            a.set_edge(u, c, (u * base + c) % M, WeightMonoid::e());
        }
    }
    return a;
}

/**
 * @brief 0 以外の数字がちょうど M 個現れるオートマトンを生成する
 * @tparam base N の進数
 * @tparam WeightMonoid 重みのモノイド
 * @param M 0以外の数字の目標個数
 * @return WeightedAutomaton<base, WeightMonoid>
 */
template <i32 base, monoid WeightMonoid>
WeightedAutomaton<base, WeightMonoid> non_zero_count_exact(i32 M) {
    WeightedAutomaton<base, WeightMonoid> a(M + 1, 1);
    a.add_init(0, WeightMonoid::e());
    a.set_condition(M, 1ULL);
    for (i32 u = 0; u <= M; ++u) {
        for (i32 c = 0; c < base; ++c) {
            i32 next_u = u + (c == 0 ? 0 : 1);
            if (next_u <= M) {
                a.set_edge(u, c, next_u, WeightMonoid::e());
            }
        }
    }
    return a;
}

/**
 * @brief 0 以外の数字が M 個以下現れるオートマトンを生成する
 * @tparam base N の進数
 * @tparam WeightMonoid 重みのモノイド
 * @param M 0以外の数字の個数の上限
 * @return WeightedAutomaton<base, WeightMonoid>
 */
template <i32 base, monoid WeightMonoid>
WeightedAutomaton<base, WeightMonoid> non_zero_count_leq(i32 M) {
    WeightedAutomaton<base, WeightMonoid> a(M + 1, 1);
    a.add_init(0, WeightMonoid::e());
    for (i32 u = 0; u <= M; ++u) {
        a.set_condition(u, 1ULL);
        for (i32 c = 0; c < base; ++c) {
            i32 next_u = u + (c == 0 ? 0 : 1);
            if (next_u <= M) {
                a.set_edge(u, c, next_u, WeightMonoid::e());
            }
        }
    }
    return a;
}

} // namespace gwen::automaton
