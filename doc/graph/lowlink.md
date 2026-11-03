---
title: Lowlink
documentation_of: //include/gwen/graph/lowlink.hpp
---

# Lowlink

無向グラフに対して DFS の発見順と lowlink 値を求め、関節点、橋、頂点二重連結成分を列挙します。
頂点二重連結成分は辺番号の集合として返します。

## コンストラクタ

```cpp
Lowlink<EdgeType> lowlink(const GraphBase<EdgeType, false>& g)
```

グラフを解析します。辺番号は `g.all_edges()` の 0-based 添字です。

**制約**

- `EdgeType` は `gwen::edge` を満たすこと。

**計算量**

- $O(V + E)$

## ord / low

```cpp
const std::vector<i32>& lowlink.ord() const
const std::vector<i32>& lowlink.low() const
```

各頂点の DFS 発見順と lowlink 値を返します。`low[v]` は `v` の DFS 部分木から、木辺を 0 回以上たどり、後退辺を高々 1 回使って到達できる頂点の `ord` の最小値です。

**計算量**

- $O(1)$

## articulation_points

```cpp
const std::vector<i32>& lowlink.articulation_points() const
```

関節点を頂点番号の昇順で返します。

**計算量**

- $O(1)$

## bridges

```cpp
const std::vector<usize>& lowlink.bridges() const
```

橋を `g.all_edges()` における辺番号で返します。返却順は未規定です。並行辺がある場合も辺を個別に判定します。

**計算量**

- $O(1)$

## vertex_biconnected_components

```cpp
const std::vector<std::vector<usize>>& lowlink.vertex_biconnected_components() const
```

頂点二重連結成分 (blocks) を辺番号のリストとして返します。成分と成分内の辺の返却順は未規定です。橋は1辺だけの成分になります。自己ループも1辺だけの成分として一度含まれます。孤立頂点は辺集合で表す成分には含めません。

**計算量**

- $O(1)$
