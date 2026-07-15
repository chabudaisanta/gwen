---
title: 素数・素因数分解 (prime.hpp)
documentation_of: //include/gwen/math/prime.hpp
---

# 素数・素因数分解 (prime.hpp)

素数判定、素因数分解に関する高速なアルゴリズムを提供するモジュールです。用途に応じて最適なクラス・関数を使い分けてください。

## アルゴリズムと使い分けの指針

本モジュールでは、以下の2通りのアプローチを提供しています。

1. **LPFテーブル (最小素因数テーブル)**
   - 複数のテストケースで大量の小さな数（$10^7 \sim 10^8$ 以下）を素因数分解・素数判定するのに適しています。
   - **`LpfTable` (エラトステネスの篩)**: 前計算 $O(N \log\log N)$ ですが、メモリアクセスの局所性が高いため定数倍が極めて軽く、通常はこちらの利用を推奨します。
   - **`LinearLpfTable` (線形篩)**: 前計算 $O(N)$ ですが、メモリアクセスがランダムになるため実環境では定数倍がやや重くなります。
2. **汎用関数 (`is_prime`, `factorize`)**
   - $2^{64}-1$ までの巨大な数を単発または少数回処理するのに適しています。
   - `is_prime` は **Miller-Rabin法** を、`factorize` は **ポラード・ロー法 (Pollard's rho)** を用いることで、巨大な合成数でも $O(n^{1/4})$ という圧倒的な速度で分解可能です。

## LpfTable

```cpp
LpfTable lpf(int n)
```

指定された上限 `n` 以下の数に対して、エラトステネスの篩を用いて最小素因数 (LPF) テーブルを初期化します。
定数倍が優秀でキャッシュ効率が良いため、通常はこちらの利用を推奨します。マルチテストケースの場合はプログラム全体で1回だけインスタンス化して使い回すことを推奨します。

**制約**

- $0 \le n \le 10^8$ 程度

**計算量**

- $O(n \log \log n)$

## LpfTable::is_prime

```cpp
bool lpf.is_prime(int x)
```

`x` が素数であるかを判定します。

**制約**

- $0 \le x \le n$

**計算量**

- $O(1)$

## LpfTable::factorize

```cpp
std::vector<int> lpf.factorize(int x)
```

`x` を素因数分解し、昇順にソートされた素因数のリストを返します。

**制約**

- $1 \le x \le n$

**計算量**

- $O(\log x)$

## LinearLpfTable

```cpp
LinearLpfTable llpf(int n)
```

指定された上限 `n` 以下の数に対して、線形篩を用いて最小素因数 (LPF) テーブルを初期化します。
漸近計算量では優れていますが、メモリアクセスの局所性が低いため実環境での定数倍は重めです（ベンチマーク・比較用）。

**制約**

- $0 \le n \le 10^8$ 程度

**計算量**

- $O(n)$

## LinearLpfTable::is_prime

```cpp
bool llpf.is_prime(int x)
```

`x` が素数であるかを判定します。

**制約**

- $0 \le x \le n$

**計算量**

- $O(1)$

## LinearLpfTable::factorize

```cpp
std::vector<int> llpf.factorize(int x)
```

`x` を素因数分解し、昇順にソートされた素因数のリストを返します。

**制約**

- $1 \le x \le n$

**計算量**

- $O(\log x)$

## is_prime

```cpp
bool is_prime(u64 n)
```

Miller-Rabin 法を用いて、任意の整数に対して確定的な素数判定を行います。内部で `DynamicModInt64` を利用し、法を設定するためスレッドセーフではありません。

**制約**

- $0 \le n < 2^{64}$

**計算量**

- $O(\log n)$

## factorize

```cpp
std::vector<u64> factorize(u64 n)
```

ポラード・ロー法 (Pollard's rho) と Miller-Rabin 法を組み合わせて、任意の整数を素因数分解します。昇順にソートされた素因数のリストを返します。内部で `DynamicModInt64` の法を設定するためスレッドセーフではありません。

**制約**

- $0 \le n < 2^{64}$

**計算量**

- 期待値 $O(n^{1/4})$
