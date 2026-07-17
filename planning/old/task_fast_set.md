# FastSet (64分木) 仕様書

## 概要
非負整数の集合を管理するデータ構造。
64-ary tree を用い、要素の追加・削除・検索・前後の要素探索をビット演算で高速に行う。
計算量: $O(\log_{64} N)$

## 配置先
- 本体: `include/gwen/ds/fast_set.hpp`
- 単体テスト: `test/unit/ds/fast_set_test.cpp`
- ドキュメント: `doc/fast_set.md`

## クラス・メソッド名 (Naming Conventions)
- クラス名: `FastSet` (PascalCase)
- メソッド名: (snake_case)
  - `FastSet(i32 n)`
  - `void insert(i32 x)`
  - `void erase(i32 x)`
  - `bool contains(i32 x) const`
  - `i32 next(i32 x) const`
  - `i32 prev(i32 x) const`

## 設計詳細
- **型エイリアス**: `gwen::i32`, `gwen::u64`, `gwen::usize` を使用する (`gwen/types.hpp` インクルード)。
- **内部構造**: `std::vector<std::vector<u64>>` 等の多段配列。
  - 最下段が実際の要素の有無を示し、上段はその64個分を1ビットに要約したもの。
  - `next` / `prev` の無限ループを防ぐため、範囲外アクセスのケアや番兵の配置を行う。
- **ビット操作**: `<bit>` ヘッダの `std::countr_zero`, `std::countl_zero` 等を使用し、高速化を図る。
- **Concept / 制約**: テンプレートではないため型制約は特に不要だが、ルールに従い必要に応じて Concept を使う。
- **コメント**: 全ての public API に対して Doxygen 形式のコメント (`@brief`, `@param` 等) を付与する。

## テスト計画
- **Unit Test**: Google Test を使用 (`test/unit/ds/fast_set_test.cpp`)。
  - 基本操作の確認 (insert, erase, contains)
  - `next` / `prev` の境界値テスト
  - 空集合に対する `next` / `prev`
  - 大きなサイズ ($n = 10^6$ 程度) での動作確認
  - `std::set` との挙動一致を確かめるランダムテスト
- **Verify**: Library Checker の `predecessor_problem` を対象に、`verify/yosupo/predecessor_problem.test.cpp` を作成する。

## ドキュメント
- `doc/fast_set.md` を AC Library スタイルで作成 (Frontmatter に title 等を設定、メソッドごとに 制約・計算量 を明記)。
- `.verify-helper/docs/index.md` へ目次リンクを追加。
