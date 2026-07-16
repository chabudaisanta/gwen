---
title: Lazy Segment Tree
documentation_of: //include/gwen/ds/lazy_segment_tree.hpp
---

## 概要

作用付きモノイドを要素に持ち、区間更新と区間積の取得を $O(\log N)$ で行うデータ構造です。

## コンストラクタ

```cpp
LazySegmentTree<M>()
LazySegmentTree<M>(int n)
LazySegmentTree<M>(const std::vector<typename M::S>& v)
```
- `M`: 作用付きモノイド（`gwen::acted_monoid` コンセプトを満たす構造体）

**制約**
- $0 \le n \le 10^8$

**計算量**
- $O(n)$

## メソッド

### set

```cpp
void LazySegmentTree<M>::set(int p, typename M::S x)
```
位置 `p` の要素を `x` に更新します。

**制約**
- $0 \le p < n$

**計算量**
- $O(\log n)$

### get

```cpp
typename M::S LazySegmentTree<M>::get(int p)
```
位置 `p` の要素を取得します。

**制約**
- $0 \le p < n$

**計算量**
- $O(\log n)$

### prod

```cpp
typename M::S LazySegmentTree<M>::prod(int l, int r)
```
区間 $[l, r)$ の積を取得します。$l = r$ の場合は `M::e()` を返します。

**制約**
- $0 \le l \le r \le n$

**計算量**
- $O(\log n)$

### all_prod

```cpp
typename M::S LazySegmentTree<M>::all_prod()
```
区間 $[0, n)$ の積を取得します。

**計算量**
- $O(1)$

### apply

```cpp
void LazySegmentTree<M>::apply(int p, typename M::F f)
void LazySegmentTree<M>::apply(int l, int r, typename M::F f)
```
位置 `p`、または区間 $[l, r)$ の要素に作用 `f` を適用します。

**制約**
- $0 \le p < n$
- $0 \le l \le r \le n$

**計算量**
- $O(\log n)$

### max_right

```cpp
template <bool (*g)(typename M::S)> int LazySegmentTree<M>::max_right(int l)
template <class G> int LazySegmentTree<M>::max_right(int l, G g)
```
`g(prod(l, r))` が `true` となる最大の `r` を返します。

**制約**
- $0 \le l \le n$
- `g(M::e()) == true`

**計算量**
- $O(\log n)$

### min_left

```cpp
template <bool (*g)(typename M::S)> int LazySegmentTree<M>::min_left(int r)
template <class G> int LazySegmentTree<M>::min_left(int r, G g)
```
`g(prod(l, r))` が `true` となる最小の `l` を返します。

**制約**
- $0 \le r \le n$
- `g(M::e()) == true`

**計算量**
- $O(\log n)$
