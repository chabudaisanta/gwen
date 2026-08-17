#pragma once

#include <vector>
#include <utility>

#include "gwen/types.hpp"
#include "gwen/alge/ring.hpp"
#include "gwen/alge/monoid.hpp"

namespace gwen {

/**
 * @brief オートマトンを表す構造体
 * @tparam T 遷移のコストを表す型 (gwen::ring を満たすこと)
 * @tparam base 遷移のアルファベットのサイズ
 */
template <ring T, i32 base>
struct Automaton {
    i32 n_states;                          ///< 状態数
    std::vector<i32> init;                 ///< 初期状態のリスト
    std::vector<i32> accept;               ///< 受理状態のリスト
    std::vector<std::pair<i32, T>> edges;  ///< 遷移リスト (edges[u * base + c] = {next_state, cost})
};

/**
 * @brief オートマトンの直積などを扱うモノイド
 * @tparam T 遷移のコストを表す型 (gwen::ring を満たすこと)
 * @tparam base 遷移のアルファベットのサイズ
 */
template <ring T, i32 base>
struct automaton_monoid {
    using S = Automaton<T, base>; ///< 扱うデータ構造 (Automaton)

    /**
     * @brief 2つのオートマトンの直積を計算する
     * @param a 1つ目のオートマトン
     * @param b 2つ目のオートマトン
     * @return S 直積オートマトン
     */
    static S op(const S& a, const S& b) {
        auto prod = [&](const std::vector<i32>& s, const std::vector<i32>& t) {
            std::vector<i32> res(s.size() * t.size());
            for (i32 i = 0; i < static_cast<i32>(s.size()); ++i) {
                for (i32 j = 0; j < static_cast<i32>(t.size()); ++j) {
                    res[i * t.size() + j] = s[i] * b.n_states + t[j];
                }
            }
            return res;
        };

        return S {
            .n_states = a.n_states * b.n_states,
            .init = prod(a.init, b.init),
            .accept = prod(a.accept, b.accept),
            .edges = [&]() {
                std::vector<std::pair<i32, T>> res(a.n_states * b.n_states * base);
                for (i32 i = 0; i < a.n_states; ++i) {
                    for (i32 j = 0; j < b.n_states; ++j) {
                        i32 k = i * b.n_states + j;
                        for (i32 x = 0; x < base; ++x) {
                            const auto& p = a.edges[i * base + x];
                            const auto& q = b.edges[j * base + x];
                            res[k * base + x] = {
                                p.first * b.n_states + q.first,
                                p.second * q.second
                            };
                        }
                    }
                }
                return res;
            }()
        };
    }

    /**
     * @brief 単位オートマトン (直積の単位元)
     * @return S 単位オートマトン
     */
    static S e() {
        return S {
            .n_states = 1,
            .init = { 0 },
            .accept = { 0 },
            .edges = []() {
                std::vector<std::pair<i32, T>> res(base);
                for (i32 x = 0; x < base; ++x) {
                    res[x] = { 0, T(1) };
                }
                return res;
            }()
        };
    }
};

} // namespace gwen
