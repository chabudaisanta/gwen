# TODO

gwen ライブラリの開発・改善メモ。

---

## 現状

### 実装済み

- **algebra**（代数構造）: basic_monoid, basic_abel, range_affine_range_sum, rolling_hash_monoid
- **container**: fenwick_tree, persistent_stack, sq_div, segtree（ACL ラッパ）, trie, bit_vector, wavelet_matrix, disjoint_sparse_table, treap/sorted_treap/implicit_treap/associative_treap, wrapper/range_add_update_sum, wrapper/range_add_point_get, mdarray
- **graph**: edge, graph（graph_base）, csr, dijkstra, tree_lca, forest, weighted_dsu, xor_linked_tree, hld
- **hash**: rolling_hash, zobrist, to_hash
- **math**: binom, combination, matrix, prime, xor_basis
- **misc**: compress, counting_sort, lis, xorshift, node_pool
- **mod**: mod, mod61, modint（dynamic_modint64）
- **query**: mo, swag, functional_query
- **geo**: point, graham_scan

### 作りかけ・スタブ

- **segtree / lazysegtree**: `atcoder::segtree` のエイリアスのみ（ACL 依存）。独自実装は未着手（`segtree.hpp` 内 TODO）
- **csr**: 動作はするが `// 書き直す` コメントあり。`operator[]` に境界チェックなし

---

## バグ・ビルド不整合（優先度高）

| 箇所 | 内容 | 状態 |
|------|------|------|
| `verify/yosupo/jump_on_tree.cpp` | `cin` / `cout` ではなく `input` / `output` を使う | 修正済み |
| `verify/yosupo/primality_test.cpp` | 同上 | 修正済み |
| `doc/README.md` | ドキュメントパスは `doc/`（`src/doc/` 表記は誤り） | 修正済み |

---

## 品質・設計の改善

### ビルド・テスト環境

| 箇所 | 内容 | 状態 |
|------|------|------|
| ディレクトリ構成 | `src/gwen/` ➡ `include/gwen/` へ移行（標準的なヘッダーオンリー構成へ） | **完了** |
| 外部リファレンス | `src/nyaan/` ➡ `references/nyaan/` へ移動し、自作コードと分離 | **完了** |
| ビルド成果物 | `debug/` と `build/` を `.build/` に統合し、ルートを汚さないようクリーンアップ | **完了** |
| テスト環境 | Python (pytest) から C++ Google Test + CMake への移行 | 設計完了・実装待ち |
| .vscode | `.vscode/` フォルダを Git 追跡から外し、`.gitignore` で無視するように設定 | **完了** |

### graph

| 箇所 | 内容 |
|------|------|
| API 不統一 | `tree_lca` / `dijkstra` は `graph_base`、`forest` / `hld` は `vector<vector<i32>>`（または辺リスト→隣接リスト変換）。方針を揃えるか意図を doc に明記 |
| `tree_lca` | `graph_base<Edge, isDirected>` を受け付ける。有向グラフのまま木 LCA すると辺が片方向になり**誤動作しうる**（`NonDirectedGraph` 限定を検討） |
| メソッド名 | `tree_lca::lca` / `forest::get_lca` / `hld::get_lca` が不統一 |
| `dijkstra` | 到達不能は `numeric_limits::max()`。問題によっては `-1` への変換が必要。非負辺はコード上未検証 |
| `xor_tree_scan` / `tree_lca` | 辺集合が木でない・`root` が不正だと親配列が壊れる（assert 不足） |
| `csr` | `operator[](v)` の範囲外アクセス未チェック |

### io / verify

| 箇所 | 内容 |
|------|------|
| `io.hpp` | `i128` / `u128` 未対応（ファイル内 TODO）。`cin`/`cout` エイリアスなしで verify が古い書き方のまま |
| verify 全体 | 以前よりカバレッジが向上。サブディレクトリの verify ファイルもコンパイル確認を行うようにスクリプトを改善 |

### その他モジュール

| 箇所 | 内容 |
|------|------|
| `query/swag.hpp` | **未使用**の `#include "gwen/dump.hpp"`（本番ヘッダにデバッグ依存を混在） |
| `container/mdarray.hpp` | `#include <iostream>` をヘッダに含む（依存が重い） |
| `mod/modint.hpp` | `dynamic_modint64` は mod を **static 1 つ**で保持。別 mod を同時に使うと壊れる／スレッド非安全 |
| `math/combination.hpp` | `extend()` のたび階乗テーブルを先頭から再計算（拡張のたび O(n)） |
| `query/functional_query.hpp` | 関数グラフが DAG でない・`pos` にループがあると未定義動作 |
| `math/prime.hpp` | `factorize` / `miller` は `assert` 前提（`x > 1e8` など）。範囲外の扱いを doc と揃える |

### リポジトリ運用

- `csr.hpp` に未コミットのフォーマット差分が残っている可能性あり（意図しないなら `git restore`）

---

## 追加したいもの

### algebra

- （代数構造は現状で十分）

### algorithm

- Convex Hull Trick
- Li-Chao Tree
- Monotone Minima
- SMAWK

### container

- binary trie
- segtree beats（ACL 非依存 of segtree 本体もここに含む）
- wordsize-tree (64分木)
- ordered multiset（重複対応 order statistics）

### graph

- 木の直径
- LCA（Euler Tour + RMQ）— `tree_lca` は binary lifting 済み
- ~~最短路（dijkstra）~~ → 実装済み。`bfs01` は未実装
- block-cut tree
- link-cut tree（nyaan にあり）
- low-link（SCC, 関節点, 橋）
- bucket BFS
- undo DSU（経路圧縮なし）
- フロー（最大流 / 最小費用流）
- rerooting DP
- `forest` / `hld` の `graph_base` 対応

### math

- 約数系
- Stern-Brocot tree
- floor_sum / linear floor sum
- CRT / Garner
- 多項式（NTT, FPS）
- ポラードロー
- 行列（既存 `matrix` の拡張・高速化）

### mod

- dynamic_modint32
- static_modint
- sqrt mod

### misc

- timer
- next_combination
- bigint
- 座標圧縮2D

### string

- Z-algorithm
- KMP（prefix function）
- suffix array, LCP array
- Aho-Corasick
- LCS

### その他

- convolution（理解してから）
- 幾何（必要に応じて）

---

## ドキュメント

- 配置: **`doc/`**（`doc/graph/` に edge, graph, csr, dijkstra, tree_lca, forest, …）
- `doc/README.md` のパス表記を `doc/` に修正完了
- 現在のドキュメント不足状況の精査結果:
  - **カテゴリごと未作成**:
    - `algebra/` (basic_abel, basic_monoid, range_affine_range_sum_monoid, rolling_hash_monoid)
    - `misc/` (compress, counting_sort, lis, node_pool, xorshift)
    - `mod/` (mod, mod61, modint)
    - ルート直下の基本ヘッダ (`dump.hpp`, `io.hpp`, `types.hpp`, `my_template.hpp` 等)
  - **既存カテゴリ内のファイル不足**:
    - `container/`: bit_vector, fenwick_tree, mdarray, persistent_stack, segtree, sq_div, wavelet_matrix, wrapper系
    - `graph/`: hld, range_edge_graph
    - `hash/`: to_hash
    - `math/`: binom, xor_basis
- 既存のドキュメント（.md）内には "TODO" 等の明白な未完成マーカーや空ファイルは見当たらなかった。
