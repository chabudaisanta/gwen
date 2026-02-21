# CSR（Compressed Sparse Row）

有向グラフ（または無向グラフ）を隣接リスト形式で保持するデータ構造です。頂点 $v$ から出る辺を連続したメモリで参照できます。

参照: [AC Library document_ja](https://atcoder.github.io/ac-library/master/document_ja/)

---

## テンプレート引数

- `Edge`: 辺の型。少なくともメンバ `u`, `v`（始点・終点）を持つこと。無向の場合は `directed = false` で逆辺も追加されます。

---

## コンストラクタ

```
(1) csr<Edge> g;
(2) csr<Edge> g(i32 n, const vector<Edge>& edges, bool directed = true);
```

- **(1)** 空で構築します。
- **(2)** 頂点数 `n`、辺リスト `edges` で構築します。`directed == false` のときは各辺に対して逆辺も追加します。

**計算量**

- (2): $O(n + |\mathrm{edges}|)$

---

## size

```
i32 g.size() const;
```

頂点数 $n$ を返します。

---

## num_edges

```
i32 g.num_edges() const;
```

辺数（無向の場合は逆辺を含む）を返します。

---

## operator[]

```
span<const Edge> g[i32 v] const;
```

頂点 `v` から出る辺の列を返します。`std::span` なので範囲 for で使えます。

**制約**

- $0 \leq v < n$

**計算量**

- $O(1)$（参照のみ）

---

## 使用例

```cpp
#include "gwen/graph/csr.hpp"

using namespace gwen;

struct edge { i32 u, v; };
vector<edge> es = {{0, 1}, {1, 2}, {2, 0}};
csr<edge> g(3, es, false);  // 無向
for (const auto& e : g[0]) {
    // e.v は 0 に隣接する頂点
}
```
