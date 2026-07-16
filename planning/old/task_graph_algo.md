# グラフアルゴリズム (dijkstra, hld) 移植タスク

## 1. 概要
`dijkstra.hpp` (単一始点最短経路) と `hld.hpp` (Heavy-Light Decomposition) を移植し、新しく実装した `graph` コンセプトに適合させる。

## 2. 移植対象のファイル
- `include/gwen/graph/dijkstra.hpp`
- `include/gwen/graph/hld.hpp`

## 3. 仕様・設計方針
### `dijkstra.hpp`
- **クラス名**: PascalCase に従い `Dijkstra` とする。
- **コンセプト**:
  - `dijkstra_weight`: 重みの要件 (`std::totally_ordered`, `+` 演算, ゼロ初期化, `std::numeric_limits<W>::max()` など) を定義。
  - `dijkstra_graph`: `graph` コンセプトを満たし、かつその `weight_type` が `dijkstra_weight` を満たすこと。
- **インターフェース**:
  - `template <dijkstra_graph G> class Dijkstra { ... };` (クラステンプレートとする)
  - `explicit Dijkstra(const G& g)`
  - `const std::vector<weight_type>& solve(i32 s)`
  - `const std::vector<weight_type>& get_dist() const`
  - `const std::vector<edge_type>& get_spt() const`
- **アルゴリズム**: `std::priority_queue` を用いた標準的な Dijkstra 法。`Graph` の `edges(u)` を用いてイテレーションする。

### `hld.hpp`
- **クラス名**: PascalCase に従い `Hld` とする。
- **インターフェース**:
  - クラス全体を `template <graph G> class Hld { ... };` としてテンプレート化する。
  - コンストラクタは `explicit Hld(const G& g, i32 root = 0)` を用意する。
  - 内部で `g.adjacent(u)` を走査して `depth`, `parent`, `heavy`, `head`, `in`, `out` などを計算する。
  - `get_lca`, `get_dist`, `path`, `kth_on_path`, `kth_ancestor`, `subtree` メソッド等、以前のAPIをそのまま継承する。
- **注意点**: 木構造であることを前提とするため、無向グラフで構築されている場合でも、親へ逆流しないように DFS を書く。

## 4. テストと Verify
- `dijkstra`:
  - 単体テスト (`dijkstra_test.cpp`)
  - `competitive-verifier` (`yosupo judge` の `shortest_path` などで verify可能であれば `verify/graph/shortest_path_verify.cpp` を作成)
- `hld`:
  - 単体テスト (`hld_test.cpp`)
  - `competitive-verifier` (`yosupo judge` の `lca` などで verify 可能であれば `verify/graph/lca_verify.cpp` を作成)

## 5. 作業手順
1. 本ファイルの作成（現在完了）
2. サブエージェントによる事前レビュー依頼
3. ユーザーへの仕様レビュー依頼
4. 実装の実施 (`dijkstra.hpp`, `hld.hpp`)
5. サブエージェントによる実装レビュー
6. テスト (`dijkstra_test.cpp`, `hld_test.cpp`) と Verify コード (`shortest_path_verify.cpp`, `lca_verify.cpp`)
7. ドキュメント作成 (`doc/graph/dijkstra.md`, `doc/graph/hld.md`) と `index.md` への追加
8. 最終レビューとコミット
