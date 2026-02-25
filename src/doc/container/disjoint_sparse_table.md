# Disjoint Sparse Table

[モノイド](https://ja.wikipedia.org/wiki/%E3%83%A2%E3%83%8E%E3%82%A2%E3%83%89) $(S, \cdot: S \times S \to S, e \in S)$ に対し使用できるデータ構造です。

長さ $n$ の列に対し、

- 区間の要素の総積（モノイド積）の取得

を $O(1)$ で行います。**更新はできません**（静的）。半群（逆元がなくてもよい）で動作します。

前計算 $O(n \log n)$ 時間・空間。クエリ $O(1)$。

---

## コンストラクタ

```
(1) disjoint_sparse_table<Monoid> dst;
(2) disjoint_sparse_table<Monoid> dst(vector<S> vec);
```

- **(1)** 空で構築します。
- **(2)** `vec` を列として構築します。

**計算量**

- (1): $O(1)$
- (2): $O(n \log n)$

---

## prod

```
S dst.prod(i32 l, i32 r) const;
```

$op(a[l], a[l+1], \ldots, a[r-1])$ を返します。$l = r$ のときは `Monoid::e()` を返します。

**制約**

- $0 \leq l \leq r \leq n$

**計算量**

- $O(1)$

---

## 使用例

```cpp
#include "gwen/container/disjoint_sparse_table.hpp"
#include "gwen/algebra/basic_monoid.hpp"

using namespace gwen;

std::vector<i32> v = {1, 3, 2, 5, 4};
disjoint_sparse_table<min_monoid<i32>> dst(v);
dst.prod(0, 5);   // min = 1
dst.prod(2, 4);   // min = 2
```
