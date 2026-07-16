---
title: RangeAddUpdateSum
documentation_of: //include/gwen/query/range_add_update_sum.hpp
---

## 概要

`RangeAddUpdateSum` は区間加算、区間更新、区間和をサポートする汎用的なデータ構造です。内部では `LazySegmentTree` と `range_affine_range_sum_monoid` を使用して実装されています。

## コンストラクタ

```cpp
RangeAddUpdateSum<T> seg(i32 n)
RangeAddUpdateSum<T> seg(const std::vector<T>& vec)
```

- $n$ が指定された場合、長さ $n$ の初期値 $0$ の配列として構築します。
- `vec` が指定された場合、その配列を元に構築します。

**計算量**
- $O(n)$

## add

```cpp
void seg.add(i32 l, i32 r, T x)
void seg.add(i32 p, T x)
```

区間 $[l, r)$ または要素 $p$ に $x$ を加算します。

**計算量**
- $O(\log n)$

## update

```cpp
void seg.update(i32 l, i32 r, T x)
void seg.update(i32 p, T x)
```

区間 $[l, r)$ または要素 $p$ を $x$ に更新します。

**計算量**
- $O(\log n)$

## multiply

```cpp
void seg.multiply(i32 l, i32 r, T x)
void seg.multiply(i32 p, T x)
```

区間 $[l, r)$ または要素 $p$ に $x$ を乗算します。

**計算量**
- $O(\log n)$

## affine

```cpp
void seg.affine(i32 l, i32 r, T a, T b)
void seg.affine(i32 p, T a, T b)
```

区間 $[l, r)$ または要素 $p$ の値 $y$ を $a \times y + b$ に更新します。

**計算量**
- $O(\log n)$

## prod

```cpp
T seg.prod(i32 l, i32 r)
```

区間 $[l, r)$ の総和を返します。

**計算量**
- $O(\log n)$

## all_prod

```cpp
T seg.all_prod()
```

配列全体の総和を返します。

**計算量**
- $O(1)$

## get

```cpp
T seg.get(i32 p)
```

要素 $p$ の値を返します。

**計算量**
- $O(\log n)$
