---
title: Weighted Digit DP
documentation_of: //include/gwen/automaton/weighted_digit_dp.hpp
---

# Weighted Digit DP

`WeightedAutomaton` を用いて、重み付きの遷移や複雑な条件（ビットマスク）を扱う桁DPを実行するモジュールです。

## run_weighted_digit_dp

```cpp
template <
    weighted_digit_dp_value T,
    gwen::i32 base, 
    gwen::monoid WeightMonoid, 
    typename F,
    typename OpAct = std::multiplies<>
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

既存のオーバーロードでは、初期状態 `(s, w)` のDP値を `static_cast<T>(w)` で初期化します。この挙動は後方互換性のため維持されています。

**既存オーバーロード固有の制約**

- 初期状態の重み`w`を`static_cast<T>(w)`で`T`へ変換できる。

### 初期DP値を指定するオーバーロード

```cpp
template <weighted_digit_dp_value T, i32 base, monoid WeightMonoid,
          typename F, typename OpAct>
T run_weighted_digit_dp(
    const std::vector<i32>& upper_bound,
    const WeightedAutomaton<base, WeightMonoid>& a,
    const T& initial_value,
    F f,
    OpAct op_act
)
```

各初期状態 `(s, w)` を `op_act(initial_value, w)` で初期化します。重み付きファクトリが設定する単位重みから、経路数などの初期値を明示的に構築する場合に使用します。

**共通制約**

- `T`は`weighted_digit_dp_value`を満たし、`T(0)`、コピー構築・代入、`+=`、等値比較を提供する必要があります。
- `f`は`T(const T& dp_val, u64 condition_mask)`と互換性のあるシグネチャを持つ必要があります。
- `op_act` の戻り値は `T` へ変換可能である必要があります。
- `op_act(x, WeightMonoid::e()) == x` を満たす必要があります。
- `op_act(op_act(x, a), b) == op_act(x, WeightMonoid::op(a, b))` を満たす必要があります。
- `op_act` はDP値の加算に対して分配可能である必要があります。
- `upper_bound`の各桁は`0 <= digit < base`を満たす必要があります。
- `a.valid()`が`true`である必要があります。

**計算量**

- $O(|upper\_bound| \times a.n \times base)$
