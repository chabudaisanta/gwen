---
title: FenwickTree
documentation_of: //include//gwen/ds/fenwick_tree.hpp
---

# fenwick_tree.hpp

アーベル群を要素に持つ長さ $N$ の配列に対し、一点更新とプレフィックス・区間取得を $O(\log N)$ で行うデータ構造 (Binary Indexed Tree) です。

## インクルード

```cpp
#include "gwen/ds/fenwick_tree.hpp"
```

## コンストラクタ

```cpp
FenwickTree()
```
サイズ $0$ で初期化します。

```cpp
explicit FenwickTree(i32 N)
```
サイズ $N$ で、すべての要素を単位元 `Abel::e()` で初期化します。
- 計算量: $O(N)$

```cpp
explicit FenwickTree(const std::vector<S>& vec)
```
配列 `vec` を元に初期化します。
- 計算量: $O(N)$

## メソッド

### `add`
```cpp
void add(i32 p, S x)
```
要素 $p$ に対して `x` を作用させます（$a_p \gets a_p \cdot x$）。
- 制約: $0 \le p < N$
- 計算量: $O(\log N)$

### `sum`
```cpp
S sum(i32 r) const
```
区間 $[0, r)$ の総和を返します。
- 制約: $0 \le r \le N$
- 計算量: $O(\log N)$

```cpp
S sum(i32 l, i32 r) const
```
区間 $[l, r)$ の総和を返します。内部で `Abel::inv` を呼び出します。
- 制約: $0 \le l \le r \le N$
- 計算量: $O(\log N)$

### `max_right`
二分探索（ダブリング）を用いて、特定の条件を満たす最大の右端 $r$ を求めます。

```cpp
template <class F>
i32 max_right(F&& f) const
```
$f(sum(0, r)) = \text{true}$ となる最大の $r$ を返します。
- 制約: $f(e) = \text{true}$ であり、$r$ に対して単調性を持つこと
- 計算量: $O(\log N)$

```cpp
template <class F>
i32 max_right(i32 l, F&& f) const
```
$f(sum(l, r)) = \text{true}$ となる最大の $r$ を返します。
- 制約: $f(e) = \text{true}$ であり、$r$ に対して単調性を持つこと
- 計算量: $O(\log N)$
