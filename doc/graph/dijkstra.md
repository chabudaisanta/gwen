---
title: Dijkstra
documentation_of: //include/gwen/graph/dijkstra.hpp
---

# Dijkstra

優先度付きキュー（`std::priority_queue`）を用いた単一始点最短経路アルゴリズムの実装です。

## **コンストラクタ**

```cpp
template <dijkstra_graph G>
class Dijkstra;

Dijkstra(const G& g)
```

構築済みグラフ $g$ を対象として初期化します。

**制約**

- なし

**計算量**

- $O(N)$（$N$ は頂点数）

## **solve**

```cpp
const std::vector<weight_type>& solve(i32 s)
```

頂点 $s$ を始点とする最短経路を計算し、各頂点への最短距離の配列を返します。
到達不可能な頂点の距離は `Dijkstra<G>::INF` となります。

**制約**

- $0 \le s < N$

**計算量**

- $O(M \log N)$（$M$ はエッジ数、$N$ は頂点数）

## **get_dist**

```cpp
const std::vector<weight_type>& get_dist() const
```

`solve()` で計算された距離配列を取得します。

**制約**

- `solve()` が呼び出された後であること

**計算量**

- $O(1)$

## **get_spt**

```cpp
const std::vector<edge_type>& get_spt() const
```

最短経路木（Shortest Path Tree）における、各頂点の「親へ向かうエッジ」の配列を取得します。始点および到達不可能な頂点にはデフォルト構築されたエッジが格納されます。

**制約**

- `solve()` が呼び出された後であること

**計算量**

- $O(1)$
