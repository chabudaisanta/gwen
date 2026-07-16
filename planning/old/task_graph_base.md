# グラフ基盤 (graph, csr, edge) 移植タスク

## 1. 概要
グラフの基本構造を定義する `edge.hpp`, `csr.hpp`, `graph.hpp` を `master` ブランチから移植し、C++23 の規約に沿うようリファクタリングする。

## 2. 移植対象のファイル
- `include/gwen/graph/edge.hpp`
- `include/gwen/graph/csr.hpp`
- `include/gwen/graph/graph.hpp`

## 3. 仕様・設計方針
- **コンセプトの配置**: ユーザー指示に従い、各ヘッダーファイル内で直接 Concept を定義する。
  - `edge.hpp` 内に `edge_concept` を定義。
  - `graph.hpp` 内に `graph_concept` を定義。
  - コンセプト名は `snake_case` の命名規則に従う。`edge_concept` はそのままか、単に `is_edge` よりは `edge` という名詞は既に構造体名として使われるため、`edge_like` のような名前にするか検討が必要。ただし `RULES.md` によると、コンセプト名に `_concept` というサフィックスは非推奨（「_concept は使いません」）。抽象的な単語を使うか、前置詞・形容詞を使う。Edgeは名詞。構造体が `Edge` (PascalCase) なら コンセプトは `edge` (snake_case) にできる。
  - しかし `RULES.md` には「型名 (クラス、構造体) は PascalCase を使用します。（例: Edge）」とある。
  - したがって、構造体を `Edge` にし、コンセプトを `edge` にすることが可能。

- **`edge.hpp`**:
  - `template <typename T> concept edge = ...;`
  - `template <typename T = i32> struct Edge { ... };` (旧 `edge`)
  - `u`, `v`, `w` (重み) を持つ。
  - メンバー変数名は snake_case なので `u`, `v`, `w` でOK。

- **`csr.hpp`**:
  - `template <edge E> class Csr { ... };` (旧 `csr`)
  - クラス名は PascalCase なので `Csr`。
  - CSR形式の有向・無向グラフの隣接リスト。
  - ビルド処理 (配列の累積和など) はほぼそのまま移植。`std::span` によるイテレーションを提供。

- **`graph.hpp`**:
  - `template <typename T> concept graph = ...;`
  - `template <edge E, bool IsDirected = true> class GraphBase { ... };` (旧 `graph_base`)
  - エイリアス `DirectedGraph<T>`, `NonDirectedGraph<T>` を用意 (PascalCase)。
  - `Csr` を内部に持ち、遅延評価 (add_edge して、後で build) する設計を継承。

## 4. 依存関係
- `gwen/types.hpp` (`i32`, `i64` など)
- 標準ライブラリ (`<vector>`, `<span>`, `<concepts>`, `<cassert>`)

## 5. 作業手順
1. 本ファイルの作成（現在完了）
2. サブエージェントによる事前レビュー依頼
3. ユーザーへの仕様レビュー依頼
4. 実装の実施 (`edge.hpp`, `csr.hpp`, `graph.hpp`)
5. サブエージェントによる実装レビュー
6. テスト (`test/unit/graph_test.cpp`)
   - Verify について: グラフ基盤単体で解ける Verify 問題が乏しいため、今回は単体テストのみとし、Verify は `dijkstra` や `hld` 実装時に回したいと考えていますが、よろしいでしょうか？（ユーザーへの承認事項）
7. ドキュメント作成 (`doc/graph/graph.md`, `doc/graph/csr.md`, `doc/graph/edge.md`) と `.verify-helper/docs/index.md` への追加
8. 最終レビューとコミット
