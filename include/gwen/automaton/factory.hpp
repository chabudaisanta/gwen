#pragma once

#include <bit>
#include <cassert>
#include <limits>
#include <span>
#include <vector>

#include "gwen/automaton/automaton.hpp"
#include "gwen/types.hpp"

namespace gwen::automaton {

/**
 * @brief 各位の数字の和がちょうど M になるオートマトンを生成する
 * @tparam base N の進数
 * @param M 目標となる和
 * @return Automaton<base>
 */
template <i32 base = 10> Automaton<base> digit_sum_exact(i32 M) {
    assert(0 <= M && M < std::numeric_limits<i32>::max());
    Automaton<base> a(M + 1);
    a.init = {0};
    a.accept = {M};
    for (i32 u = 0; u <= M; ++u) {
        for (i32 c = 0; c < base; ++c) {
            const i64 next = static_cast<i64>(u) + c;
            if (next <= M) {
                a.set_edge(u, c, static_cast<i32>(next));
            }
        }
    }
    return a;
}

/**
 * @brief 各位の数字の和が M 以下になるオートマトンを生成する
 * @tparam base N の進数
 * @param M 和の上限
 * @return Automaton<base>
 */
template <i32 base = 10> Automaton<base> digit_sum_leq(i32 M) {
    auto a = digit_sum_exact<base>(M);
    a.accept.clear();
    for (i32 u = 0; u <= M; ++u) {
        a.accept.push_back(u);
    }
    return a;
}

/**
 * @brief 各位の数字の和を M で割った余りが K になるオートマトンを生成する
 * @tparam base N の進数
 * @param M 法
 * @param K 余り
 * @return Automaton<base>
 */
template <i32 base = 10> Automaton<base> digit_sum_mod(i32 M, i32 K) {
    assert(M > 0);
    assert(0 <= K && K < M);
    Automaton<base> a(M);
    a.init = {0};
    a.accept = {K};
    for (i32 u = 0; u < M; ++u) {
        for (i32 c = 0; c < base; ++c) {
            const i64 sum = static_cast<i64>(u) + c;
            a.set_edge(u, c, static_cast<i32>(sum % M));
        }
    }
    return a;
}

/**
 * @brief 指定した文字を含まないオートマトンを生成する
 * @tparam base N の進数
 * @param S 含まれてはならない文字のリスト
 * @return Automaton<base>
 */
template <i32 base = 10> Automaton<base> exclude_digits(std::span<const i32> S) {
    Automaton<base> a(1);
    a.init = {0};
    a.accept = {0};
    for (i32 c = 0; c < base; ++c) {
        a.set_edge(0, c, 0);
    }
    for (i32 c : S) {
        if (0 <= c && c < base) {
            a.set_edge(0, c, -1);
        }
    }
    return a;
}

/**
 * @brief 指定した文字をすべて含むオートマトンを生成する
 * @tparam base N の進数
 * @param S 含まれなければならない文字のリスト
 * @return Automaton<base>
 */
template <i32 base = 10> Automaton<base> include_all_digits(std::span<const i32> S) {
    std::vector<i32> id(base, -1);
    i32 num_req = 0;
    for (i32 s : S) {
        if (0 <= s && s < base && id[s] == -1) {
            id[s] = num_req++;
        }
    }

    if (num_req >= std::numeric_limits<i32>::digits) {
        assert(false && "too many required digits");
        return Automaton<base>();
    }
    const i32 state_count = static_cast<i32>(u32{1} << num_req);
    Automaton<base> a(state_count);
    a.init = {0};
    a.accept = {state_count - 1};

    for (i32 u = 0; u < a.n; ++u) {
        for (i32 c = 0; c < base; ++c) {
            if (id[c] >= 0)
                a.set_edge(u, c, u | static_cast<i32>(u32{1} << id[c]));
            else
                a.set_edge(u, c, u);
        }
    }
    return a;
}

/**
 * @brief 数値そのものを M で割った余りが K になるオートマトンを生成する
 * @tparam base N の進数
 * @param M 法
 * @param K 余り
 * @return Automaton<base>
 */
template <i32 base = 10> Automaton<base> value_mod(i32 M, i32 K) {
    assert(M > 0);
    assert(0 <= K && K < M);
    Automaton<base> a(M);
    a.init = {0};
    a.accept = {K};
    for (i32 u = 0; u < M; ++u) {
        for (i32 c = 0; c < base; ++c) {
            const i64 value = static_cast<i64>(u) * base + c;
            a.set_edge(u, c, static_cast<i32>(value % M));
        }
    }
    return a;
}

/**
 * @brief 0 以外の数字がちょうど M 個現れるオートマトンを生成する
 * @tparam base N の進数
 * @param M 0以外の数字の目標個数
 * @return Automaton<base>
 */
template <i32 base = 10> Automaton<base> non_zero_count_exact(i32 M) {
    assert(0 <= M && M < std::numeric_limits<i32>::max());
    Automaton<base> a(M + 1);
    a.init = {0};
    a.accept = {M};
    for (i32 u = 0; u <= M; ++u) {
        for (i32 c = 0; c < base; ++c) {
            i32 next_u = u + (c == 0 ? 0 : 1);
            if (next_u <= M) {
                a.set_edge(u, c, next_u);
            }
        }
    }
    return a;
}

/**
 * @brief 0 以外の数字が M 個以下現れるオートマトンを生成する
 * @tparam base N の進数
 * @param M 0以外の数字の個数の上限
 * @return Automaton<base>
 */
template <i32 base = 10> Automaton<base> non_zero_count_leq(i32 M) {
    auto a = non_zero_count_exact<base>(M);
    a.accept.clear();
    for (i32 u = 0; u <= M; ++u) {
        a.accept.push_back(u);
    }
    return a;
}

/**
 * @brief ちょうど k 種類の数字が使われるオートマトンを生成する
 * @tparam base N の進数
 * @param k 使用される数字の種類の目標数
 * @return Automaton<base>
 */
template <i32 base = 10> Automaton<base> used_digits_count_exact(i32 k) {
    static_assert(base < std::numeric_limits<i32>::digits, "used_digits_count_exact requires base < 31");
    const i32 lz_state = static_cast<i32>(u32{1} << base);
    Automaton<base> a(lz_state + 1);
    a.init = {lz_state};

    if (k == 0) {
        a.accept.push_back(lz_state);
    }
    a.set_edge(lz_state, 0, lz_state);
    for (i32 c = 1; c < base; ++c) {
        a.set_edge(lz_state, c, static_cast<i32>(u32{1} << c));
    }

    for (i32 u = 0; u < lz_state; ++u) {
        if (std::popcount(static_cast<u32>(u)) == k) {
            a.accept.push_back(u);
        }
        for (i32 c = 0; c < base; ++c) {
            a.set_edge(u, c, u | static_cast<i32>(u32{1} << c));
        }
    }
    return a;
}

/**
 * @brief k 種類以下の数字が使われるオートマトンを生成する
 * @tparam base N の進数
 * @param k 使用される数字の種類の目標数
 * @return Automaton<base>
 */
template <i32 base = 10> Automaton<base> used_digits_count_leq(i32 k) {
    static_assert(base < std::numeric_limits<i32>::digits, "used_digits_count_leq requires base < 31");
    const i32 lz_state = static_cast<i32>(u32{1} << base);
    Automaton<base> a(lz_state + 1);
    a.init = {lz_state};

    if (k >= 0) {
        a.accept.push_back(lz_state);
    }
    a.set_edge(lz_state, 0, lz_state);
    for (i32 c = 1; c < base; ++c) {
        a.set_edge(lz_state, c, static_cast<i32>(u32{1} << c));
    }

    for (i32 u = 0; u < lz_state; ++u) {
        if (std::popcount(static_cast<u32>(u)) <= k) {
            a.accept.push_back(u);
        }
        for (i32 c = 0; c < base; ++c) {
            a.set_edge(u, c, u | static_cast<i32>(u32{1} << c));
        }
    }
    return a;
}

/**
 * @brief 指定した文字列(数字列)を含むオートマトンを生成する
 * @tparam base N の進数
 * @param pattern 含まれるべき文字列(数字列)のリスト
 * @return Automaton<base>
 */
template <i32 base = 10> Automaton<base> contains_pattern(std::span<const i32> pattern) {
    assert(pattern.size() <= static_cast<usize>(std::numeric_limits<i32>::max() - 1));
    for (i32 digit : pattern) {
        assert(0 <= digit && digit < base);
    }
    const i32 m = static_cast<i32>(pattern.size());
    Automaton<base> a(m + 1);
    a.init = {0};

    if (m > 0) {
        for (i32 c = 0; c < base; ++c) {
            a.set_edge(0, c, (pattern[0] == c) ? 1 : 0);
        }
        i32 x = 0;
        for (i32 u = 1; u < m; ++u) {
            for (i32 c = 0; c < base; ++c) {
                if (pattern[u] == c) {
                    a.set_edge(u, c, u + 1);
                }
                else {
                    a.set_edge(u, c, a.edge(x, c));
                }
            }
            x = a.edge(x, pattern[u]);
        }
    }

    for (i32 c = 0; c < base; ++c) {
        a.set_edge(m, c, m);
    }

    a.accept = {m};
    return a;
}

}  // namespace gwen::automaton
