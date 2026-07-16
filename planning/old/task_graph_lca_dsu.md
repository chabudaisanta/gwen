# Phase 4.2: `weighted_dsu`, `forest`, `tree_lca` の移植仕様

本ドキュメントは `weighted_dsu`, `forest`, `tree_lca` の移植およびリファクタリングの仕様書です。

## 1. `weighted_dsu.hpp` の仕様
重み付き Union-Find データ構造。

- **クラス名**: `WeightedDsu` (PascalCase化)
- **テンプレート制約**: C++23 のコンセプト `abel` を用いて、テンプレート引数 `Abel` に加群であることを制約します (`template <abel Abel> struct WeightedDsu`)。
- **メソッド**:
  - `leader(i32 a)`
  - `same(i32 a, i32 b)`
  - `diff(i32 a, i32 b)` : $P(a) - P(b)$ を返す
  - `merge(i32 a, i32 b, S w)` : $P(a) - P(b) = w$ となるようにマージ
  - `groups()` : 連結成分ごとの頂点リストを返す
  - Doxygen形式のコメントを各メソッドに記述します。
- **変更点**: masterからはクラス名とテンプレート制約のみの軽微な変更になります。

## 2. `forest.hpp` の仕様
複数の木（森）に対する LCA (最小共通祖先) や k 番目の祖先を求める構造体。

- **クラス名**: `Forest` (PascalCase化)
- **テンプレート**: `template <edge_concept Edge, bool isDirected>`
- **コンストラクタ**: `explicit Forest(const Graph<Edge, isDirected>& G)`
  - ユーザー指示に従い、`std::vector<std::vector<i32>>` から `Graph` を受け取る形に変更します。
  - 内部で全頂点についてDFSを行い、親（`parent`）と深さ（`depth`）を構築し、ダブリングのテーブルを作成します。
- **メソッド**: `get_root`, `get_size`, `is_same`, `get_depth`, `get_kth_ancestor`, `get_lca`, `get_dist` (全てスネークケース、Doxygen コメント付き)

## 3. `tree_lca.hpp` の仕様
単一の木に対する LCA を求めるクラス。

- **クラス名**: `TreeLca` (PascalCase化)
- **テンプレート**: `template <edge_concept Edge, bool isDirected>`
- **コンストラクタ**: `explicit TreeLca(i32 root, const Graph<Edge, isDirected>& G)`
  - ユーザー指示に従い `xor_tree_scan` への依存を削除します。
  - 代わりにコンストラクタ内で `root` から DFS を行い、親と深さを計算してからダブリングのテーブルを構築します。
- **メソッド**: `kth_anc`, `depth`, `lca`, `len` (全てスネークケース、Doxygen コメント付き)
