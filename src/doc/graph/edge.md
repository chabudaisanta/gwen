# edge（辺型）

グラフの辺を表す型と、辺として使える型のコンセプト `edge_concept` です。`graph_base` や `tree_lca` のテンプレート引数として使います。

---

## edge_concept

辺型 `T` が次を満たすとき `edge_concept<T>` です。

- メンバ `u`, `v` が `i32` に変換可能
- 型エイリアス `weight_type` が存在する

---

## edge

```
template <typename T = i32> struct edge;
```

| メンバ | 型 | 説明 |
|--------|-----|------|
| `u`, `v` | `i32` | 始点・終点 |
| `w` | `weight_type` (= `T`) | 重み |

### コンストラクタ

```
edge();
edge(i32 u, i32 v);              // w = 1
edge(i32 u, i32 v, weight_type w);
```

---

## 使用例

```cpp
#include "gwen/graph/edge.hpp"

using namespace gwen;

edge<> e(0, 1);
edge<i64> w(0, 1, 100);

static_assert(edge_concept<edge<>>);
```
