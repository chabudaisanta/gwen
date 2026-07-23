---
title: Segment Tree
documentation_of: //include/gwen/ds/segment_tree.hpp
---

## 概要

モノイドを要素に持ち、一点更新と区間積の取得を $O(\log N)$ で行うデータ構造です。

## コンストラクタ

```cpp
SegmentTree<M>()
SegmentTree<M>(int n)
SegmentTree<M>(const std::vector<typename M::S>& v)
```
- `M`: モノイド（`gwen::monoid` コンセプトを満たす構造体）

**制約**
- $0 \le n \le 10^8$

**計算量**
- $O(n)$

## メソッド

### set

```cpp
void SegmentTree<M>::set(int p, typename M::S x)
```
位置 `p` の要素を `x` に更新します。

**制約**
- $0 \le p < n$

**計算量**
- $O(\log n)$

### get

```cpp
typename M::S SegmentTree<M>::get(int p)
```
位置 `p` の要素を取得します。

**制約**
- $0 \le p < n$

**計算量**
- $O(1)$

### prod

```cpp
typename M::S SegmentTree<M>::prod(int l, int r)
```
区間 $[l, r)$ の積を取得します。$l = r$ の場合は `M::e()` を返します。

**制約**
- $0 \le l \le r \le n$

**計算量**
- $O(\log n)$

### all_prod

```cpp
typename M::S SegmentTree<M>::all_prod()
```
区間 $[0, n)$ の積を取得します。

**計算量**
- $O(1)$

### max_right

```cpp
template <bool (*g)(typename M::S)> int SegmentTree<M>::max_right(int l)
template <class G> int SegmentTree<M>::max_right(int l, G g)
```
`g(prod(l, r))` が `true` となる最大の `r` を返します。

**制約**
- $0 \le l \le n$
- `g(M::e()) == true`

**計算量**
- $O(\log n)$

### min_left

```cpp
template <bool (*g)(typename M::S)> int SegmentTree<M>::min_left(int r)
template <class G> int SegmentTree<M>::min_left(int r, G g)
```
`g(prod(l, r))` が `true` となる最小の `l` を返します。

**制約**
- $0 \le r \le n$
- `g(M::e()) == true`

**計算量**
- $O(\log n)$

## **dump**

```cpp
std::string dump() const
```

デバッグ用にデータ構造の内部状態を文字列として返します。

**計算量**

- $O(N)$
