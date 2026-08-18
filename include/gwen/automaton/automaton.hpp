#pragma once

#include <vector>

#include "gwen/alge/monoid.hpp"
#include "gwen/types.hpp"

namespace gwen {

/**
 * @brief オートマトンを表す構造体
 * @tparam base 遷移のアルファベットのサイズ
 */
template <i32 base>
struct Automaton {
    i32 n;               // 状態数
    std::vector<i32> init;      // 初期状態のリスト
    std::vector<i32> accept;    // 受理状態のリスト
    std::vector<i32> edges;     // 遷移リスト

    Automaton() : Automaton(0) {}
    Automaton(i32 n) : n(n), edges(n * base, -1) {}

    void add_init(i32 i) {
        init.push_back(i);
    }
    void add_init(i32 l, i32 r) {
        for(i32 i = l; i < r; ++i) init.push_back(i);
    }
    void add_accept(i32 i) {
        accept.push_back(i);
    }
    void add_accept(i32 l, i32 r) {
        for(i32 i = l; i < r; ++i) accept.push_back(i);
    }
    void set_edge(i32 from, i32 label, i32 to) {
        edges[from * base + label] = to;
    }
    i32 edge(i32 from, i32 label) const {
        return edges[from * base + label];
    }
};

/**
 * @brief オートマトンの直積などを扱うモノイド
 * @tparam base 遷移のアルファベットのサイズ
 */
template <i32 base>
struct automaton_monoid {
    using S = Automaton<base>; ///< 扱うデータ構造 (Automaton)

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
                    res[i * t.size() + j] = s[i] * b.n + t[j];
                }
            }
            return res;
        };

        S res(a.n * b.n);
        res.init = prod(a.init, b.init);
        res.accept = prod(a.accept, b.accept);
        for (i32 i = 0; i < a.n; ++i) {
            for (i32 j = 0; j < b.n; ++j) {
                i32 k = i * b.n + j;
                for (i32 x = 0; x < base; ++x) {
                    i32 na = a.edges[i * base + x];
                    i32 nb = b.edges[j * base + x];
                    if (na == -1 || nb == -1) {
                        res.edges[k * base + x] = -1;
                    } else {
                        res.edges[k * base + x] = na * b.n + nb;
                    }
                }
            }
        }
        return res;
    }

    /**
     * @brief 単位オートマトン (直積の単位元)
     * @return S 単位オートマトン
     */
    static S e() {
        S res(1);
        res.init = {0};
        res.accept = {0};
        res.edges = std::vector<i32>(base, 0);
        return res;
    }
};

/**
 * @brief 2つのオートマトンの直積を計算する (* 演算子でのシンタックスシュガー)
 */
template <i32 base>
Automaton<base> operator*(const Automaton<base>& a, const Automaton<base>& b) {
    return automaton_monoid<base>::op(a, b);
}

} // namespace gwen
