#pragma once

#include <vector>
#include <utility>
#include <cassert>
#include "gwen/types.hpp"
#include "gwen/alge/monoid.hpp"

namespace gwen {

/**
 * @brief 重み付きオートマトンを表す構造体
 * @tparam base 遷移アルファベットのサイズ
 * @tparam WeightMonoid 重みの演算を定義するモノイド
 */
template <i32 base, monoid WeightMonoid>
struct WeightedAutomaton {
    using Weight = typename WeightMonoid::S;

    i32 n;                           ///< 状態数
    i32 condition_count;             ///< このオートマトンが管理する条件の数
    std::vector<std::pair<i32, Weight>> init; ///< 初期状態と初期重みのリスト
    std::vector<u64> condition;      ///< 達成状況の bitmask
    std::vector<i32> edges;          ///< 遷移リスト
    std::vector<Weight> edge_weights;///< 遷移重みリスト

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
        : n(n), condition_count(condition_count), condition(n, 0ULL), 
          edges(n * base, -1), edge_weights(n * base, WeightMonoid::e()) {
        assert(condition_count >= 0 && condition_count <= 64);
    }

    /**
     * @brief 初期状態を追加する
     * @param i 状態
     * @param w 初期重み
     */
    void add_init(i32 i, Weight w) {
        init.push_back({i, w});
    }

    /**
     * @brief 状態の達成条件(bitmask)を設定する
     * @param i 状態
     * @param mask 達成状況の bitmask
     */
    void set_condition(i32 i, u64 mask) {
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
        edges[from * base + label] = to;
        edge_weights[from * base + label] = w;
    }

    /**
     * @brief 遷移を取得する
     * @param from 遷移元の状態
     * @param label 遷移ラベル
     * @return 遷移先の状態と重みのペア
     */
    std::pair<i32, Weight> edge(i32 from, i32 label) const {
        return {edges[from * base + label], edge_weights[from * base + label]};
    }

    /**
     * @brief 初期状態から到達不可能な状態を削除し縮約する
     */
    void trim() {
        std::vector<i32> q;
        std::vector<i32> id(n, -1);
        for (const auto& [u, w] : init) {
            if (id[u] == -1) {
                id[u] = q.size();
                q.push_back(u);
            }
        }
        for (i32 head = 0; head < static_cast<i32>(q.size()); ++head) {
            i32 u = q[head];
            for (i32 x = 0; x < base; ++x) {
                i32 v = edges[u * base + x];
                if (v != -1 && id[v] == -1) {
                    id[v] = q.size();
                    q.push_back(v);
                }
            }
        }

        i32 new_n = q.size();
        std::vector<i32> new_edges(new_n * base, -1);
        std::vector<Weight> new_edge_weights(new_n * base, WeightMonoid::e());
        for (i32 i = 0; i < new_n; ++i) {
            i32 u = q[i];
            for (i32 x = 0; x < base; ++x) {
                i32 v = edges[u * base + x];
                if (v != -1) {
                    new_edges[i * base + x] = id[v];
                    new_edge_weights[i * base + x] = edge_weights[u * base + x];
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
    }
};

/**
 * @brief WeightedAutomaton の直積を扱うモノイド
 * @tparam base 遷移アルファベットのサイズ
 * @tparam WeightMonoid 重みのモノイド
 */
template <i32 base, monoid WeightMonoid>
struct weighted_automaton_monoid {
    using S = WeightedAutomaton<base, WeightMonoid>;
    using Weight = typename WeightMonoid::S;

    /**
     * @brief 直積を計算する
     * @details condition の bitmask は下位に a、上位に b の条件を割り当てるように (b.condition[j] << a.condition_count) | a.condition[i] で合成される。
     * @param a 1つ目のオートマトン
     * @param b 2つ目のオートマトン
     * @return S 直積オートマトン
     */
    static S op(const S& a, const S& b) {
        assert(a.condition_count + b.condition_count <= 64);
        S res(a.n * b.n, a.condition_count + b.condition_count);

        for (const auto& [u, wu] : a.init) {
            for (const auto& [v, wv] : b.init) {
                res.add_init(u * b.n + v, WeightMonoid::op(wu, wv));
            }
        }

        for (i32 i = 0; i < a.n; ++i) {
            for (i32 j = 0; j < b.n; ++j) {
                i32 k = i * b.n + j;
                res.set_condition(k, (b.condition[j] << a.condition_count) | a.condition[i]);

                for (i32 x = 0; x < base; ++x) {
                    i32 na = a.edges[i * base + x];
                    i32 nb = b.edges[j * base + x];
                    if (na != -1 && nb != -1) {
                        res.set_edge(k, x, na * b.n + nb, WeightMonoid::op(a.edge_weights[i * base + x], b.edge_weights[j * base + x]));
                    }
                }
            }
        }
        return res;
    }

    /**
     * @brief 単位元を返す
     * @return S 単位オートマトン
     */
    static S e() {
        S res(1, 0); // 単位元なので condition_count は 0
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
WeightedAutomaton<base, WeightMonoid> operator*(const WeightedAutomaton<base, WeightMonoid>& a, const WeightedAutomaton<base, WeightMonoid>& b) {
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
WeightedAutomaton<base, WeightMonoid> build_weighted_automaton(const std::vector<WeightedAutomaton<base, WeightMonoid>>& automata) {
    auto res = weighted_automaton_monoid<base, WeightMonoid>::e();
    for (const auto& a : automata) {
        res = weighted_automaton_monoid<base, WeightMonoid>::op(res, a);
    }
    return res;
}

} // namespace gwen
