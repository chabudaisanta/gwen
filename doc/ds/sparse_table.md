---
title: Sparse Table
documentation_of: //include/gwen/ds/sparse_table.hpp
---

## 概要

静的配列に対する区間積を $O(1)$ で取得するデータ構造です。クエリで重なる区間を結合するため、演算には冪等性 `op(x, x) == x` が必要です。最小値や最大値などに使用できます。

## コンストラクタ

```cpp
SparseTable<M>(const std::vector<typename M::S>& values)
```

- `M`: `gwen::monoid` を満たす型

**制約**
- クエリで使う演算 `M::op` は結合的かつ冪等であること

**計算量**
- $O(N \log N)$

**空間計算量**
- $O(N \log N)$

## メソッド

### size

```cpp
i32 SparseTable<M>::size() const
```

配列の長さを返します。

**計算量**
- $O(1)$

### prod

```cpp
typename M::S SparseTable<M>::prod(i32 l, i32 r) const
```

区間 $[l, r)$ の積を返します。空区間では `M::e()` を返します。

**制約**
- $0 \le l \le r \le N$
- `M::op` は冪等であること

**計算量**
- $O(1)$
