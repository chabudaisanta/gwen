#pragma once

#include <cassert>
#include <limits>
#include <utility>
#include <vector>

#include "gwen/alge/monoid.hpp"
#include "gwen/types.hpp"

namespace gwen {

namespace automaton_detail {

template <i32 base> constexpr i32 checked_state_count(i32 n) {
    static_assert(base >= 1, "base must be positive");
    assert(n >= 0);
    return n;
}

template <i32 base> constexpr usize edge_count(i32 n) {
    assert(n >= 0);
    assert(static_cast<usize>(n) <= std::numeric_limits<usize>::max() / static_cast<usize>(base));
    return static_cast<usize>(n) * static_cast<usize>(base);
}

constexpr i32 checked_product_state_count(i32 lhs, i32 rhs) {
    assert(lhs >= 0 && rhs >= 0);
    const usize product = static_cast<usize>(lhs) * static_cast<usize>(rhs);
    assert(product <= static_cast<usize>(std::numeric_limits<i32>::max()));
    return static_cast<i32>(product);
}

template <i32 base> constexpr usize edge_index(i32 n, i32 from, i32 label) {
    assert(0 <= from && from < n);
    assert(0 <= label && label < base);
    return static_cast<usize>(from) * static_cast<usize>(base) + static_cast<usize>(label);
}

}  // namespace automaton_detail

/**
 * @brief オートマトンを表す構造体
 * @tparam base 遷移のアルファベットのサイズ
 */
template <i32 base> struct Automaton {
    static_assert(base >= 1, "base must be positive");

    i32 n;                    ///< 状態数
    std::vector<i32> init;    ///< 初期状態のリスト
    std::vector<i32> accept;  ///< 受理状態のリスト
    std::vector<i32> edges;   ///< 遷移リスト

    /** @brief 状態数0の空のオートマトンを構築する。 */
    Automaton() : Automaton(0) {}

    /**
     * @brief 状態数を指定してオートマトンを構築する。
     * @param n 状態数
     */
    Automaton(i32 n)
        : n(automaton_detail::checked_state_count<base>(n)), edges(automaton_detail::edge_count<base>(this->n), -1) {}

    /**
     * @brief 初期状態を追加する。
     * @param i 状態番号
     */
    void add_init(i32 i) {
        assert(0 <= i && i < n);
        init.push_back(i);
    }

    /**
     * @brief 区間 `[l, r)` の状態を初期状態として追加する。
     * @param l 区間の左端
     * @param r 区間の右端
     */
    void add_init(i32 l, i32 r) {
        assert(0 <= l && l <= r && r <= n);
        for (i32 i = l; i < r; ++i) init.push_back(i);
    }

    /**
     * @brief 受理状態を追加する。
     * @param i 状態番号
     */
    void add_accept(i32 i) {
        assert(0 <= i && i < n);
        accept.push_back(i);
    }

    /**
     * @brief 区間 `[l, r)` の状態を受理状態として追加する。
     * @param l 区間の左端
     * @param r 区間の右端
     */
    void add_accept(i32 l, i32 r) {
        assert(0 <= l && l <= r && r <= n);
        for (i32 i = l; i < r; ++i) accept.push_back(i);
    }

    /**
     * @brief 遷移を設定する。遷移先 `-1` は遷移なしを表す。
     * @param from 遷移元
     * @param label 遷移ラベル
     * @param to 遷移先
     */
    void set_edge(i32 from, i32 label, i32 to) {
        assert(to == -1 || (0 <= to && to < n));
        edges[automaton_detail::edge_index<base>(n, from, label)] = to;
    }

    /**
     * @brief 遷移先を取得する。
     * @param from 遷移元
     * @param label 遷移ラベル
     * @return 遷移先
     */
    i32 edge(i32 from, i32 label) const { return edges[automaton_detail::edge_index<base>(n, from, label)]; }

    /**
     * @brief 内部表現がオートマトンの不変条件を満たすか判定する。
     * @return 不変条件を満たす場合は `true`
     */
    [[nodiscard]] bool valid() const noexcept {
        if (n < 0 || edges.size() != automaton_detail::edge_count<base>(n)) return false;
        for (i32 state : init) {
            if (state < 0 || state >= n) return false;
        }
        for (i32 state : accept) {
            if (state < 0 || state >= n) return false;
        }
        for (i32 to : edges) {
            if (to != -1 && (to < 0 || to >= n)) return false;
        }
        return true;
    }

    /**
     * @brief 初期状態から到達可能な状態のみを残し、オートマトンを縮約する
     */
    void trim() {
        assert(valid());
        std::vector<i32> q;
        std::vector<i32> id(n, -1);
        for (i32 u : init) {
            if (id[u] == -1) {
                id[u] = q.size();
                q.push_back(u);
            }
        }
        for (usize head = 0; head < q.size(); ++head) {
            const i32 u = q[head];
            for (i32 x = 0; x < base; ++x) {
                const i32 v = edge(u, x);
                if (v != -1 && id[v] == -1) {
                    id[v] = q.size();
                    q.push_back(v);
                }
            }
        }

        const i32 new_n = static_cast<i32>(q.size());
        std::vector<i32> new_edges(automaton_detail::edge_count<base>(new_n), -1);
        for (i32 i = 0; i < new_n; ++i) {
            const i32 u = q[static_cast<usize>(i)];
            for (i32 x = 0; x < base; ++x) {
                const i32 v = edge(u, x);
                if (v != -1) {
                    new_edges[automaton_detail::edge_index<base>(new_n, i, x)] = id[v];
                }
            }
        }

        std::vector<i32> new_init;
        for (i32 u : init) {
            if (id[u] != -1) new_init.push_back(id[u]);
        }
        std::vector<i32> new_accept;
        for (i32 u : accept) {
            if (id[u] != -1) new_accept.push_back(id[u]);
        }

        n = new_n;
        edges = std::move(new_edges);
        init = std::move(new_init);
        accept = std::move(new_accept);
        assert(valid());
    }
};

/**
 * @brief オートマトンの直積などを扱うモノイド
 * @tparam base 遷移のアルファベットのサイズ
 */
template <i32 base> struct automaton_monoid {
    using S = Automaton<base>;  ///< 扱うデータ構造 (Automaton)

    /**
     * @brief 2つのオートマトンの直積を計算する
     * @details 到達可能性にかかわらず全状態を構築し、`trim()` は自動実行しない。
     * @param a 1つ目のオートマトン
     * @param b 2つ目のオートマトン
     * @return S 直積オートマトン
     */
    static S op(const S& a, const S& b) {
        assert(a.valid() && b.valid());
        auto prod = [&](const std::vector<i32>& s, const std::vector<i32>& t) {
            assert(t.empty() || s.size() <= std::numeric_limits<usize>::max() / t.size());
            std::vector<i32> res(s.size() * t.size());
            for (usize i = 0; i < s.size(); ++i) {
                for (usize j = 0; j < t.size(); ++j) {
                    res[i * t.size() + j] = s[i] * b.n + t[j];
                }
            }
            return res;
        };

        S res(automaton_detail::checked_product_state_count(a.n, b.n));
        res.init = prod(a.init, b.init);
        res.accept = prod(a.accept, b.accept);
        for (i32 i = 0; i < a.n; ++i) {
            for (i32 j = 0; j < b.n; ++j) {
                i32 k = i * b.n + j;
                for (i32 x = 0; x < base; ++x) {
                    const i32 na = a.edge(i, x);
                    const i32 nb = b.edge(j, x);
                    if (na == -1 || nb == -1) {
                        res.set_edge(k, x, -1);
                    }
                    else {
                        res.set_edge(k, x, na * b.n + nb);
                    }
                }
            }
        }
        assert(res.valid());
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
 * @tparam base 遷移アルファベットのサイズ
 * @param a 1つ目のオートマトン
 * @param b 2つ目のオートマトン
 * @return 直積オートマトン
 */
template <i32 base> Automaton<base> operator*(const Automaton<base>& a, const Automaton<base>& b) {
    return automaton_monoid<base>::op(a, b);
}

}  // namespace gwen
