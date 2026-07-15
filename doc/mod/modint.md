---
title: Dynamic ModInt
documentation_of: //include/gwen/mod/modint.hpp
---

# Dynamic ModInt (実行時モジュラ演算)

実行時に法 $M$ を設定できる ModInt 構造体 `DynamicModInt64` を提供します。内部で Montgomery 乗算を用いることで、演算が高速化されています。また、本ヘッダ内で `modint` および `ring` コンセプトも定義しています。

## コンセプト

```cpp
template <typename T> concept ring
template <typename T> concept modint
```

`modint` コンセプトは、四則演算、`val()`, `T::mod()`, `inv()`, `pow(n)` など、ACL互換のインターフェースを持つことを要求します。

## set_mod

```cpp
static void DynamicModInt64::set_mod(u64 n)
```

クラス全体で共通の法を設定します。このクラスを使用する前に必ず呼び出す必要があります。

**制約**

- $n$ は奇数であること。
- $1 \le n < 2^{62}$ であること。

**計算量**

- $O(1)$

## コンストラクタ

```cpp
DynamicModInt64()
template <std::integral T> DynamicModInt64(T x)
```

デフォルトでは $0$ で初期化します。整数 `x` から初期化する際、自動的に $0 \le \text{val} < n$ の範囲に正規化されます。負の数からも初期化可能です。

**制約**

- 構築の前に `set_mod` が呼び出されていること。

**計算量**

- $O(1)$

## val

```cpp
u64 val() const
```

現在の値を $0 \le x < n$ の範囲で返します。

**制約**

- なし

**計算量**

- $O(1)$

## mod

```cpp
static u64 mod()
```

現在設定されている法を返します。

**制約**

- なし

**計算量**

- $O(1)$

## inv

```cpp
DynamicModInt64 inv() const
```

自身の逆元を返します。拡張ユークリッドの互除法を用いて計算されます。

**制約**

- 自身の値と法が互いに素であること。

**計算量**

- $O(\log n)$

## pow

```cpp
template <std::integral T> DynamicModInt64 pow(T x) const
```

自身の `x` 乗を返します。

**制約**

- $x \ge 0$

**計算量**

- $O(\log x)$

## 演算子

```cpp
DynamicModInt64& operator+=(const DynamicModInt64& rhs)
DynamicModInt64& operator-=(const DynamicModInt64& rhs)
DynamicModInt64& operator*=(const DynamicModInt64& rhs)
DynamicModInt64& operator/=(const DynamicModInt64& rhs)
DynamicModInt64 operator-() const
```

四則演算および単項マイナスをサポートします。`+`, `-`, `*`, `/`, `==`, `!=` もフレンド関数としてサポートされており、左辺が通常の整数型である場合の演算も可能です。

**制約**

- `/=` 演算時、`rhs` の値と法が互いに素であること。

**計算量**

- $O(1)$ (除算は $O(\log n)$)
