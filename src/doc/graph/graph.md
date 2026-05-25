# graph_base（グラフ）

辺リストから CSR を構築するグラフ型です。`add_edge` で辺を追加し、`build()` 後に `operator[]` で隣接辺を参照します。

---

## graph_concept

型 `T` が次を満たすとき `graph_concept<T>` です。

- `edge_type`, `weight_type` が存在する
- `N` が `i32` に変換可能
- `t[u]` が range で、要素参照が `const edge_type&` に変換可能

---

## graph_base

```
template <edge_concept Edge, bool isDirected = true> struct graph_base;
```

| 型エイリアス | 説明 |
|--------------|------|
| `edge_type` | `Edge` |
| `weight_type` | `Edge::weight_type` |

| メンバ | 説明 |
|--------|------|
| `N` | 頂点数（構築時に固定） |

### エイリアス

```
DirectedGraph<T>    = graph_base<edge<T>, true>
NonDirectedGraph<T> = graph_base<edge<T>, false>
```

---

## コンストラクタ

```
(1) graph_base();
(2) explicit graph_base(i32 N);
(3) graph_base(i32 N, const vector<Edge>& E);
```

- **(1)** 頂点 0 の空グラフ。
- **(2)** 頂点 `N` のみ。辺は `add_edge` で追加し、最後に `build()`。
- **(3)** 辺リスト `E` から即座に CSR を構築（`build()` 不要）。

**計算量**

- (3) の構築・`build()`: $O(N + |E|)$（無向のとき CSR 内で逆辺も追加）

---

## add_edge

```
void add_edge(const Edge& e);
void add_edge(i32 u, i32 v);
void add_edge(i32 u, i32 v, weight_type w);
```

`build()` 前のみ有効です。

---

## build

```
void build();
```

辺バッファから CSR を構築します。二重呼び出しは `assert` で失敗します。

---

## edges

```
const vector<Edge>& edges() const;
```

保持している辺リストを返します。`tree_lca` などはこのリストから木を構築します。

---

## operator[]

```
span<const Edge> g[i32 u] const;
```

頂点 `u` から出る辺の列（CSR）を返します。`build()` 後、または (3) のコンストラクタ直後のみ有効です。

**計算量**

- $O(1)$

---

## 使用例

```cpp
#include "gwen/graph/graph.hpp"

using namespace gwen;

NonDirectedGraph<i32> g(4);
g.add_edge(0, 1);
g.add_edge(1, 2);
g.add_edge(2, 3);
g.build();

for (const auto& e : g[1]) {
    // 隣接辺
}

tree_lca lca(0, g);
```

---

## 関連

- 辺型: [edge.md](edge.md)
- CSR の詳細: [csr.md](csr.md)
- 根付き木の LCA: [tree_lca.md](tree_lca.md)
