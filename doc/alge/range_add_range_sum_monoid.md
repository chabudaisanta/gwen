---
title: 区間加算・区間和モノイド
documentation_of: //include/gwen/alge/range_add_range_sum_monoid.hpp
---

# 区間加算・区間和モノイド

区間加算と区間和を遅延評価セグメント木で扱う作用付きモノイドを提供する。

## インクルード

```cpp
#include "gwen/alge/range_add_range_sum_monoid.hpp"
```

## range_add_range_sum_monoid<T>

状態 `S` は `{sum, len}`、作用 `F` は `T` とする。

**制約**

- `T` は `semiring` を満たす。
- `op`、`mapping`、`composition` で行う全ての加算と乗算の結果が `T` で表現できる。特に、遅延作用の合成、区間長の加算、作用と区間長の積、区間和の加算を含む。

**計算量**

- 各演算は $O(1)$。

### op(a, b)

2区間の和と長さをそれぞれ加算して結合する。

### e()

空区間 `{0, 0}` を返す。

### unit(x)

値 `x` を含む長さ1の状態 `{x, 1}` を返す。

### mapping(f, x)

状態 `x` の区間全体に `f` を加算し、和を `x.sum + f * x.len` にする。

### composition(f, g)

先に `g`、後に `f` を適用する作用を `f + g` で返す。

### id()

加算しない作用 `0` を返す。
