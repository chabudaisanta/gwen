# dump機能の強化とモジュール別対応プラン (最終案)

## 1. 設計思想（シンプルさ優先）
ユーザーフィードバックにより、ローカル実行前提のため O(N) の計算量よりも内部実装のシンプルさを優先する方針に変更されました。
遅延評価付きのデータ構造などでは、正確な状態を副作用なく（`const`）取得するためにディープコピーが必要なため、O(N) は許容されます。

## 2. DUMP_SIZE_LIMIT の実装方針（シンプルさ重視）
*   **制限の適用とフォーマット**: `#ifdef DUMP_SIZE_LIMIT` で設定された値を用い、コンテナや配列の先頭 `DUMP_SIZE_LIMIT` 個と最後の1個を出力する。
*   **C++23 の活用**: 取得済みのコンテナに対して `std::views::take(DUMP_SIZE_LIMIT)` 等を用いて簡潔に文字列化する共通ヘルパー `gwen::internal::format_range` を実装する。

## 3. 各モジュールで dump するべき要素の定義（シンプル実装版）
複雑なクエリによる部分取得を避け、既存の `to_vec()` などを利用して取得した結果にリミットを適用する。

### データ構造 (ds / bbst)
*   **FenwickTree (BIT) / SegmentTree / LazySegmentTree**: `N`, `data(restored)`。`to_vec()` で全取得してからフォーマッタで切り詰める。*(Lazy の場合)* `lazy` もそのまま出力。
*   **DynamicSegmentTree**: `N`, `size`, `elements`（ノード一覧に対してリミット適用）
*   **UnionFind / DSU**: `N`, `groups`（`groups()` で得た全連結成分にリミット適用）

### グラフ (graph)
*   **Graph / Digraph**: `V`, `E`, `adjacency_list`（全取得しリミット適用）
*   **Tree (木構造)**: `V`, `root`, `parents`, `depths`

### 数学・代数・文字列 (math / alge / string etc.)
*   **ModInt**: `val()`
*   **Zobrist Hash**: `(key, hash_value)` の一覧
