# Heavy-Light Decomposition (hld)

根付き木を heavy edge の連鎖に分解し、木上の任意のパスや部分木に関するクエリを $O(\log N)$ 個の区間に帰着させるデータ構造 (HL分解) です。

## ヘッダファイル

```cpp
#include "gwen/graph/hld.hpp"
```

## コンストラクタ

```cpp
// 1. 隣接リストからの構築
explicit hld(i32 n, i32 root, const std::vector<std::vector<i32>>& G);

// 2. 辺リストからの構築 (無向辺を想定)
template <typename Edge>
explicit hld(i32 n, i32 root, const std::vector<Edge>& edges);
```
- `n`: 頂点数
- `root`: 根となる頂点番号 (0-indexed)
- 森（非連結）には対応していません。森の場合は連結成分ごとにインスタンスを分けて構築してください。

## メンバ関数

### `i32 get_lca(i32 u, i32 v) const`
頂点 `u` と `v` の最小共通祖先 (LCA) を返します。
計算量: $O(\log N)$

### `i32 get_dist(i32 u, i32 v) const`
頂点 `u` と `v` のパス上の距離（辺の本数）を返します。
計算量: $O(\log N)$

### `std::vector<std::pair<i32, i32>> path(i32 u, i32 v) const`
頂点 `u` から `v` へのパスを in-index の区間列に分解して返します。
返り値の各要素は `[l, r)` を表す半開区間です。セグメント木などでパスに対するクエリ（取得・更新）を行う際に、分解された区間群に対して操作を適用できます。
計算量: $O(\log N)$

### `i32 kth_on_path(i32 u, i32 v, i32 k) const`
頂点 `u` から `v` に向かう単純パス上で `k` 番目（`u` を 0 番目とする）の頂点を返します。
パス長が `k` 未満の場合は `-1` を返します。
計算量: $O(\log N)$

### `i32 kth_ancestor(i32 v, i32 k) const`
頂点 `v` の `k` 個上の祖先を返します。
存在しない（根を通り越す）場合は `-1` を返します。
計算量: $O(\log N)$

### `std::pair<i32, i32> subtree(i32 v) const`
頂点 `v` を根とする部分木に対応する in-index の半開区間 `[in[v], out[v])` を返します。
計算量: $O(1)$

### `i32 idx(i32 v) const`
頂点 `v` の in-index (Euler tour での行きがけ順、セグ木に乗せるときの配列インデックス) を返します。
計算量: $O(1)$

### `i32 vertex(i32 i) const`
in-index `i` に対応する本来の頂点番号を返します。
計算量: $O(1)$
