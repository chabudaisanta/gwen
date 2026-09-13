---
title: Stern-Brocot 木 (stern_brocot_tree.hpp)
documentation_of: //include/gwen/math/stern_brocot_tree.hpp
---

# Stern-Brocot 木

正の既約分数を Stern-Brocot 木の頂点として扱います。根は `1/1` です。

```cpp
#include "gwen/math/stern_brocot_tree.hpp"

using Tree = gwen::SternBrocotTree<gwen::i64>;
```

## 制約

- `T` は `bool` 以外の整数型
- 分数の分子・分母は正で互いに素
- 各計算結果は `T` に収まる

## encode_path

```cpp
std::vector<std::pair<char, T>> Tree::encode_path(T p, T q)
```

根から `p/q` までの run-length 圧縮パスを返す。`L` は左の子、`R` は右の子を表し、各 run 長は正である。根 `1/1` のパスは空である。

**制約**

- `p`, `q` は正で互いに素

**計算量**

- $O(\log \max(p, q))$

## decode_path

```cpp
std::pair<T, T> Tree::decode_path(std::span<const std::pair<char, T>> path)
```

パスの表す既約分数 `{分子, 分母}` を返す。

**制約**

- 各方向は `L` または `R`
- 各 run 長は正

**計算量**

- $O(K)$。`K = path.size()`

## lca

```cpp
std::pair<T, T> Tree::lca(T p, T q, T r, T s)
```

`p/q` と `r/s` の最小共通祖先を `{分子, 分母}` で返す。

**制約**

- `p`, `q`, `r`, `s` は正で、各分数は既約

**計算量**

- $O(\log \max(p, q, r, s))$

## ancestor

```cpp
std::pair<T, T> Tree::ancestor(T k, T p, T q)
```

根を深さ `0` とした深さ `k` の `p/q` の祖先を `{分子, 分母}` で返す。存在しない場合は `{0, 0}` を返す。

**制約**

- `k` は非負
- `p`, `q` は正で互いに素

**計算量**

- $O(\log \max(p, q))$

## range

```cpp
std::tuple<T, T, T, T> Tree::range(T p, T q)
```

`a/b < p/q < c/d` を満たす Stern-Brocot 区間の左右端 `{a, b, c, d}` を返す。端には番兵 `0/1`、`1/0` が含まれ得る。

**制約**

- `p`, `q` は正で互いに素

**計算量**

- $O(\log \max(p, q))$
