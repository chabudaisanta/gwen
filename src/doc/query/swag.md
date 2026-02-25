# SWAG（Sliding Window Aggregation）

区間 $[l, r)$ に対するモノイド積を、クエリを「$l$ の昇順 → $r$ の昇順」で処理することで、ならし $O(1)$ で求めるオフラインアルゴリズムです。`sliding_window_aggregation` という名前で提供されています。

---

## コンストラクタ

```
(1) sliding_window_aggregation<Monoid> swag;
(2) sliding_window_aggregation<Monoid> swag(i32 N);
```

- **(1)** 列の長さ 0 で構築します。
- **(2)** 列の長さ `N` で構築します。クエリの区間は $[0, N)$ 内であることを想定します。

**計算量**

- $O(1)$

---

## add_query

```
void swag.add_query(i32 l, i32 r);
```

区間 $[l, r)$ に対するクエリを 1 件追加します。

**制約**

- $0 \leq l \leq r \leq N$

**計算量**

- $O(1)$

---

## solve

```
vector<S> swag.solve(const vector<S>& data, bool sorted = false);
```

- **入力**:
  - `data`: 長さ `N` の列。$i$ 番目の要素は `data[i]` です。
  - `sorted`: `true` のとき、クエリは既に「$l$ の昇順 → $r$ の昇順」で並んでいるとして、ソートをスキップします。`false` のときは内部で counting sort によりソートします。
- **返り値**: 追加したクエリの順に対応する、各区間のモノイド積の列。

**前提**

- クエリを「$l$ の昇順、同値なら $r$ の昇順」で処理したとき、$l$ も $r$ も単調非減少になるように並べ替えて処理します。この順序で区間を伸縮すると、各要素の追加・削除がならし $O(1)$ でできます。

**計算量**

- ソート: $O(Q + N)$（counting sort 使用時）。クエリ処理: ならし $O(Q + N)$。

---

## 使用例

```cpp
#include "gwen/query/swag.hpp"
#include "gwen/algebra/basic_monoid.hpp"

using namespace gwen;

vector<i64> data = {1, 2, 3, 4, 5};
sliding_window_aggregation<sum_monoid<i64>> swag(5);
swag.add_query(0, 3);  // [0,3) -> 1+2+3 = 6
swag.add_query(1, 4);  // [1,4) -> 2+3+4 = 9
auto res = swag.solve(data);  // {6, 9}
```
