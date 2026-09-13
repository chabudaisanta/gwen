---
title: Formal Power Series (形式的冪級数)
documentation_of: //include/gwen/math/formal_power_series.hpp
---

## 概要

`FormalPowerSeries<T>` は係数を低次から順に保持する形式的冪級数です。末尾の 0 は自動で削除しません。
係数型 `T` は `field` を満たす必要があります。`modint998244353` では NTT と疎な漸化式を使い分け、それ以外の型では愚直法を使用します。

## コンストラクタ

```cpp
explicit FormalPowerSeries(std::vector<T> coeffs = {})
FormalPowerSeries(std::initializer_list<T> coeffs)
```

低次から並べた係数列で構築します。

**計算量**

- $O(n)$

## size / empty

```cpp
i32 size() const
bool empty() const
```

係数列の長さ、または空かどうかを返します。

**制約**

- 係数列の長さは `i32` の最大値以下であること。

**計算量**

- $O(1)$

## operator[]

```cpp
T& operator[](i32 i)
const T& operator[](i32 i) const
```

$x^i$ の係数を返します。

**制約**

- $0 \leq i < \mathrm{size}()$

**計算量**

- $O(1)$

## prefix

```cpp
FormalPowerSeries prefix(i32 n) const
```

先頭 `min(size(), n)` 項を返します。元の係数列が短い場合は 0 埋めしません。

**制約**

- $n \geq 0$

**計算量**

- $O(\min(\mathrm{size}(), n))$

## 加減算・スカラー演算

```cpp
FormalPowerSeries operator-() const
FormalPowerSeries& operator+=(const FormalPowerSeries& rhs)
FormalPowerSeries& operator-=(const FormalPowerSeries& rhs)
FormalPowerSeries& operator*=(const T& scalar)
FormalPowerSeries& operator/=(const T& scalar)
```

非メンバの `+`, `-`, FPSとスカラーの `*`, `/` も使用できます。加減算は長い方の係数列の長さを結果の長さとします。

**制約**

- 除算では `scalar != T(0)` であること。

**計算量**

- $O(\max(n,m))$

## 乗算

```cpp
FormalPowerSeries& operator*=(const FormalPowerSeries& rhs)
FormalPowerSeries operator*(FormalPowerSeries lhs, const FormalPowerSeries& rhs)
```

通常の多項式積を返します。どちらかが空の場合は空のFPSを返します。

**制約**

- `modint998244353` では、空でない結果の長さ以上となる最小の2冪が $2^{23}$ 以下であること。

**計算量**

- `T = modint998244353`: $O(N \log N)$。$N$ は変換長です。
- その他の `field`: $O(nm)$

## derivative

```cpp
FormalPowerSeries derivative() const
```

微分を返します。

**計算量**

- $O(n)$

## integral

```cpp
FormalPowerSeries integral() const
```

積分定数を 0 とした積分を返します。

**制約**

- 係数体で $1,2,\ldots,n$ が 0 でなく可逆であること。
- `T = modint998244353` では $n < 998244353$。

**計算量**

- $O(n)$

## inv

```cpp
FormalPowerSeries inv(i32 n) const
```

$f(x)g(x) \equiv 1 \pmod{x^n}$ となる、長さ `n` の $g(x)$ を返します。`n == 0` では空を返します。

**制約**

- $n \geq 0$
- $n > 0$ では係数列が空でなく、$f(0) \neq 0$。

**計算量**

- `T = modint998244353`: $O(n \log n)$。非0項数を $k$ とする疎な入力では $O(kn)$。
- その他の `field`: $O(n^2)$

## log

```cpp
FormalPowerSeries log(i32 n) const
```

$\log f(x)$ を $x^n$ 未満で打ち切り、長さ `n` で返します。`n == 0` では空を返します。

**制約**

- $n \geq 0$
- $n > 0$ では係数列が空でなく、$f(0)=1$。
- 係数体で $1,2,\ldots,n-1$ が 0 でなく可逆であること。

**計算量**

- `T = modint998244353`: $O(n \log n)$。非0項数を $k$ とする疎な入力では $O(kn)$。
- その他の `field`: $O(n^2)$

## exp

```cpp
FormalPowerSeries exp(i32 n) const
```

$\exp f(x)$ を $x^n$ 未満で打ち切り、長さ `n` で返します。`n == 0` では空を返します。

**制約**

- $n \geq 0$
- $n > 0$ では係数列が空、または $f(0)=0$。
- 係数体で $1,2,\ldots,n-1$ が 0 でなく可逆であること。
- `T = modint998244353` の密な入力では $n \leq 2^{23}$。

**計算量**

- `T = modint998244353`: $O(n \log n)$。非0項数を $k$ とする疎な入力では $O(kn)$。
- その他の `field`: $O(n^2)$
