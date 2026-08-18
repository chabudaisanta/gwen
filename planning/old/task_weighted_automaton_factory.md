# Weighted Automaton Factory 拡充 仕様書

## 目的
`WeightedAutomaton` 向けのファクトリ関数を拡充し、使用されている数字の種類の制約や、特定の文字列（数字列）を含むという制約を表すオートマトンを容易に生成できるようにする。

## 設計

### 追加するファイル
`include/gwen/automaton/weighted_automaton_factory.hpp` を新規作成する。

### 追加する関数

```cpp
#pragma once

#include <span>
#include <vector>
#include <bit>

#include "gwen/automaton/weighted_automaton.hpp"
#include "gwen/types.hpp"
#include "gwen/alge/monoid.hpp"

namespace gwen::automaton {

/**
 * @brief ちょうど k 種類の数字が使われるオートマトンを生成する
 * @tparam base N の進数
 * @tparam WeightMonoid 重みのモノイド
 * @param k 使用される数字の種類の目標数
 * @return WeightedAutomaton<base, WeightMonoid>
 */
template <i32 base, monoid WeightMonoid>
WeightedAutomaton<base, WeightMonoid> used_digits_count_exact(i32 k);

/**
 * @brief k 種類以下の数字が使われるオートマトンを生成する
 * @tparam base N の進数
 * @tparam WeightMonoid 重みのモノイド
 * @param k 使用される数字の種類の目標数
 * @return WeightedAutomaton<base, WeightMonoid>
 */
template <i32 base, monoid WeightMonoid>
WeightedAutomaton<base, WeightMonoid> used_digits_count_leq(i32 k);

/**
 * @brief 指定した文字列(数字列)を含むオートマトンを生成する
 * @tparam base N の進数
 * @tparam WeightMonoid 重みのモノイド
 * @param pattern 含まれるべき文字列(数字列)のリスト
 * @return WeightedAutomaton<base, WeightMonoid>
 */
template <i32 base, monoid WeightMonoid>
WeightedAutomaton<base, WeightMonoid> contains_pattern(std::span<const i32> pattern);

} // namespace gwen::automaton
```

### アルゴリズム詳細
- `used_digits_count_exact` / `leq`:
  状態数は `2^base`。状態は現在までに使用された数字の集合を bitmask で表す。
  遷移は `S -> S | (1 << c)`。
  条件は `std::popcount(S) == k` (または `<= k`) のときに bitmask 1 を設定する。
- `contains_pattern`:
  状態数は `|pattern| + 1`。KMP法の遷移オートマトンを用いて構成する。
  状態 `|pattern|` に到達した後は、どの文字を受け取っても `|pattern|` に留まるようにする。
  条件は状態 `|pattern|` のときに bitmask 1 を設定する。

### テストおよびVerifyコード作成計画
- **Unit Test (`test/unit/automaton/weighted_automaton_factory_test.cpp`)**:
  - `used_digits_count_exact`, `used_digits_count_leq` の状態数・遷移・条件が正しいかをテスト。
  - `contains_pattern` の KMP 遷移が正しいか、およびパターンが含まれるかどうかの条件が正しく設定されているかをテスト。
- **Verifyコード (`verify/dummy/weighted_automaton_factory_dummy.test.cpp`)**:
  - ダミーテストを作成し、コンパイルと実行が正常に行われることを確認。

### ドキュメント作成計画
- `doc/automaton/weighted_automaton_factory.md` を作成。
- `.verify-helper/docs/index.md` にリンクを追加。
