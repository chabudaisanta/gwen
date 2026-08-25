---
title: 区間アフィン変換・区間和モノイド
documentation_of: //include/gwen/alge/range_affine_range_sum_monoid.hpp
---

# 区間アフィン変換・区間和モノイド

遅延評価セグメント木などで、区間アフィン変換と区間和取得を行う作用付きモノイドです。

```cpp
#include "gwen/alge/range_affine_range_sum_monoid.hpp"
```

## range_affine_range_sum_monoid

```cpp
template <semiring T>
struct range_affine_range_sum_monoid;
```

モノイド要素 `S` は区間和 `val` と、半環の要素へ変換した区間長 `len` を保持します。作用素 `F` はアフィン変換 $f(x)=ax+b$ を表します。

**制約**

- `T` は `gwen::semiring<T>` を満たす。

**計算量**

- 型定義のみであり、実行時コストはありません。

## op(a, b)

```cpp
static constexpr S op(S a, S b);
```

隣接する2区間を結合し、区間和と区間長をそれぞれ加算します。

**制約**

- `a`、`b` は同じ配列上の隣接区間を表す。

**計算量**

- `T` の加算2回分です。

## e()

```cpp
static constexpr S e();
```

空区間を表す `{T(0), T(0)}` を返します。

**制約**

- なし

**計算量**

- $O(1)$

## unit(x)

```cpp
static constexpr S unit(T x);
```

値 `x` を持つ長さ1の区間を生成します。

**制約**

- `x` は配列の1要素を表す。

**計算量**

- $O(1)$

## mapping(f, x)

```cpp
static constexpr S mapping(F f, S x);
```

区間 `x` の全要素へ $f(y)=ay+b$ を適用します。

**制約**

- `x.len` は区間長を `T` に変換した値である。

**計算量**

- `T` の乗算2回、加算1回分です。

## composition(f, g)

```cpp
static constexpr F composition(F f, F g);
```

先に `g`、後に `f` を適用する合成 $f(g(x))$ を返します。

**制約**

- `f` と `g` は有効なアフィン変換を表す。

**計算量**

- `T` の乗算2回、加算1回分です。

## id()

```cpp
static constexpr F id();
```

恒等写像 $f(x)=x$ を表す `{T(1), T(0)}` を返します。

**制約**

- なし

**計算量**

- $O(1)$
