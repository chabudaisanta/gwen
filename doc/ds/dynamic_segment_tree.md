---
title: Dynamic Segment Tree
documentation_of: //include/gwen/ds/dynamic_segment_tree.hpp
---

## 概要

モノイドを要素に持ち、一点更新と区間積の取得を $O(\log N)$ で行うデータ構造です。
内部的にはポインタ（`gwen::NodePool` を用いたインデックス）ベースでノードを管理しており、座標圧縮を行わずに広大な区間（$10^9$ や $10^{18}$ など）を扱うことができます。

## コンストラクタ

```cpp
DynamicSegmentTree<M>()
DynamicSegmentTree<M>(gwen::i64 n)
```
- `M`: モノイド（`gwen::monoid` コンセプトを満たす構造体）

**制約**
- $0 \le n \le 10^{18}$

**計算量**
- $O(1)$

## メソッド

### set

```cpp
void DynamicSegmentTree<M>::set(gwen::i64 p, typename M::S x)
```
位置 `p` の要素を `x` に更新します。まだノードが存在しない場合は動的に生成されます。

**制約**
- $0 \le p < n$

**計算量**
- 時間・空間計算量: $O(\log n)$

### get

```cpp
typename M::S DynamicSegmentTree<M>::get(gwen::i64 p)
```
位置 `p` の要素を取得します。

**制約**
- $0 \le p < n$

**計算量**
- $O(\log n)$

### prod

```cpp
typename M::S DynamicSegmentTree<M>::prod(gwen::i64 l, gwen::i64 r)
```
区間 $[l, r)$ の積を取得します。$l = r$ の場合は `M::e()` を返します。

**制約**
- $0 \le l \le r \le n$

**計算量**
- $O(\log n)$

### all_prod

```cpp
typename M::S DynamicSegmentTree<M>::all_prod()
```
区間 $[0, n)$ の積を取得します。

**計算量**
- $O(1)$
