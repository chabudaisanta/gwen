---
title: Weighted Digit DP
documentation_of: //include/gwen/automaton/weighted_digit_dp.hpp
---

# Weighted Digit DP

`WeightedAutomaton` を用いて、重み付きの遷移や複雑な条件（ビットマスク）を扱う桁DPを実行するモジュールです。

## run_weighted_digit_dp

```cpp
template <
    typename T, 
    gwen::i32 base, 
    gwen::monoid WeightMonoid, 
    std::invocable<T, gwen::u64> F, 
    std::invocable<T, typename WeightMonoid::S> OpAct = std::multiplies<>
>
T gwen::run_weighted_digit_dp(
    const std::vector<i32>& upper_bound, 
    const WeightedAutomaton<base, WeightMonoid>& a, 
    F f, 
    OpAct op_act = OpAct{}
)
```

指定された進数 `base` で、各桁の上限が `upper_bound` で与えられる制約のもと、オートマトン `a` に従った桁DPを実行します。

- `upper_bound`: 数の上限を表す配列（上位桁から順に格納）。
- `a`: 桁の遷移や重みを管理する重み付きオートマトン。
- `f`: DPの最終結果を集約する関数。各状態に対して `f(dp_val, condition_mask)` が呼ばれ、その返り値の総和が全体の結果となります。
- `op_act`: 遷移時に、状態のDP値 `dp_val` に対して遷移の重み `w` を作用させる関数。デフォルトでは `std::multiplies<>` が用いられ、`dp_val * w` のように計算されます。

**制約**

- `T` は加算 `+` と `T(0)` によるゼロ初期化がサポートされている必要があります。
- オートマトン `a` の初期状態における重み `w` を `static_cast<T>(w)` として `T` 型に変換できる必要があります。
- `f` は `T(T dp_val, u64 condition_mask)` と互換性のあるシグネチャを持つ必要があります。

**計算量**

- $O(|upper\_bound| \times a.n \times base)$
