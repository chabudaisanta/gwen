---
title: Matrix (行列)
documentation_of: //include/gwen/math/matrix.hpp
---

## 概要

行列 (Matrix) とその各種演算（加減乗除、累乗、行列式、階数、逆行列など）をサポートする汎用的なデータ構造です。
`matrix_field` コンセプトを満たす任意の体 (Field) を要素に持つことができます（例えば、有理数、実数、あるいは `DynamicModInt64` などのモジュラ整数など）。

## クラス

### Matrix

```cpp
template <typename T> class Matrix
```

## メソッド

### コンストラクタ

```cpp
Matrix()
Matrix(i32 n)
Matrix(i32 n, i32 m, T val = T{})
```

$n \times m$ 行列を作成します。正方行列の場合は `Matrix(n)` のように省略できます。初期値は `val` です。

**計算量**

- $O(n \times m)$

### 行列のサイズ情報

```cpp
i32 rows() const
i32 cols() const
bool is_square() const
```

行数、列数、および正方行列かどうかを取得します。

**計算量**

- $O(1)$

### 演算

```cpp
Matrix operator+(const Matrix& other) const
Matrix operator-(const Matrix& other) const
Matrix operator*(const Matrix& other) const
Matrix operator*(const U& scalar) const
Matrix& operator+=(const Matrix& other)
Matrix& operator-=(const Matrix& other)
Matrix& operator*=(const Matrix& other)
Matrix& operator*=(const U& scalar)
static Matrix add(const Matrix& a, const Matrix& b)
static Matrix sub(const Matrix& a, const Matrix& b)
static Matrix mul(const Matrix& a, const Matrix& b)
```

行列同士の加算、減算、乗算、およびスカラー倍をサポートします。演算子によるものと、静的メソッド `add`, `sub`, `mul` が利用可能です。代入演算子 (`+=`, `-=`, `*=`) もサポートしています。

**計算量**

- 加算・減算・スカラー倍: $O(n \times m)$
- 乗算: 行列 $A$ が $n \times k$、$B$ が $k \times m$ のとき $O(n \times m \times k)$

### 転置行列

```cpp
Matrix transpose() const
```

転置行列を返します。

**計算量**

- $O(n \times m)$

### 行列式

```cpp
T det() const
```

掃き出し法（ガウスの消去法）を用いて行列式を求めます。要素型 `T` は `matrix_field` を満たす必要があります。

**制約**

- 正方行列であること。

**計算量**

- $O(n^3)$

### 階数

```cpp
i32 rank() const
```

行列の階数 (rank) を求めます。

**計算量**

- $O(n \times m \times \min(n, m))$

### トレース

```cpp
T trace() const
```

行列のトレース (対角成分の和) を計算します。

**制約**

- 正方行列であること。

**計算量**

- $O(n)$

### 逆行列

```cpp
Matrix inverse() const
```

掃き出し法を用いて逆行列を求めます。逆行列が存在しない場合は空の行列（行数・列数がともに0）を返します。

**制約**

- 正方行列であること。

**計算量**

- $O(n^3)$

### 単位行列

```cpp
static Matrix eye(i32 n)
```

サイズ $n$ の単位行列を作成します。

**計算量**

- $O(n^2)$

### 累乗

```cpp
template <std::integral U> static Matrix pow(Matrix a, U k)
```

行列 `a` の `k` 乗を計算します。

**制約**

- `a` が正方行列であること。

**計算量**

- $O(n^3 \log k)$
