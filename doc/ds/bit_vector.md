---
title: BitVector
documentation_of: //include/gwen/ds/bit_vector.hpp
---

# BitVector

`BitVector` は、ビット列に対する rank / select を提供する簡潔ビットベクトルです。すべての `set` 操作の後に `build` を呼び出してください。

## コンストラクタ

```cpp
BitVector(i32 n)
```

長さ $n$ の、すべて 0 のビット列を構築します。

**制約**

- $n \geq 0$

**計算量**

- $O(n / 64)$

## set

```cpp
void set(i32 i)
```

$i$ 番目のビットを 1 にします。

**制約**

- $0 \leq i < n$

**計算量**

- $O(1)$

## get

```cpp
bool get(i32 i) const
```

$i$ 番目のビットを返します。

**制約**

- $0 \leq i < n$

**計算量**

- $O(1)$

## build

```cpp
void build()
```

rank クエリ用の内部データを構築します。すべての `set` 操作の後、`rank1`、`rank0`、`select1`、`select0` を呼ぶ前に実行してください。

**計算量**

- $O(n / 64)$

## rank1 / rank0

```cpp
i32 rank1(i32 i) const
i32 rank0(i32 i) const
```

それぞれ区間 $[0, i)$ に含まれる 1 と 0 の個数を返します。

**制約**

- $0 \leq i \leq n$

**計算量**

- $O(1)$

## select1 / select0

```cpp
i32 select1(i32 k) const
i32 select0(i32 k) const
```

それぞれ $k$ 番目（0-indexed）の 1 または 0 の位置を返します。存在しない場合は $n$ を返します。

**制約**

- なし（$k < 0$ または対象となるビット数が $k$ 以下の場合は $n$ を返します）

**計算量**

- $O(\log n)$

## size

```cpp
i32 size() const
```

ビット列の長さを返します。

**計算量**

- $O(1)$
