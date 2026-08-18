# Leading Zero 対応仕様書

## 目的
`used_digits_count_exact` および `used_digits_count_leq` において、数値の先頭の `0` (Leading Zero) が「数字の `0` を使用した」とカウントされないように修正する。

## 設計
状態数を `2^base` から `2^base + 1` に拡張する。
追加する1つの状態（インデックス `1 << base`）を **Leading Zero (LZ) 状態** とする。

### 遷移
- **初期状態**: LZ状態 (`1 << base`)
- **LZ状態からの遷移**:
  - `0` を受け取った場合: そのまま LZ状態へ遷移する。
  - `c > 0` を受け取った場合: 通常状態の `1 << c` へ遷移する。
- **通常状態 (`0` 〜 `2^base - 1`) からの遷移**:
  - `c` を受け取った場合: 既存の通り `u | (1 << c)` へ遷移する。

### 条件 (Condition)
- LZ状態: 使用した数字の種類数は `0` として扱うため、`k == 0` (または `<= k`) の場合に `condition` を `1` にする。
- 通常状態: 既存の通り、bitmask の popcount が `k` と等しい (または `k` 以下) 場合に `condition` を `1` にする。

## 修正対象の関数
- `used_digits_count_exact`
- `used_digits_count_leq`

## テストの修正
`test/unit/automaton/weighted_automaton_factory_test.cpp` の該当テストを更新し、Leading Zero が正しく処理されること（`0` を何度踏んでも使われた数字の種類が増えないこと、その後 `1` などを踏むと正しく遷移すること）を確認するアサーションを追加する。
