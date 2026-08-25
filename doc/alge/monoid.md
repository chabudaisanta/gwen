---
title: monoid
documentation_of: //include/gwen/alge/monoid.hpp
---

# モノイド

代数構造の一つである「モノイド (Monoid)」のコンセプトと、よく使われる基本的なモノイド構造体を提供するヘッダーです。
モノイドは、集合 $S$ とその上の二項演算 $\cdot$ について、結合律 $(a \cdot b) \cdot c = a \cdot (b \cdot c)$ と単位元の存在 $e \cdot a = a \cdot e = a$ を満たす構造です。

## インクルード

```cpp
#include "gwen/alge/monoid.hpp"
```

## monoid
```cpp
template <typename T>
concept monoid = requires {
    typename T::S;
    { T::op(std::declval<typename T::S>(), std::declval<typename T::S>()) } -> std::convertible_to<typename T::S>;
    { T::e() } -> std::convertible_to<typename T::S>;
};
```
データ構造（セグメント木など）の要素として載せるための構文要件です。

**制約**

- `M::op` は結合律を満たす。
- `M::e()` は `M::op` の左右の単位元である。
- これらの代数法則は Concept 自体では検査されないため、利用者が保証する。

**計算量**

- コンパイル時の構文検査のみであり、実行時コストはありません。

## M::op(a, b)

```cpp
static S op(S a, S b);
```

2つの要素を結合します。

**制約**

- `a` と `b` は `M::S` 型である。

**計算量**

- 定義済みモノイドでは、要素型の対応する演算1回分です。

## M::e()

```cpp
static S e();
```

二項演算の単位元を返します。

**制約**

- 返り値は `M::op` の左右の単位元である。

**計算量**

- $O(1)$

## 定義済みモノイド構造体

すべての構造体は以下の形式で定義されており、型 `S`、演算 `op`、単位元 `e` を持ちます。
- `sum_monoid<T>`: 加法モノイド ($a + b$)
- `xor_monoid<T>`: 排他的論理和モノイド ($a \oplus b$)
- `min_monoid<T>`: 最小値モノイド。単位元は浮動小数点型では正の無限大、それ以外では `numeric_limits<T>::max()`
- `max_monoid<T>`: 最大値モノイド。単位元は浮動小数点型では負の無限大、それ以外では `numeric_limits<T>::lowest()`
- `minmax_monoid<T>`: 最小値と最大値のペアを管理するモノイド
- `gcd_monoid<T>`: 最大公約数モノイド ($\gcd(a, b)$)
- `affine_monoid<T>`: 一次関数 $f(x) = ax + b$ の合成を管理するモノイド（遅延セグメント木などで利用）
- `mul_monoid<T>`: 乗法モノイド。単位元は `T(1)`
- `and_monoid<T>`: ビットANDモノイド。単位元は全ビット1
- `no_op_monoid`: `S` と `F` に `std::monostate` を使う一元の作用付きモノイド

**制約**

- `min_monoid`、`max_monoid`、`minmax_monoid` に浮動小数点型を使う場合、要素に NaN を含めない。
- 各要素型の演算が、対応するモノイドの代数法則を満たす。

**計算量**

- いずれも要素型の対応する演算1回分です。
