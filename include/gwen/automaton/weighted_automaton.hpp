#pragma once

#include <cassert>
#include <utility>
#include <vector>

#include "gwen/alge/monoid.hpp"
#include "gwen/automaton/automaton.hpp"
#include "gwen/types.hpp"

namespace gwen {

/**
 * @brief 重み付きオートマトンを表す構造体
 * @tparam base 遷移アルファベットのサイズ
 * @tparam WeightMonoid 重みの演算を定義するモノイド
 */
template <i32 base, monoid WeightMonoid> struct WeightedAutomaton {
    static_assert(base >= 1, "base must be positive");

    using Weight = typename WeightMonoid::S;  ///< 遷移重みの型

    i32 n;                                     ///< 状態数
    i32 condition_count;                       ///< このオートマトンが管理する条件の数
    std::vector<std::pair<i32, Weight>> init;  ///< 初期状態と初期重みのリスト
    std::vector<u64> condition;                ///< 達成状況の bitmask
    std::vector<i32> edges;                    ///< 遷移リスト
    std::vector<Weight> edge_weights;          ///< 遷移重みリスト

    /**
     * @brief デフォルトコンストラクタ
     */
    WeightedAutomaton() : WeightedAutomaton(0, 0) {}

    /**
     * @brief 状態数と条件数を指定して初期化
     * @param n 状態数
     * @param condition_count 条件の数 (デフォルト1)
     */
    WeightedAutomaton(i32 n, i32 condition_count = 1)
        : n(automaton_detail::checked_state_count<base>(n)),
          condition_count(checked_condition_count(condition_count)),
          condition(this->n, 0ULL),
          edges(automaton_detail::edge_count<base>(this->n), -1),
          edge_weights(automaton_detail::edge_count<base>(this->n), WeightMonoid::e()) {}

    /**
     * @brief 初期状態を追加する
     * @param i 状態
     * @param w 初期重み
     */
    void add_init(i32 i, Weight w) {
        assert(0 <= i && i < n);
        init.push_back({i, w});
    }

    /**
     * @brief 状態の達成条件(bitmask)を設定する
     * @param i 状態
     * @param mask 達成状況の bitmask
     */
    void set_condition(i32 i, u64 mask = 1) {
        assert(0 <= i && i < n);
        if (condition_count < 64) assert((mask >> condition_count) == 0);
        condition[i] = mask;
    }

    /**
     * @brief 遷移を追加する
     * @param from 遷移元の状態
     * @param label 遷移ラベル
     * @param to 遷移先の状態
     * @param w 遷移重み
     */
    void set_edge(i32 from, i32 label, i32 to, Weight w) {
        assert(to == -1 || (0 <= to && to < n));
        const usize pos = automaton_detail::edge_index<base>(n, from, label);
        edges[pos] = to;
        edge_weights[pos] = std::move(w);
    }

    /**
     * @brief 遷移を取得する
     * @param from 遷移元の状態
     * @param label 遷移ラベル
     * @return 遷移先の状態と重みのペア
     */
    std::pair<i32, Weight> edge(i32 from, i32 label) const {
        const usize pos = automaton_detail::edge_index<base>(n, from, label);
        return {edges[pos], edge_weights[pos]};
    }

    /**
     * @brief 遷移先を取得する。
     * @param from 遷移元
     * @param label 遷移ラベル
     * @return 遷移先
     */
    i32 edge_to(i32 from, i32 label) const { return edges[automaton_detail::edge_index<base>(n, from, label)]; }

    /**
     * @brief 遷移重みへの参照を取得する。
     * @param from 遷移元
     * @param label 遷移ラベル
     * @return 遷移重みへの参照
     */
    const Weight& edge_weight(i32 from, i32 label) const {
        return edge_weights[automaton_detail::edge_index<base>(n, from, label)];
    }

    /**
     * @brief 内部表現が重み付きオートマトンの不変条件を満たすか判定する。
     * @return 不変条件を満たす場合は `true`
     */
    [[nodiscard]] bool valid() const noexcept {
        if (n < 0 || condition_count < 0 || condition_count > 64) return false;
        const usize expected = automaton_detail::edge_count<base>(n);
        if (condition.size() != static_cast<usize>(n) || edges.size() != expected || edge_weights.size() != expected) {
            return false;
        }
        for (const auto& entry : init) {
            if (entry.first < 0 || entry.first >= n) return false;
        }
        if (condition_count < 64) {
            for (u64 mask : condition) {
                if ((mask >> condition_count) != 0) return false;
            }
        }
        for (i32 to : edges) {
            if (to != -1 && (to < 0 || to >= n)) return false;
        }
        return true;
    }

    /**
     * @brief 初期状態から到達不可能な状態を削除し縮約する
     */
    void trim() {
        assert(valid());
        std::vector<i32> q;
        std::vector<i32> id(n, -1);
        for (const auto& [u, w] : init) {
            if (id[u] == -1) {
                id[u] = q.size();
                q.push_back(u);
            }
        }
        for (usize head = 0; head < q.size(); ++head) {
            const i32 u = q[head];
            for (i32 x = 0; x < base; ++x) {
                const i32 v = edge_to(u, x);
                if (v != -1 && id[v] == -1) {
                    id[v] = q.size();
                    q.push_back(v);
                }
            }
        }

        const i32 new_n = static_cast<i32>(q.size());
        std::vector<i32> new_edges(automaton_detail::edge_count<base>(new_n), -1);
        std::vector<Weight> new_edge_weights(automaton_detail::edge_count<base>(new_n), WeightMonoid::e());
        for (i32 i = 0; i < new_n; ++i) {
            const i32 u = q[static_cast<usize>(i)];
            for (i32 x = 0; x < base; ++x) {
                const i32 v = edge_to(u, x);
                if (v != -1) {
                    const usize new_pos = automaton_detail::edge_index<base>(new_n, i, x);
                    new_edges[new_pos] = id[v];
                    new_edge_weights[new_pos] = edge_weight(u, x);
                }
            }
        }

        std::vector<std::pair<i32, Weight>> new_init;
        for (const auto& [u, w] : init) {
            if (id[u] != -1) new_init.push_back({id[u], w});
        }

        std::vector<u64> new_condition(new_n, 0ULL);
        for (i32 i = 0; i < n; ++i) {
            if (id[i] != -1) {
                new_condition[id[i]] = condition[i];
            }
        }

        n = new_n;
        edges = std::move(new_edges);
        edge_weights = std::move(new_edge_weights);
        init = std::move(new_init);
        condition = std::move(new_condition);
        assert(valid());
    }

private:
    static i32 checked_condition_count(i32 count) {
        assert(0 <= count && count <= 64);
        return count;
    }
};

namespace automaton_detail {

inline u64 combine_condition(u64 lhs, i32 lhs_count, u64 rhs, i32 rhs_count) {
    assert(0 <= lhs_count && 0 <= rhs_count && lhs_count + rhs_count <= 64);
    if (lhs_count == 64) {
        assert(rhs_count == 0 && rhs == 0);
        return lhs;
    }
    return lhs | (rhs << lhs_count);
}

}  // namespace automaton_detail

/**
 * @brief WeightedAutomaton の直積を扱うモノイド
 * @tparam base 遷移アルファベットのサイズ
 * @tparam WeightMonoid 重みのモノイド
 */
template <i32 base, monoid WeightMonoid> struct weighted_automaton_monoid {
    using S = WeightedAutomaton<base, WeightMonoid>;
    using Weight = typename WeightMonoid::S;

    /**
     * @brief 直積を計算する
     * @details condition の bitmask は下位に a、上位に b
     * の条件を割り当てる。到達可能性にかかわらず全状態を構築し、`trim()` は自動実行しない。
     * @param a 1つ目のオートマトン
     * @param b 2つ目のオートマトン
     * @return S 直積オートマトン
     */
    static S op(const S& a, const S& b) {
        assert(a.valid() && b.valid());
        assert(a.condition_count + b.condition_count <= 64);
        S res(automaton_detail::checked_product_state_count(a.n, b.n), a.condition_count + b.condition_count);

        for (const auto& [u, wu] : a.init) {
            for (const auto& [v, wv] : b.init) {
                res.add_init(u * b.n + v, WeightMonoid::op(wu, wv));
            }
        }

        for (i32 i = 0; i < a.n; ++i) {
            for (i32 j = 0; j < b.n; ++j) {
                i32 k = i * b.n + j;
                res.set_condition(k, automaton_detail::combine_condition(a.condition[i], a.condition_count,
                                                                         b.condition[j], b.condition_count));

                for (i32 x = 0; x < base; ++x) {
                    const i32 na = a.edge_to(i, x);
                    const i32 nb = b.edge_to(j, x);
                    if (na != -1 && nb != -1) {
                        res.set_edge(k, x, na * b.n + nb, WeightMonoid::op(a.edge_weight(i, x), b.edge_weight(j, x)));
                    }
                }
            }
        }
        assert(res.valid());
        return res;
    }

    /**
     * @brief 単位元を返す
     * @return S 単位オートマトン
     */
    static S e() {
        S res(1, 0);  // 単位元なので condition_count は 0
        res.add_init(0, WeightMonoid::e());
        res.set_condition(0, 0ULL);
        for (i32 x = 0; x < base; ++x) {
            res.set_edge(0, x, 0, WeightMonoid::e());
        }
        return res;
    }
};

/**
 * @brief 直積のシンタックスシュガー
 * @tparam base 遷移アルファベットのサイズ
 * @tparam WeightMonoid 重みのモノイド
 * @param a 1つ目のオートマトン
 * @param b 2つ目のオートマトン
 * @return WeightedAutomaton<base, WeightMonoid> 直積オートマトン
 */
template <i32 base, monoid WeightMonoid>
WeightedAutomaton<base, WeightMonoid> operator*(const WeightedAutomaton<base, WeightMonoid>& a,
                                                const WeightedAutomaton<base, WeightMonoid>& b) {
    return weighted_automaton_monoid<base, WeightMonoid>::op(a, b);
}

/**
 * @brief 複数のオートマトンを合成する
 * @tparam base 遷移アルファベットのサイズ
 * @tparam WeightMonoid 重みのモノイド
 * @param automata 合成するオートマトンのリスト
 * @return WeightedAutomaton<base, WeightMonoid> 合成された1つのオートマトン
 */
template <i32 base, monoid WeightMonoid>
WeightedAutomaton<base, WeightMonoid> build_weighted_automaton(
    const std::vector<WeightedAutomaton<base, WeightMonoid>>& automata) {
    auto res = weighted_automaton_monoid<base, WeightMonoid>::e();
    for (const auto& a : automata) {
        res = weighted_automaton_monoid<base, WeightMonoid>::op(res, a);
    }
    return res;
}

}  // namespace gwen
