---
title: Digit DP on Automaton
documentation_of: //include/gwen/automaton/digit_dp.hpp
---

## 概要

`gwen::Automaton` 上で桁 DP を実行し、上限 $N$ 以下の数のうち、オートマトンで受理される状態に至る経路のコストの重み和（あるいはパターンの総数）を計算します。
桁 DP の状態として「$N$ 未満であることが確定している状態 (loose)」と「$N$ のプレフィックスと完全に一致している状態 (tight)」の 2 つを持ち、動的計画法により高速に集計します。

## 制約

- オートマトンの遷移コスト型 `T` は `gwen::ring` を満たす必要があります。
- 各桁の値 $c$ は $0 \le c < \text{base}$ を満たしている必要があります。

## 関数

### run_digit_dp

```cpp
template <ring T, i32 base>
T run_digit_dp(const std::vector<i32>& N, const Automaton<T, base>& a)
```

- **時間計算量**
  - $O(|N| \times \text{n\_states} \times \text{base})$
  - $|N|$ は配列 `N` の要素数（桁数）
  - `n_states` はオートマトンの状態数

#### 引数
- `N`: 上限の値を表す配列。最上位の桁から順に格納されています（例: `N = {1, 2, 3}` は 10進数であれば $123$ を表します）。
- `a`: 条件を表すオートマトン。

#### 戻り値
- 上限 $N$ 以下の値で、オートマトン `a` の初期状態から受理状態に到達するすべての経路の遷移コストの積の総和（`T` 型）。
