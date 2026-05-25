# dijkstra（単一始点最短経路）

非負辺の有向グラフ（`DirectedGraph` 推奨）に対し、ダイクストラ法で最短距離と最短経路木を求めます。入力は `build()` 済みの `graph_base` です。

---

## コンストラクタ

```
explicit dijkstra(const Graph& g);
```

- `g.N` を頂点数として使います（`g` は `dijkstra` より長く生存させること）

---

## solve

```
const vector<weight_type>& solve(i32 s);
```

始点 `s` からの最短距離を `dist` に格納し、最短経路木を `spt` に格納します。戻り値は `get_dist()` と同じ参照です。

- 到達不能頂点の距離は `numeric_limits<weight_type>::max()`（`Inf`）
- 負辺があると正しく動きません

**計算量**

- $O((N + M) \log N)$（$M$ は辺数）

---

## get_dist / get_spt

```
const vector<weight_type>& get_dist() const;
const vector<edge_type>& get_spt() const;
```

`solve` 後の距離配列・最短経路木（`spt[v]` は `v` への最短辺。根 `s` は未設定）を返します。

---

## 使用例

```cpp
#include "gwen/graph/dijkstra.hpp"
#include "gwen/graph/graph.hpp"

using namespace gwen;

DirectedGraph<i32> g(N);
// add_edge ...
g.build();

dijkstra dj(g);
const auto& dist = dj.solve(s);
```

---

## 関連

- グラフ構築: [graph.md](graph.md)
- 辺型: [edge.md](edge.md)
