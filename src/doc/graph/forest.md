# Forest（森の LCA・距離）

複数の根付き木（森）に対して、LCA・$k$ 個先の祖先・距離を $O(\log n)$ で求めるデータ構造です。頂点数 $n$ の隣接リスト `vector<vector<i32>>` から構築します。

---

## コンストラクタ

```
forest f(const vector<vector<i32>>& G);
```

- **入力**: `G[v]` が頂点 `v` の子（または隣接点）のリストである隣接リスト。無向グラフとして与え、各連結成分は木であることを仮定します。
- 各連結成分ごとに DFS で根・深さ・親を求め、ダブリング用の親テーブルを構築します。根は「その成分で最初に DFS を始めた頂点」です。

**計算量**

- $O(n \log n)$

---

## get_root

```
i32 f.get_root(i32 p) const;
```

頂点 `p` の属する木の根を返します。

**計算量**

- $O(1)$

---

## get_size

```
i32 f.get_size(i32 p) const;
```

頂点 `p` の属する木の頂点数を返します。根の `parent_or_size` に負の値で格納されている想定です。

**計算量**

- $O(1)$

---

## is_same

```
bool f.is_same(i32 u, i32 v) const;
```

頂点 `u` と `v` が同じ木に属するかどうかを返します。

**計算量**

- $O(1)$

---

## get_depth

```
i32 f.get_depth(i32 p) const;
```

頂点 `p` の深さ（根からの距離）を返します。

**計算量**

- $O(1)$

---

## get_kth_ancestor

```
i32 f.get_kth_ancestor(i32 v, i32 k) const;
```

頂点 `v` の $k$ 個先の祖先を返します。$k$ が範囲外のときは `-1` を返します。

**制約**

- $0 \leq k \leq \mathrm{depth}(v)$ でない場合、戻り値は `-1` の可能性があります。

**計算量**

- $O(\log n)$

---

## get_lca

```
i32 f.get_lca(i32 u, i32 v) const;
```

頂点 `u` と `v` の LCA を返します。異なる木に属する場合は `-1` を返します。

**計算量**

- $O(\log n)$

---

## get_dist

```
i32 f.get_dist(i32 u, i32 v) const;
```

頂点 `u` と `v` の距離（パス上の辺の本数）を返します。異なる木に属する場合は `-1` を返します。

**計算量**

- $O(\log n)$

---

## 使用例

```cpp
#include "gwen/graph/forest.hpp"

using namespace gwen;

vector<vector<i32>> G(5);
G[0] = {1, 2};
G[1] = {0, 3};
G[2] = {0};
G[3] = {1};
G[4] = {};  // 孤立点
forest f(G);
f.get_lca(0, 3);       // 0
f.get_dist(0, 3);      // 2
f.get_kth_ancestor(3, 2);  // 0
```
