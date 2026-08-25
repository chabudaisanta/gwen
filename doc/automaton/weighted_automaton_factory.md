---
title: Weighted Automaton Factory
documentation_of: //include/gwen/automaton/weighted_automaton_factory.hpp
---

# Weighted Automaton Factory

`WeightedAutomaton` 向けのファクトリ関数を提供するモジュールです。桁DPなどで使用される数字の種類を制限したり、特定の文字列（数字列）を含むという制約を記述するための重み付きオートマトンを生成します。

遷移構造と条件は非重み付きファクトリと共通です。初期重みと有効な遷移重みには`WeightMonoid::e()`を設定します。各関数の引数制約、leading zero、不正な`k`、無効digitの扱いは[非重み付きファクトリ](factory.md)と同じです。

## used_digits_count_exact

```cpp
template <gwen::i32 base, gwen::monoid WeightMonoid>
gwen::WeightedAutomaton<base, WeightMonoid> gwen::automaton::used_digits_count_exact(i32 k)
```

ちょうど `k` 種類の数字が使われることを管理する重み付きオートマトンを生成します。

- 状態数は $2^{\mathrm{base}} + 1$ であり、bitmask状態に加えてleading-zero状態を持ちます。
- 制約 `k` 種類を正確に満たす状態に対してのみ、`condition` が `1` (bit 0 が立つ) となります。
- 全ての遷移の重みおよび初期状態の重みは `WeightMonoid::e()` となります。

**計算量**
- $O(2^{\mathrm{base}} \times \mathrm{base})$

## used_digits_count_leq

```cpp
template <gwen::i32 base, gwen::monoid WeightMonoid>
gwen::WeightedAutomaton<base, WeightMonoid> gwen::automaton::used_digits_count_leq(i32 k)
```

`k` 種類以下の数字が使われることを管理する重み付きオートマトンを生成します。

- 状態数は $2^{\mathrm{base}} + 1$ であり、bitmask状態に加えてleading-zero状態を持ちます。
- 制約（出現した種類の数が `k` 以下）を満たす状態に対してのみ、`condition` が `1` (bit 0 が立つ) となります。
- 全ての遷移の重みおよび初期状態の重みは `WeightMonoid::e()` となります。

**計算量**
- $O(2^{\mathrm{base}} \times \mathrm{base})$

## contains_pattern

```cpp
template <gwen::i32 base, gwen::monoid WeightMonoid>
gwen::WeightedAutomaton<base, WeightMonoid> gwen::automaton::contains_pattern(std::span<const i32> pattern)
```

指定した文字列(数字列) `pattern` を含むことを管理する重み付きオートマトンを生成します。

- 状態数は $|pattern| + 1$ であり、KMPオートマトンに基づく遷移を持ちます。
- 状態 $|pattern|$ に到達すると、その後はどの文字を受け取っても状態 $|pattern|$ に留まります (受理状態でのループ)。
- 指定されたパターンを含んだことを示す状態 $|pattern|$ に対してのみ、`condition` が `1` (bit 0 が立つ) となります。
- 全ての遷移の重みおよび初期状態の重みは `WeightMonoid::e()` となります。

**計算量**
- $O(|pattern| \times \mathrm{base})$

## digit_sum_exact

```cpp
template <gwen::i32 base, gwen::monoid WeightMonoid>
gwen::WeightedAutomaton<base, WeightMonoid> gwen::automaton::digit_sum_exact(i32 M)
```

各位の数字の和がちょうど `M` になるオートマトンを生成します。
`condition` は `M` に到達した状態に対してのみ `1` が設定されます。

## digit_sum_leq

```cpp
template <gwen::i32 base, gwen::monoid WeightMonoid>
gwen::WeightedAutomaton<base, WeightMonoid> gwen::automaton::digit_sum_leq(i32 M)
```

各位の数字の和が `M` 以下になるオートマトンを生成します。

## digit_sum_mod

```cpp
template <gwen::i32 base, gwen::monoid WeightMonoid>
gwen::WeightedAutomaton<base, WeightMonoid> gwen::automaton::digit_sum_mod(i32 M, i32 K)
```

各位の数字の和を `M` で割った余りが `K` になるオートマトンを生成します。

## exclude_digits

```cpp
template <gwen::i32 base, gwen::monoid WeightMonoid>
gwen::WeightedAutomaton<base, WeightMonoid> gwen::automaton::exclude_digits(std::span<const i32> S)
```

指定した文字を含まないオートマトンを生成します。

## include_all_digits

```cpp
template <gwen::i32 base, gwen::monoid WeightMonoid>
gwen::WeightedAutomaton<base, WeightMonoid> gwen::automaton::include_all_digits(std::span<const i32> S)
```

指定した文字をすべて含むオートマトンを生成します。

## value_mod

```cpp
template <gwen::i32 base, gwen::monoid WeightMonoid>
gwen::WeightedAutomaton<base, WeightMonoid> gwen::automaton::value_mod(i32 M, i32 K)
```

数値そのものを `M` で割った余りが `K` になるオートマトンを生成します。

## non_zero_count_exact

```cpp
template <gwen::i32 base, gwen::monoid WeightMonoid>
gwen::WeightedAutomaton<base, WeightMonoid> gwen::automaton::non_zero_count_exact(i32 M)
```

0 以外の数字がちょうど `M` 個現れるオートマトンを生成します。

## non_zero_count_leq

```cpp
template <gwen::i32 base, gwen::monoid WeightMonoid>
gwen::WeightedAutomaton<base, WeightMonoid> gwen::automaton::non_zero_count_leq(i32 M)
```

0 以外の数字が `M` 個以下現れるオートマトンを生成します。
