---
title: WeightedDsu
documentation_of: //include/gwen/graph/weighted_dsu.hpp
---

# WeightedDsu

ポテンシャル付き (重み付き) の Union-Find データ構造です。
各連結成分において、要素間の相対的な重みの差分を管理することができます。

## コンストラクタ

```cpp
WeightedDsu<Abel> dsu(int n)
```

- 頂点数 $n$ の重み付き Union-Find を構築します。初期状態ではすべての頂点が別々のグループに属します。

**制約**

- `Abel` は `gwen::abel` コンセプト (加群) を満たすこと。
- $0 \le n \le 10^8$

**計算量**

- $O(n)$

## leader

```cpp
int dsu.leader(int a)
```

頂点 $a$ の属する連結成分の代表元を返します。

**制約**

- $0 \le a < n$

**計算量**

- 均し $O(\alpha(n))$

## diff

```cpp
S dsu.diff(int a, int b)
```

頂点 $a$ と $b$ の重みの差分 $P(a) - P(b)$ を返します。

**制約**

- $0 \le a < n$
- $0 \le b < n$
- $a$ と $b$ は同じ連結成分に属していること（`same(a, b)` が `true`）。

**計算量**

- 均し $O(\alpha(n))$

## same

```cpp
bool dsu.same(int a, int b)
```

頂点 $a$ と $b$ が同じ連結成分に属するか判定します。

**制約**

- $0 \le a < n$
- $0 \le b < n$

**計算量**

- 均し $O(\alpha(n))$

## potential

```cpp
S dsu.potential(int a)
```

頂点 $a$ の代表元との重みの差分 $P(a) - P(\text{leader}(a))$ を返します。

**制約**

- $0 \le a < n$

**計算量**

- 均し $O(\alpha(n))$

## merge

```cpp
int dsu.merge(int a, int b, S w)
```

$P(a) - P(b) = w$ となるように頂点 $a$ と $b$ を同じ連結成分にマージします。
成功時は新しい代表元のインデックスを返します。既に同じ成分にあり、矛盾する場合は `-1` を返します。

**制約**

- $0 \le a < n$
- $0 \le b < n$

**計算量**

- 均し $O(\alpha(n))$

## size

```cpp
int dsu.size(int a)
```

頂点 $a$ が属する連結成分のサイズを返します。

**制約**

- $0 \le a < n$

**計算量**

- 均し $O(\alpha(n))$

## groups

```cpp
std::vector<std::vector<int>> dsu.groups()
```

グラフを連結成分に分け、その情報を返します。

**計算量**

- $O(n)$

