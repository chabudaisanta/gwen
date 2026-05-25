# tree_lca（根付き木の LCA）

根付き木 1 本に対し、ダブリング（binary lifting）で LCA・$k$ 個先の祖先・深さ・パス長を求めるデータ構造です。入力は `graph_base`（推奨: `NonDirectedGraph`）です。

---

## コンストラクタ

```
template <edge_concept Edge, bool isDirected>
explicit tree_lca(i32 root, const graph_base<Edge, isDirected>& g);
```

- **root**: 根とする頂点の番号
- **g**: `g.edges()` の辺から木を復元して構築します。`add_edge` で作った場合は事前に `build()` してください（`edges()` は `build()` 前でも参照可能ですが、木として正しい辺リストである必要があります）

**前提**

- 辺集合は根 `root` から到達可能な木であること（閉路・余分な辺がないこと）

**計算量**

- 構築: $O(n \log n)$（$n = g.N$）

---

## depth

```
i32 depth(i32 v) const;
```

頂点 `v` の深さ（根からの距離）を返します。

**制約**

- $0 \leq v < n$

**計算量**

- $O(1)$

---

## kth_anc

```
i32 kth_anc(i32 v, i32 k) const;
```

頂点 `v` の $k$ 個上の祖先を返します。$k > \mathrm{depth}(v)$ のときは `-1` を返します。

**制約**

- $0 \leq v < n$, $k \geq 0$

**計算量**

- $O(\log n)$

---

## lca

```
i32 lca(i32 u, i32 v) const;
```

頂点 `u` と `v` の LCA を返します。

**制約**

- $0 \leq u, v < n$

**計算量**

- $O(\log n)$

---

## len

```
i32 len(i32 u, i32 v) const;
```

頂点 `u` と `v` を結ぶパス上の頂点数（$u$–$v$ パスの長さ）を返します。

$$\mathrm{len}(u, v) = \mathrm{depth}(u) + \mathrm{depth}(v) - 2 \cdot \mathrm{depth}(\mathrm{lca}(u, v)) + 1$$

**計算量**

- $O(\log n)$

---

## 使用例

```cpp
#include "gwen/graph/graph.hpp"
#include "gwen/graph/tree_lca.hpp"

using namespace gwen;

NonDirectedGraph<i32> g(N);
for (i32 i = 1; i < N; ++i) {
    i32 p;
    cin >> p;
    g.add_edge(p, i);
}
g.build();

tree_lca T(0, g);
T.lca(u, v);
T.kth_anc(v, k);
T.depth(v);
T.len(u, v);
```

---

## 関連

- 複数木（森）: [forest.md](forest.md)（隣接リスト入力）
- グラフ構築: [graph.md](graph.md)
- 辺型: [edge.md](edge.md)
