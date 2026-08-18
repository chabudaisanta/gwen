# 重み付き桁DP (Weighted Digit DP) 仕様書

## 目的
`WeightedAutomaton` を用いた桁DPモジュール `weighted_digit_dp.hpp` を実装する。各状態での重み遷移と、最終的な条件達成状況（bitmask）を用いた集約をサポートし、複雑な制約やコスト計算が伴う桁DPを汎用的に解けるようにする。

## 設計

### クラス・関数構造

`include/gwen/automaton/weighted_digit_dp.hpp` を新規作成する。

```cpp
#pragma once

#include <algorithm>
#include <utility>
#include <vector>

#include "gwen/alge/ring.hpp"
#include "gwen/automaton/weighted_automaton.hpp"
#include "gwen/types.hpp"

namespace gwen {

/**
 * @brief 重み付きオートマトン上の桁 DP を実行する
 * @tparam T DPで保持する値の型（加法 `+` が定義されていること）
 * @tparam base n の進数
 * @tparam WeightMonoid オートマトンの重みのモノイド
 * @tparam F 最終的な値の集約関数の型。シグネチャは `T(T dp_val, u64 condition_mask)` を想定。
 * @tparam OpAct 遷移重みをDP値に作用させる関数の型。シグネチャは `T(T dp_val, Weight w)` を想定。
 * @param upper_bound 上限の値 (各桁の値が上位から順に格納されている配列)
 * @param a 条件と重み遷移を表すオートマトン
 * @param f 最終的な集約関数。各状態について得られたDP値と、その状態の達成条件マスクを受け取り、最終的なスコアを返す。
 * @param op_act DP値に対して遷移の重みを作用させる関数。デフォルトは乗算を想定。
 * @return T 集約された総和
 */
template <typename T, i32 base, monoid WeightMonoid, 
          std::invocable<T, u64> F, 
          std::invocable<T, typename WeightMonoid::S> OpAct = std::multiplies<>>
T run_weighted_digit_dp(const std::vector<i32>& upper_bound, const WeightedAutomaton<base, WeightMonoid>& a, F f, OpAct op_act = OpAct{}) {
    using Weight = typename WeightMonoid::S;
    
    std::vector<T> dp_tight(a.n, T(0)); // T はゼロ初期化可能とする
    std::vector<T> dp_loose(a.n, T(0));
    std::vector<T> next_tight(a.n, T(0));
    std::vector<T> next_loose(a.n, T(0));

    // 初期状態のセットアップ
    for (const auto& [s, w] : a.init) {
        dp_tight[s] += static_cast<T>(w);
    }

    for (i32 x : upper_bound) {
        std::fill(next_tight.begin(), next_tight.end(), T(0));
        std::fill(next_loose.begin(), next_loose.end(), T(0));

        for (i32 u = 0; u < a.n; ++u) {
            const T loose_val = dp_loose[u];
            const T tight_val = dp_tight[u];

            if (loose_val != T(0)) {
                for (i32 c = 0; c < base; ++c) {
                    auto [v, w] = a.edge(u, c);
                    if (v != -1) {
                        next_loose[v] += op_act(loose_val, w);
                    }
                }
            }

            if (tight_val != T(0)) {
                for (i32 c = 0; c < x; ++c) {
                    auto [v, w] = a.edge(u, c);
                    if (v != -1) {
                        next_loose[v] += op_act(tight_val, w);
                    }
                }
                if (x < base) {
                    auto [v, w] = a.edge(u, x);
                    if (v != -1) {
                        next_tight[v] += op_act(tight_val, w);
                    }
                }
            }
        }

        std::swap(dp_tight, next_tight);
        std::swap(dp_loose, next_loose);
    }

    T ans = T(0);
    for (i32 u = 0; u < a.n; ++u) {
        ans += f(dp_tight[u] + dp_loose[u], a.condition[u]);
    }
    return ans;
}

} // namespace gwen
```

### 検討事項（ユーザーへの確認ポイント）
1. **初期値の設定**: `a.init` にある重み `w` を `dp_tight` に足し込む際、`static_cast<T>(w)` としていますが、これで問題ないでしょうか？（`T` は通常 `mint` などを想定）
2. **遷移の重み作用**: `OpAct` 関数を通じて `dp_val` に `Weight` を作用させる設計にしました。デフォルトは `*` 演算子 (`std::multiplies<>`) を使います。

### テストおよびVerifyコード作成計画
- **Unit Test (`test/unit/automaton/weighted_digit_dp_test.cpp`)**:
  - `run_weighted_digit_dp` の基本動作確認。
- **Verifyコード (`verify/dummy/weighted_digit_dp_dummy.test.cpp`)**:
  - ダミーテストの追加。

### ドキュメント作成計画
- `doc/automaton/weighted_digit_dp.md` を作成し、AC Library スタイルで記述。
- `index.md` へリンクを追加。
