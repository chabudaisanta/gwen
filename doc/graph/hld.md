---
title: Heavy-Light Decomposition (HL分解)
documentation_of: //include/gwen/graph/hld.hpp
---

# Hld

根付き木を Heavy Edge の連鎖（Heavy Path）に分解し、任意のパスを $O(\log N)$ 個の区間に分割するためのデータ構造です。
1つの根のみをサポートしているため、森の場合は連結成分ごとに HLD を構築してください。

## コンストラクタ

```cpp
template <graph G>
class Hld;

Hld(const G& g, i32 root = 0)
```
- 対象となるグラフ $g$ と根 $root$ から HL 分解を構築します。
- $g$ は木である必要があります。内部では親への逆流を防ぐ形で DFS を行います。
- **計算量**: $O(N)$

## メソッド

### get_lca

```cpp
i32 get_lca(i32 u, i32 v) const
```
- 頂点 $u, v$ の最小共通祖先 (LCA) を返します。
- **計算量**: $O(\log N)$

### get_dist

```cpp
i32 get_dist(i32 u, i32 v) const
```
- 頂点 $u$ と $v$ の距離（エッジ数）を返します。
- **計算量**: $O(\log N)$

### path

```cpp
std::vector<std::pair<i32, i32>> path(i32 u, i32 v) const
```
- パス $u \to v$ を、セグメント木などで管理しやすいように `in` 配列上のインデックスの区間の列に分解します。
- 返り値の各要素は半開区間 $[l, r)$ を表します。
- **計算量**: $O(\log N)$ 区間

### kth_ancestor

```cpp
i32 kth_ancestor(i32 v, i32 k) const
```
- 頂点 $v$ の $k$ 個上の祖先を返します。祖先が存在しない場合は `-1` を返します。
- **計算量**: $O(\log N)$

### kth_on_path

```cpp
i32 kth_on_path(i32 u, i32 v, i32 k) const
```
- パス $u \to v$ 上で、$u$ から $k$ 番目の頂点（$u$ 自身を $0$ 番目とする）を返します。
- パス長を超える場合は `-1` を返します。
- **計算量**: $O(\log N)$

### subtree

```cpp
std::pair<i32, i32> subtree(i32 v) const
```
- 頂点 $v$ を根とする部分木に対応する `in` 配列上の半開区間 $[l, r)$ を返します。
- **計算量**: $O(1)$

### idx / vertex

```cpp
i32 idx(i32 v) const
i32 vertex(i32 i) const
```
- `idx`: 頂点 $v$ の Euler Tour における行きがけ順（セグメント木用インデックス）を返します。
- `vertex`: インデックス $i$ に対応する頂点番号を返します。
- **計算量**: $O(1)$
