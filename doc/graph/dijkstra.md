# dijkstra（単一始点最短経路）

非負辺のグラフに対し、ダイクストラ法で始点からの最短距離と最短経路木（SPT）を求めます。入力は `build()` 済みの `graph_base` です。有向最短経路では **`DirectedGraph`** を使います。

---

## テンプレート制約

```
template <dijkstra_graph_concept Graph> struct dijkstra;
```

`Graph` は `graph_concept` を満たし、さらに次が必要です。

- `weight_type`: 全順序、`+`、`0`、`numeric_limits::max()` が使える
- `edge_type`: デフォルト構築可能（SPT 初期化用）

辺型の最小要件だけを満たす `edge_concept` とは別に、距離計算用の制約は `dijkstra_weight_concept` でまとめています。

---

## コンストラクタ

```
explicit dijkstra(const Graph& g);
```

| 引数 | 説明 |
|------|------|
| `g` | 頂点数 `g.N`、隣接リスト `g[u]`（CSR）を持つグラフ。`build()` 済みであること |

`g` は `dijkstra` オブジェクトより長く生存させてください（`const Graph&` を保持）。

**計算量**

- 構築: $O(1)$（参照の保存のみ）

---

## solve

```
const vector<weight_type>& solve(i32 s);
```

始点 `s` からの最短距離を `dist` に格納し、最短経路木を `spt` に格納します。戻り値は `get_dist()` と同じ参照です。

| 項目 | 内容 |
|------|------|
| 距離 | `dist[v]` は $s \to v$ の最短距離。到達不能なら `Inf`（`numeric_limits<weight_type>::max()`） |
| SPT | `spt[v]` は $s \to v$ の最短路における「$v$ に入る辺」。`v = s` は未設定 |
| 負辺 | 非負辺を仮定。負辺があると不正な結果になります |
| オーバーフロー | `d + w` が `Inf` を超える緩和はスキップ |

**制約**

- $0 \leq s < N$

**計算量**

- $O((N + M) \log N)$（$M$ は CSR 上の辺数。無向グラフは逆辺込み）

---

## get_dist

```
const vector<weight_type>& get_dist() const;
```

`solve` 後の距離配列を返します。

---

## get_spt

```
const vector<edge_type>& get_spt() const;
```

`solve` 後の最短経路木を返します。`spt[v].u` は $v$ の親頂点、`spt[v].v == v` です。

---

## 使用例（重み付き有向グラフ）

```cpp
#include "gwen/graph/dijkstra.hpp"
#include "gwen/graph/graph.hpp"

using namespace gwen;

i32 N, M;
// 入力 ...
DirectedGraph<i64> g(N);
for (i32 i = 0; i < M; ++i) {
    i32 u, v;
    i64 w;
    // 入力 ...
    g.add_edge(u, v, w);
}
g.build();

dijkstra dj(g);
const auto& dist = dj.solve(0);
for (i32 v = 0; v < N; ++v) {
    if (dist[v] == std::numeric_limits<i64>::max()) {
        // 到達不能
    }
}
```

無向グラフのときは `NonDirectedGraph` でも構いません（CSR 内で両方向に辺が入ります）。

---

## 関連

- グラフ構築: [graph.md](graph.md)
- 辺型: [edge.md](edge.md)
- CSR: [csr.md](csr.md)
