# Weighted Automaton Factory 追加仕様書

## 目的
通常の `Automaton` 向けに実装されている `factory.hpp` の関数を `WeightedAutomaton` でも利用できるように、`weighted_automaton_factory.hpp` に追加実装する。

## 実装する関数
以下の関数を `template <i32 base, monoid WeightMonoid>` 形式で追加する。
1. `digit_sum_exact(i32 M)`
2. `digit_sum_leq(i32 M)`
3. `digit_sum_mod(i32 M, i32 K)`
4. `exclude_digits(std::span<const i32> S)`
5. `include_all_digits(std::span<const i32> S)`
6. `value_mod(i32 M, i32 K)`
7. `non_zero_count_exact(i32 M)`
8. `non_zero_count_leq(i32 M)`

## アルゴリズム
既存の `factory.hpp` と全く同じ状態空間および遷移を用いる。
遷移の重みは `WeightMonoid::e()` に設定する。
元の `Automaton` における `accept` 状態に対して、`condition` に `1ULL` を設定する。

## テスト
`test/unit/automaton/weighted_automaton_factory_test.cpp` に対応するテストを追加する。既存の `factory_test.cpp` のアサーションを参考にし、`condition` の値を確認するテストを記述する。
