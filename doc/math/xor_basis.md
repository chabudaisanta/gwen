---
title: XorBasis (XOR基底 / Noshi基底)
documentation_of: //include/gwen/math/xor_basis.hpp
---

## 概要

整数の集合から、それらの排他的論理和 (XOR) によって生成される部分空間の線形基底を管理します。
内部では常に基底が降順にソートされた状態を保ち、新しい要素を追加する際は `std::min(x, x ^ b)` (chmin) を用いて高速に掃き出しを行います。このアルゴリズムは競技プログラミングにおいて「Noshi基底」とも呼ばれます。

## クラス

### XorBasis

```cpp
template <std::unsigned_integral T> class XorBasis
```

## メソッド

### コンストラクタ

```cpp
XorBasis()
```

空の基底を作成します。

**計算量**

- $O(1)$

### サイズ取得・空判定

```cpp
i32 size() const noexcept
bool empty() const noexcept
```

現在の基底の要素数や、空であるかどうかを判定します。

**計算量**

- $O(1)$

### 要素の追加

```cpp
bool insert(T x)
```

要素 `x` を追加します。既存の基底による XOR で表現できない独立した要素であれば、基底に追加して `true` を返します。すでに表現可能であれば `false` を返します。

**計算量**

- $O(B)$ （$B$ は現在の基底のサイズ。最大でも型のビット幅まで）

### 表現可能性の判定

```cpp
bool can_form(T x) const
```

要素 `x` が現在の基底の XOR で表現可能かどうかを判定します。

**計算量**

- $O(B)$

### 基底のマージ

```cpp
static XorBasis op(XorBasis a, XorBasis b)
```

2つの基底 `a` と `b` をマージして、新たな基底を生成します。サイズが小さい方の要素を大きい方の基底に1つずつ追加することで最適化されています。

**計算量**

- $O(B_1 \times B_2)$ （$B_1, B_2$ はそれぞれの基底のサイズ）

### 単位元

```cpp
static XorBasis e()
```

モノイドとしての単位元（空の基底）を返します。
