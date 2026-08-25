---
title: Digit DP on Automaton
documentation_of: //include/gwen/automaton/digit_dp.hpp
---

## 概要

`gwen::Automaton` 上で桁 DP を実行し、上限 $N$ 以下の数のうち、オートマトンで受理される状態に至るパターンの総数を計算します。
桁 DP の状態として「$N$ 未満であることが確定している状態 (loose)」と「$N$ のプレフィックスと完全に一致している状態 (tight)」の 2 つを持ち、動的計画法により高速に集計します。

## 制約

- 結果の集計に用いる型 `T` は `gwen::digit_dp_value` を満たす必要があります。
- `T` は `T(0)`、`T(1)`、コピー、`+=`、等値比較を提供する必要があります。
- 各桁の値 $c$ は $0 \le c < \text{base}$ を満たしている必要があります。
- `a.valid()` が `true` である必要があります。

## 関数

### run_digit_dp

```cpp
template <digit_dp_value T, i32 base>
T run_digit_dp(const std::vector<i32>& N, const Automaton<base>& a)
```

**計算量**
- $ O(|N| \times \text{n} \times \text{base}) $
- $ |N| $ は配列 `N` の要素数（桁数）
- `n` はオートマトンの状態数

**引数**
- `N`: 上限の値を表す配列。最上位の桁から順に格納されています（例: `N = {1, 2, 3}` は 10進数であれば $123$ を表します）。
- `a`: 条件を表すオートマトン。

**戻り値**
- 上限 $N$ 以下の値で、オートマトン `a` の初期状態から受理状態に到達する条件を満たす数の総和（`T` 型）。

`init` または `accept` に同じ状態が複数回含まれる場合、その回数を多重度として結果へ加算します。入力は常に `N` と同じ桁数の固定長数字列として処理され、先頭ゼロもオートマトンへ入力されます。
