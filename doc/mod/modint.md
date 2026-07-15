---
title: Dynamic ModInt
documentation_of: //include/gwen/mod/modint.hpp
---

# Dynamic ModInt (実行時モジュラ演算)

実行時に法 $M$ を設定できる ModInt クラス `DynamicModInt64` を提供します。内部で Montgomery 乗算を用いることで、演算が高速化されています。

また、本ヘッダ内で `modint` および `ring` コンセプトも定義しています。

## コンセプト定義

```cpp
template <typename T>
concept ring = requires(T a, T b) { /* ... */ };

template <typename T>
concept modint = ring<T> && requires(T a, T b, unsigned long long n) { /* ... */ };
```

`modint` コンセプトは、四則演算、`val()`, `T::mod()`, `inv()`, `pow(n)` など、ACL互換のインターフェースを持つことを要求します。

## コンストラクタ / 法の設定

```cpp
static void DynamicModInt64::set_mod(u64 n)
```

クラス全体で共通の法を設定します。このクラスを使用する前に必ず呼び出す必要があります。

**制約**
- $n$ は奇数であること。
- $1 \le n < 2^{62}$ であること。

```cpp
DynamicModInt64(T x)
```

整数 $x$ で値を初期化します。負の数も指定可能です。

## 主なメソッド

### val

```cpp
u64 val() const
```

現在の値を $0 \le x < n$ の範囲で返します。

### mod

```cpp
static u64 mod()
```

現在設定されている法を返します。

### inv

```cpp
DynamicModInt64 inv() const
```

自身の逆元を返します。

**計算量**
- $O(\log n)$

### pow

```cpp
template <std::integral T>
DynamicModInt64 pow(T x) const
```

自身の $x$ 乗を返します。

**計算量**
- $O(\log x)$

## 四則演算

以下の演算子がオーバーロードされており、通常の整数のように計算できます。
`+`, `-`, `*`, `/`, `+=`, `-=`, `*=`, `/=`
