# Automaton Factory 実装計画

## 目的
桁DPで頻出の制約を簡単に生成できるファクトリ群（ヘルパー関数）を提供する。これにより、ユーザーが毎回ゼロから `edges` をループで構築する手間を省き、`Monoid::op` での結合に集中できるようにする。

## 設置パス
- 実装: `include/gwen/automaton/factory.hpp`
- テスト: `test/unit/automaton/factory_test.cpp`

## 提案する生成関数の一覧

名前空間: `gwen::automaton` 内

1. **`build_digit_sum_exact(i32 M)`**
   - 制約: 各位の数字の和がちょうど `M` になる。
   - 状態数: `M + 1` (和が `M` を超えたら trap 状態へ)
2. **`build_digit_sum_mod(i32 M, i32 K)`**
   - 制約: 各位の数字の和を `M` で割った余りが `K` になる。
   - 状態数: `M`
3. **`build_exclude_digits(const std::vector<i32>& S)`**
   - 制約: 集合 `S` に含まれる数字を一切使用しない。
   - 状態数: `2` (セーフ、アウト)
4. **`build_value_mod(i32 M, i32 K)`**
   - 制約: 数値そのものを `M` で割った余りが `K` になる。
   - 遷移: `(u * base + c) % M`
   - 状態数: `M`
5. **`build_non_zero_count_exact(i32 M)`**
   - 制約: `0` 以外の数字がちょうど `M` 個現れる (例: AtCoder ABC154 E)。
   - 状態数: `M + 1` (超えたら trap 状態へ)

※ 全て `template <ring T, i32 base = 10>` を取り、`Automaton<T, base>` を返す。

## 保留（または将来的な拡張）
- **特定のパターン (例 `"42"`) を含む/含まない**
  - KMPやAho-Corasickアルゴリズムでの遷移テーブル構築が必要になるため、今回は見送るか、別モジュールでの提供を検討。
- **増加的・減少的な数 (各桁が直前の桁以上の値を取る等)**
  - Leading Zero（まだ数が始まっていないかどうかの区別）の扱いが必要になるため、基本モジュールからは一旦外す。

## テスト計画
- 各ジェネレータで生成した Automaton を `digit_dp` に渡し、数万程度の小さな `N` に対する愚直ループ解と一致するかを `test/unit/automaton/factory_test.cpp` で検証する。
