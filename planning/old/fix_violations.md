# リファクタリング計画 (規約違反の修正)

このファイルは直近の全リポジトリ検査により判明した規約違反のリストです。
別セッションでの修正対応に用います。

## 1. 命名規則の修正 (PascalCase化)
データを直接扱うデータ構造については `PascalCase` とするルールに基づき、以下の構造体をリファクタリングします。（※ `monoid` や `abel` 等の実体のない代数構造は `snake_case` のままで問題ありません）

- `include/gwen/ds/prefix_sum.hpp`:
  - `prefix_sum_2d` -> `PrefixSum2d`
- `include/gwen/hash/rolling_hash.hpp`:
  - `power_table` -> `PowerTable`
  - `rolling_hash` -> `RollingHash`
- `include/gwen/utils/xorshift.hpp`:
  - `xorshift` -> `XorShift`

## 2. 型エイリアス (gwen/types.hpp) の適用
プリミティブな型（`int`, `long long`, `size_t` 等）を `gwen/types.hpp` (`i32`, `i64`, `usize` 等) に置き換えます。

- `include/gwen/template.hpp` (`long long` 等の使用確認・調整)
- `include/gwen/ds/wavelet_matrix.hpp` (`size_t` -> `usize`)
- `include/gwen/core/bit_vector.hpp` (`size_t` -> `usize`)
- `include/gwen/hash/rolling_hash.hpp` (`size_t` -> `usize`)
- `include/gwen/algo/idxsort.hpp` (`int` -> `i32`)
- `include/gwen/graph/tree_lca.hpp` (`size_t` -> `usize`)
- `include/gwen/core/grid.hpp` (`int` -> `i32`)
- `include/gwen/core/macro.hpp` (`int` -> `i32`)
- `include/gwen/core/constants.hpp` (`int` -> `i32`)
- `include/gwen/math/matrix.hpp` (`size_t` -> `usize`)
- `include/gwen/utils/compress.hpp` (`size_t` -> `usize`)
- `include/gwen/mod/modint.hpp` (`unsigned long long` -> `u64`, `int` -> `i32`)

## 3. index.md への未登録ドキュメント追加
`.verify-helper/docs/index.md` に以下のファイルへのリンクを追加します。

- `doc/dump.md`
- `doc/types.md`
- `doc/ds/wavelet_matrix.md`
- `doc/ds/fenwick_tree.md`
- `doc/algo/lis.md`
- `doc/alge/abel.md`
- `doc/alge/monoid.md`

## 4. Doxygenコメントの追加
以下のファイル内の公開APIに `@brief` や `@tparam` などのDoxygenコメントを追加します。

- `include/gwen/template.hpp`
- `include/gwen/alge/abel.hpp`
- `include/gwen/alge/monoid.hpp`
- `include/gwen/mod/mod.hpp`

## 5. AC Library スタイルの適用 (ドキュメント)
以下のドキュメントにおいて、メソッド名、制約、計算量を太字 (`**...**`) に修正します。

- `prefix_sum.md`, `idxsort.md`, `runlength.md`, `edge.md`, `dijkstra.md`, `grid.md` など
- ※ 上記を含む計21ファイルのフォーマットを一斉に見直します。
