---
title: Disjoint Sparse Table
documentation_of: //include/gwen/ds/disjoint_sparse_table.hpp
---

## 概要

静的配列に対する区間積を $O(1)$ で取得するデータ構造です。結合則を満たす任意のモノイドを扱え、演算の可換性は必要ありません。

## コンストラクタ

```cpp
DisjointSparseTable<M>(const std::vector<typename M::S>& values)
```

- `M`: `gwen::monoid` を満たす型

**制約**
- `M::op` が結合的であること

**計算量**
- $O(N \log N)$

**空間計算量**
- $O(N \log N)$

## メソッド

### size

```cpp
i32 DisjointSparseTable<M>::size() const
```

配列の長さを返します。

**計算量**
- $O(1)$

### prod

```cpp
typename M::S DisjointSparseTable<M>::prod(i32 l, i32 r) const
```

区間 $[l, r)$ の積を返します。要素の順序は保持され、空区間では `M::e()` を返します。

**制約**
- $0 \le l \le r \le N$

**計算量**
- $O(1)$
