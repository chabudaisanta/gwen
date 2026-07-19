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

# Static ModInt (コンパイル時モジュラ演算)

コンパイル時に法 `Mod` が確定している場合に用いる ModInt 構造体 `StaticModInt64<Mod>` を提供します。こちらも内部で Montgomery 乗算を用いますが、必要なパラメータが `constexpr` でコンパイル時に計算されるため、`set_mod` の呼び出しが不要であり、実行時のパフォーマンスも最適化されます。

## エイリアス

競技プログラミング等で頻出する法に対するエイリアスが用意されています。

```cpp
using modint998244353 = StaticModInt64<998244353>;
using modint1000000007 = StaticModInt64<1000000007>;
```

## クラス定義

```cpp
template <u64 Mod>
struct StaticModInt64
```

**制約**

- `Mod` は奇数であること。
- $1 \le \text{Mod} < 2^{62}$ であること。

## コンストラクタ / メソッド

基本的な使い方は `DynamicModInt64` と同一です。以下のメソッドが提供されています。

- `constexpr StaticModInt64()`
- `template <std::integral T> constexpr StaticModInt64(T x)`
- `constexpr u64 val() const`
- `static constexpr u64 mod()`
- `constexpr StaticModInt64 inv() const`
- `template <std::integral T> constexpr StaticModInt64 pow(T x) const`

全てのメソッドは `constexpr` に対応しており、コンパイル時計算でも利用可能です。計算量および制約は `DynamicModInt64` に準じます。
