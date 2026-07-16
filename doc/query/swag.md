---
title: SWAG (Sliding Window Aggregation)
documentation_of: //include/gwen/query/swag.hpp
---

## 概要

SWAG (Sliding Window Aggregation) は、スライディングウィンドウ（尺取り法など、区間の左端・右端が単調に移動するクエリ）に対して、結合律を満たす演算（モノイド）の区間畳み込みを $O(1)$ amortized で行うデータ構造です。逆元を必要としないため、アフィン変換や行列の積などの非可換な演算にも適用可能です。

本モジュールには以下の2つのクラスが含まれます。
1. `SlidingWindowAggregation`: オフラインの区間クエリを処理するクラス
2. `SwagDeque`: 両端キュー（Deque）としてオンラインで要素の追加・削除を行えるクラス

---

## SlidingWindowAggregation

### コンストラクタ

```cpp
SlidingWindowAggregation<Monoid> swag(i32 n)
```

対象となる配列の要素数 $n$ で初期化します。`Monoid` は `gwen::monoid` コンセプトを満たす型です。

**制約**

- $0 \le n \le 10^8$

**計算量**

- $O(1)$

### add_query

```cpp
void add_query(i32 l, i32 r)
```

クエリとして区間 $[l, r)$ を追加します。

**制約**

- $0 \le l \le r \le n$

**計算量**

- $O(1)$

### solve

```cpp
std::vector<S> solve(const std::vector<S>& data, bool sorted = false)
```

追加されたすべてのクエリを処理し、各クエリに対する演算結果の配列を返します。
`sorted` に `false` を指定した場合、クエリは内部でカウンティングソートを用いて $L$ の昇順かつ $R$ の昇順になるようソートされます。

**制約**

- `sorted = false` の場合、ソート後のクエリが $L_i \le L_{i+1}$ かつ $R_i \le R_{i+1}$ を満たす（完全に包含される区間によって単調性が崩れない）こと。

**計算量**

- $N$ は `data` の要素数、$Q$ はクエリ数として、全体の計算量は $O(N + Q)$ amortized です。

---

## SwagDeque

### コンストラクタ

```cpp
SwagDeque<Monoid> dq()
```

空の SWAG Deque を作成します。

**計算量**

- $O(1)$

### push_back / push_front

```cpp
void push_back(S x)
void push_front(S x)
```

キューの末尾 / 先頭に要素 `x` を追加します。

**計算量**

- $O(1)$ amortized

### pop_back / pop_front

```cpp
void pop_back()
void pop_front()
```

キューの末尾 / 先頭の要素を削除します。

**制約**

- キューが空でないこと。

**計算量**

- $O(1)$ amortized

### fold

```cpp
S fold() const
```

現在のキュー内のすべての要素を先頭から末尾の順に畳み込んだ結果を返します。

**計算量**

- $O(1)$
