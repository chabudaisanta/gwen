# Digit DP for Automaton

## 目的
`gwen::Automaton` を用いて、上限 `N` 以下の `M` 進数の条件を満たすものの数（または重み和）を高速に計算する桁 DP を提供する。

## 設置パス
- 実装: `include/gwen/automaton/digit_dp.hpp`
- テスト: `test/unit/automaton/digit_dp_test.cpp`
- Verifyコード: `verify/automaton/digit_dp.test.cpp` (適当な桁DP問題)

## 設計・仕様
### 関数シグネチャ
```cpp
#pragma once

#include <vector>
#include <ranges>
#include <concepts>
#include "gwen/types.hpp"
#include "gwen/alge/ring.hpp"
#include "gwen/automaton/automaton.hpp"

namespace gwen {

/**
 * @brief オートマトン上の桁 DP を実行する
 * @tparam R 上限 N の各桁の値を表す Range (上位桁から順に格納されていること)
 * @tparam T 遷移コストを表す型 (ring)
 * @tparam base N の進数
 * @param N 上限の値 (各桁の値が配列などで渡される)
 * @param a 条件を表すオートマトン
 * @return T 条件を満たす N 以下の値の総和 (オートマトンの遷移コストの積の和)
 */
template <std::ranges::forward_range R, ring T, i32 base>
T run_digit_dp(const R& N, const Automaton<T, base>& a);

} // namespace gwen
```

### アルゴリズム詳細
- 状態を `dp_tight` (Nのprefixと完全に一致している状態) と `dp_loose` (N未満であることが確定している状態) の2つの配列（サイズ `n_states`）で管理する。
- 遷移:
  各桁 `x` (`N` の要素) について、
  - `dp_loose` からの遷移: 任意の `0 <= c < base` について、`next_loose[v] += dp_loose[u] * cost`
  - `dp_tight` からの遷移:
    - `0 <= c < x` について、`next_loose[v] += dp_tight[u] * cost`
    - `c == x` について、`next_tight[v] += dp_tight[u] * cost`
- 高速化:
  - 内側のループで配列アクセスを減らすため、`const auto* edges_u = &a.edges[u * base];` を用いて連続アクセスを最適化。
  - 値が `T(0)` である状態からの遷移は枝刈りする (`if (val != T(0))`)。
- 初期化:
  - 初期状態 (`a.init` に含まれる状態) の `dp_tight` を `T(1)` とする。
- 結果の集計:
  - `a.accept` に含まれる受理状態について、最終的な `dp_tight[s] + dp_loose[s]` の和を返す。

## 制約
- `N` の要素は `0 <= c < base` を満たすと仮定する。
- Leading zeros の扱いについては、本関数では「与えられた長さ（`N` の桁数）に満たない数は 0 埋めされている」ものとしてオートマトンに文字 `0` が流し込まれる。
  （特別な leading zero の処理が必要な場合は、オートマトンの状態側で吸収することを想定）

## テスト計画
1. **Google Test**:
   - 単純な制約（例: `3` が含まれない等）で小さい `N` に対して愚直解と一致するかをテスト。
2. **Verify**:
   - AtCoder または AOJ の適当な桁 DP 系の問題 (例: abc029_d, tdpc_e など) に対して本関数を利用した Verify コードを作成。
