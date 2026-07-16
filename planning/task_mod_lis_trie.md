# LIS と Trie の実装仕様書

## 概要
`master` ブランチの `misc/lis.hpp` と `container/trie.hpp` を現行プロジェクトの規約に沿って移植します。
この作業が完了後、現在のブランチ (`rebuild-v2`) を用いて `master` を上書きする予定です。

## 配置
- `lis` ヘッダファイル: `include/gwen/algo/lis.hpp` (`misc` から `algo` へ移動)
- `trie` ヘッダファイル: `include/gwen/container/trie.hpp`
- テストファイル: `test/unit/algo_lis_test.cpp`, `test/unit/container_trie_test.cpp`
- Verifyコード: 
  - LIS: `verify/yosupo/dp/longest_increasing_subsequence.test.cpp` (該当する問題が存在しない場合は AOJ などの同等のものをユニットテストで代替) -> Yosupo Judge にはない可能性があるので要確認。なければテストのみ。
  - Trie: 該当する Yosupo Judge 問題がなければユニットテストのみで担保。
- ドキュメント: `doc/algo/lis.md`, `doc/container/trie.md`

## 設計方針

### LIS (algo/lis.hpp)
- 関数名の snake_case 徹底:
  - `LIS` -> `lis`
  - `LIS_strict` -> `lis_strict`
- 型制約: `template <std::totally_ordered T>` などを用いて順序比較可能な型に制約する（C++23 `concept`）。
- 内部変数の型を `i32` など `gwen/types.hpp` のものへ統一。

### Trie (container/trie.hpp)
- `gwen/utils/node_pool.hpp` への依存パス修正（`misc` -> `utils`）。
- 構造体名の PascalCase 化:
  - `trie_node_minimum` -> `TrieNodeMinimum`
  - `trie_base` -> `TrieBase`
- 型エイリアスの変更:
  - 旧 `trie` (a-z) -> `LowerAlphaTrie`
  - 旧 `TRIE` (A-Z) -> `UpperAlphaTrie`
  - 旧 `Trie` (A-Z, a-z) -> `AlphaTrie` (Offsetなど見直しが必要なら直す。旧実装は 'A' をオフセットとして 52文字分確保していたが、Zとaの間に不要な隙間がある。もし当時の通りで良いなら仕様を維持するか、あるいは厳密に直すか。今回は `master` の仕様互換を優先する)。
- メソッド名: `find`, `count`, `contains`, `match_count`, `get_node`, `insert`, `erase`, `path` は snake_case 維持。
- Doxygen コメントの追加。

## ワークフローの進行
この仕様案について `code_reviewer` エージェントに事前レビューを依頼し、その後にユーザーへ承認を求めます。
