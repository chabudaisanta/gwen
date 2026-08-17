#pragma once

#include <span>
#include <vector>
#include <utility>

#include "gwen/types.hpp"
#include "gwen/alge/ring.hpp"
#include "gwen/automaton/automaton.hpp"

namespace gwen::automaton {

/**
 * @brief 各位の数字の和がちょうど M になるオートマトンを生成する
 * @tparam T 遷移コストを表す型 (ring)
 * @tparam base N の進数
 * @param M 目標となる和
 * @return Automaton<T, base>
 */
template <ring T, i32 base = 10>
Automaton<T, base> digit_sum_exact(i32 M) {
    i32 n_states = M + 1;
    std::vector<std::pair<i32, T>> edges(n_states * base);
    for (i32 u = 0; u <= M; ++u) {
        for (i32 c = 0; c < base; ++c) {
            if (u + c <= M) {
                edges[u * base + c] = {u + c, T(1)};
            } else {
                edges[u * base + c] = {0, T(0)}; // Trap state represented as cost T(0)
            }
        }
    }
    return Automaton<T, base>{
        .n_states = n_states,
        .init = {0},
        .accept = {M},
        .edges = edges
    };
}

/**
 * @brief 各位の数字の和が M 以下になるオートマトンを生成する
 * @tparam T 遷移コストを表す型 (ring)
 * @tparam base N の進数
 * @param M 和の上限
 * @return Automaton<T, base>
 */
template <ring T, i32 base = 10>
Automaton<T, base> digit_sum_leq(i32 M) {
    auto a = digit_sum_exact<T, base>(M);
    a.accept.clear();
    for (i32 u = 0; u <= M; ++u) {
        a.accept.push_back(u);
    }
    return a;
}

/**
 * @brief 各位の数字の和を M で割った余りが K になるオートマトンを生成する
 * @tparam T 遷移コストを表す型 (ring)
 * @tparam base N の進数
 * @param M 法
 * @param K 余り
 * @return Automaton<T, base>
 */
template <ring T, i32 base = 10>
Automaton<T, base> digit_sum_mod(i32 M, i32 K) {
    i32 n_states = M;
    std::vector<std::pair<i32, T>> edges(n_states * base);
    for (i32 u = 0; u < M; ++u) {
        for (i32 c = 0; c < base; ++c) {
            edges[u * base + c] = {(u + c) % M, T(1)};
        }
    }
    return Automaton<T, base>{
        .n_states = n_states,
        .init = {0},
        .accept = {K},
        .edges = edges
    };
}

/**
 * @brief 指定した文字を含まないオートマトンを生成する
 * @tparam T 遷移コストを表す型 (ring)
 * @tparam base N の進数
 * @param S 含まれてはならない文字のリスト
 * @return Automaton<T, base>
 */
template <ring T, i32 base = 10>
Automaton<T, base> exclude_digits(std::span<const i32> S) {
    i32 n_states = 1;
    std::vector<std::pair<i32, T>> edges(base, {0, T(1)});
    for (i32 c : S) {
        if (0 <= c && c < base) {
            edges[c] = {0, T(0)}; // Transition cost 0 effectively drops the path
        }
    }
    return Automaton<T, base>{
        .n_states = n_states,
        .init = {0},
        .accept = {0},
        .edges = edges
    };
}

/**
 * @brief 数値そのものを M で割った余りが K になるオートマトンを生成する
 * @tparam T 遷移コストを表す型 (ring)
 * @tparam base N の進数
 * @param M 法
 * @param K 余り
 * @return Automaton<T, base>
 */
template <ring T, i32 base = 10>
Automaton<T, base> value_mod(i32 M, i32 K) {
    i32 n_states = M;
    std::vector<std::pair<i32, T>> edges(n_states * base);
    for (i32 u = 0; u < M; ++u) {
        for (i32 c = 0; c < base; ++c) {
            edges[u * base + c] = {(u * base + c) % M, T(1)};
        }
    }
    return Automaton<T, base>{
        .n_states = n_states,
        .init = {0},
        .accept = {K},
        .edges = edges
    };
}

/**
 * @brief 0 以外の数字がちょうど M 個現れるオートマトンを生成する
 * @tparam T 遷移コストを表す型 (ring)
 * @tparam base N の進数
 * @param M 0以外の数字の目標個数
 * @return Automaton<T, base>
 */
template <ring T, i32 base = 10>
Automaton<T, base> non_zero_count_exact(i32 M) {
    i32 n_states = M + 1;
    std::vector<std::pair<i32, T>> edges(n_states * base);
    for (i32 u = 0; u <= M; ++u) {
        for (i32 c = 0; c < base; ++c) {
            i32 next_u = u + (c == 0 ? 0 : 1);
            if (next_u <= M) {
                edges[u * base + c] = {next_u, T(1)};
            } else {
                edges[u * base + c] = {0, T(0)};
            }
        }
    }
    return Automaton<T, base>{
        .n_states = n_states,
        .init = {0},
        .accept = {M},
        .edges = edges
    };
}

/**
 * @brief 0 以外の数字が M 個以下現れるオートマトンを生成する
 * @tparam T 遷移コストを表す型 (ring)
 * @tparam base N の進数
 * @param M 0以外の数字の個数の上限
 * @return Automaton<T, base>
 */
template <ring T, i32 base = 10>
Automaton<T, base> non_zero_count_leq(i32 M) {
    auto a = non_zero_count_exact<T, base>(M);
    a.accept.clear();
    for (i32 u = 0; u <= M; ++u) {
        a.accept.push_back(u);
    }
    return a;
}

} // namespace gwen::automaton
