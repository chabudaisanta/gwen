# 重み付きオートマトン (Weighted Automaton) 仕様書

## 目的
遷移や受理状態に重みを持たせることができる `WeightedAutomaton` を実装する。これにより、特殊な桁DPなどでの重み付き遷移を扱うことが可能になる。

## 設計

### クラス構造

`include/gwen/automaton/weighted_automaton.hpp` を新規作成する。

```cpp
#pragma once

#include <vector>
#include <utility>
#include "gwen/types.hpp"

namespace gwen {

template <i32 base, monoid WeightMonoid>
struct WeightedAutomaton {
    using Weight = typename WeightMonoid::S;

    i32 n;
    i32 condition_count; // このオートマトンが持つ条件の数
    std::vector<std::pair<i32, Weight>> init;
    std::vector<u64> condition; // 達成状況の bitmask
    std::vector<i32> edges;
    std::vector<Weight> edge_weights;

    /**
     * @brief デフォルトコンストラクタ
     */
    WeightedAutomaton() : WeightedAutomaton(0) {}

    /**
     * @brief 状態数を指定して初期化
     * @param n 状態数
     * @param condition_count 条件の数 (デフォルト1)
     */
    WeightedAutomaton(i32 n, i32 condition_count = 1) 
        : n(n), condition_count(condition_count), condition(n, 0ULL), edges(n * base, -1), edge_weights(n * base, WeightMonoid::e()) {
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
    void trim();
};

/**
 * @brief WeightedAutomaton の直積を扱うモノイド
 * @tparam base 遷移アルファベットのサイズ
 * @tparam WeightMonoid 重みのモノイド
 */
template <i32 base, monoid WeightMonoid>
struct weighted_automaton_monoid {
    using S = WeightedAutomaton<base, WeightMonoid>;

    /**
     * @brief 直積を計算する
     * @details condition の bitmask は下位に a、上位に b の条件を割り当てるように (b.condition[j] << a.condition_count) | a.condition[i] で合成される。
     */
    static S op(const S& a, const S& b) {
        assert(a.condition_count + b.condition_count <= 64);
        // 直積のロジック (重みは WeightMonoid::op(a.w, b.w) で合成)
        // res.condition_count = a.condition_count + b.condition_count;
    }

    /**
     * @brief 単位元を返す
     */
    static S e() {
        // 単位元
    }
};

/**
 * @brief 直積のシンタックスシュガー
 */
template <i32 base, monoid WeightMonoid>
WeightedAutomaton<base, WeightMonoid> operator*(const WeightedAutomaton<base, WeightMonoid>& a, const WeightedAutomaton<base, WeightMonoid>& b) {
    return weighted_automaton_monoid<base, WeightMonoid>::op(a, b);
}

/**
 * @brief 複数のオートマトンを合成する
 * @param automata 合成するオートマトンのリスト
 * @return 合成された1つのオートマトン
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
```

### 命名規則・制約
- **クラス・構造体**: データ構造として `WeightedAutomaton` (PascalCase)、代数構造の定義として `weighted_automaton_monoid` (snake_case)。
- **メソッド・変数**: `add_init`, `set_edge`, `trim`, `is_accept` などの snake_case。
- **Doxygenコメント**: 全ての公開APIに対して `@brief`, `@tparam`, `@param`, `@return` 等のコメントを記述。

### テストおよびVerifyコード作成計画
- **Unit Test (`test/unit/automaton/weighted_automaton_test.cpp`)**:
  - `WeightedAutomaton` の各メソッド（`add_init`, `set_edge`, `trim` 等）の動作確認。
  - `weighted_automaton_monoid::op` による直積計算（状態数、遷移、重みの合成）のテスト。
  - `weighted_automaton_monoid::e` の単位元が直積で正しく振る舞うかのテスト。
- **Verifyコード (`verify/dummy/weighted_automaton_dummy.test.cpp`)**:
  - ダミーテストを作成し、既存のテストと同様の形式でVerifyを通すことを確認。

### ドキュメント作成計画
- `doc/automaton/weighted_automaton.md` を作成。
- AC Library スタイル（メソッド名、制約、計算量を太字で記載）で記述。
- `.verify-helper/docs/index.md` の目次にリンクを追加。
