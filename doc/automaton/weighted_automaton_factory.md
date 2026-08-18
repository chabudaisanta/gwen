---
title: Weighted Automaton Factory
documentation_of: //include/gwen/automaton/weighted_automaton_factory.hpp
---

# Weighted Automaton Factory

`WeightedAutomaton` 向けのファクトリ関数を提供するモジュールです。桁DPなどで使用される数字の種類を制限したり、特定の文字列（数字列）を含むという制約を記述するための重み付きオートマトンを生成します。

## used_digits_count_exact

```cpp
template <gwen::i32 base, gwen::monoid WeightMonoid>
gwen::WeightedAutomaton<base, WeightMonoid> gwen::automaton::used_digits_count_exact(i32 k)
```

ちょうど `k` 種類の数字が使われることを管理する重み付きオートマトンを生成します。

- 状態数は $2^{\mathrm{base}}$ であり、各状態はこれまでに出現した数字の種類を bitmask として保持します。
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

- 状態数は $2^{\mathrm{base}}$ であり、各状態はこれまでに出現した数字の種類を bitmask として保持します。
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
