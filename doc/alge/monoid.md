---
title: monoid
documentation_of: //include/gwen/alge/monoid.hpp
---

# monoid.hpp

代数構造の一つである「モノイド (Monoid)」のコンセプトと、よく使われる基本的なモノイド構造体を提供するヘッダーです。
モノイドは、集合 $S$ とその上の二項演算 $\cdot$ について、結合律 $(a \cdot b) \cdot c = a \cdot (b \cdot c)$ と単位元の存在 $e \cdot a = a \cdot e = a$ を満たす構造です。

## インクルード

```cpp
#include "gwen/alge/monoid.hpp"
```

## コンセプト
```cpp
template <typename T>
concept monoid = requires {
    typename T::S;
    { T::op(std::declval<typename T::S>(), std::declval<typename T::S>()) } -> std::convertible_to<typename T::S>;
    { T::e() } -> std::convertible_to<typename T::S>;
};
```
データ構造（セグメント木など）の要素として載せるための要件です。

## 定義済みモノイド構造体

すべての構造体は以下の形式で定義されており、型 `S`、演算 `op`、単位元 `e` を持ちます。
- `sum_monoid<T>`: 加法モノイド ($a + b$)
- `xor_monoid<T>`: 排他的論理和モノイド ($a \oplus b$)
- `min_monoid<T>`: 最小値モノイド ($\min(a, b)$)
- `max_monoid<T>`: 最大値モノイド ($\max(a, b)$)
- `minmax_monoid<T>`: 最小値と最大値のペアを管理するモノイド
- `gcd_monoid<T>`: 最大公約数モノイド ($\gcd(a, b)$)
- `affine_monoid<T>`: 一次関数 $f(x) = ax + b$ の合成を管理するモノイド（遅延セグメント木などで利用）
- `no_op_monoid<T>`: 何も操作を行わないダミーモノイド
