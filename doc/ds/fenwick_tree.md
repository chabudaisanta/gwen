---
title: FenwickTree
documentation_of: //include/gwen/ds/fenwick_tree.hpp
---

## 概要

アーベル群を要素に持つ長さ $N$ の配列に対し、一点更新とプレフィックス・区間取得を $O(\log N)$ で行うデータ構造 (Binary Indexed Tree) です。

## コンストラクタ

```cpp
FenwickTree()
FenwickTree(i32 N)
FenwickTree(const std::vector<S>& vec)
```
- 1つ目: サイズ $0$ で初期化します。
- 2つ目: サイズ $N$ で、すべての要素を単位元 `Abel::e()` で初期化します。
- 3つ目: 配列 `vec` を元に初期化します。

**計算量**
- 1つ目: $O(1)$
- 2つ目、3つ目: $O(N)$

## add

```cpp
void add(i32 p, S x)
```
要素 $p$ に対して `x` を作用させます（$a_p \gets a_p \cdot x$）。

**制約**
- $0 \le p < N$

**計算量**
- $O(\log N)$

## sum

```cpp
S sum(i32 r) const
S sum(i32 l, i32 r) const
```
- 1つ目: 区間 $[0, r)$ の総和を返します。
- 2つ目: 区間 $[l, r)$ の総和を返します。内部で `Abel::inv` を呼び出します。

**制約**
- 1つ目: $0 \le r \le N$
- 2つ目: $0 \le l \le r \le N$

**計算量**
- 共に $O(\log N)$

## max_right

```cpp
template <class F>
i32 max_right(F&& f) const
template <class F>
i32 max_right(i32 l, F&& f) const
```
- 1つ目: $f(sum(0, r)) = \text{true}$ となる最大の $r$ を返します。
- 2つ目: $f(sum(l, r)) = \text{true}$ となる最大の $r$ を返します。

**制約**
- $f(sum(0, r))$ または $f(sum(l, r))$ が $r$ に対して単調性を持つこと

**計算量**
- 共に $O(\log N)$

## to_vec

```cpp
std::vector<S> to_vec() const
```
現在の木が表す配列を復元して返します。

**計算量**
- $O(N)$

## dump

```cpp
std::string dump() const
```
デバッグ用にデータ構造の内部状態を文字列として返します。

**計算量**
- $O(N)$
