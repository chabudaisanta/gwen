# SortedTreap Iterator Addition Task

## 概要
`SortedTreap`, `ProdSortedTreap`, `LazySortedTreap` に `parent` ポインタを持たせ、`std::set` のように扱える双方向イテレータ (`iterator`) を実装する。既存の `lower_bound`, `upper_bound` はこの `iterator` を返すように変更する。

## クラス・メソッド名とConcept
- クラス: `SortedTreap`, `ProdSortedTreap`, `LazySortedTreap` を修正。内部クラス `iterator` を追加。
- メソッド名: `begin`, `end`, `lower_bound`, `upper_bound` 等、STLに準拠。
- イテレータ無効化条件: `insert`, `erase` 等、木構造を変更する操作を行うと、以前取得したイテレータは直ちに無効になることをドキュメントに厳密に明記する。
- Concept: 新規の Concept 追加はなし。
- 型制約・エイリアス: プリミティブな型ではなく `gwen/types.hpp` の `i32`, `usize` 等を厳守する。
- Doxygenコメント: 新規追加する `iterator` クラスや `begin`, `end`, `lower_bound`, `upper_bound` などのすべての公開APIに `@brief` 等のDoxygen形式のコメントを付与する。

## 実装の詳細
- `node` 構造体に `tree parent = NIL;` を追加。
- `update(tree t)` で左右の子の `parent` を自身に更新。
- `split`, `merge` で分離される木のルートの `parent` を `NIL` に設定。
- `iterator` は `std::bidirectional_iterator_tag` を満たし、内部に `tree id` と `const Treap* tr` を持つ。

## ドキュメントとテストの計画
- テスト: `test/unit/bbst/` 以下の各テストファイルに、`iterator` の `++`, `--`, `begin()`, `end()`, `lower_bound()`, `upper_bound()` のテストを追加しパスさせる。
- Verifyコード: ユーザーが後ほど作成するため、今回は省略。
- ドキュメント: `doc/bbst/` 以下の `.md` にAC Libraryスタイル（メソッド名、制約、計算量を太字で記載）で `iterator` の挙動、計算量、および **イテレータが無効になるタイミング（構造変更時）** を厳密に記載する。
