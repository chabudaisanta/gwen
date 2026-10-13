---
title: RangeAddRangeMin
documentation_of: //include/gwen/query/range_add_range_min.hpp
---

# RangeAddRangeMin

区間加算と区間最小値をサポートする遅延評価セグメント木。

## インクルード

```cpp
#include "gwen/query/range_add_range_min.hpp"
```

## コンストラクタ

```cpp
RangeAddRangeMin<T> seg(i32 n)
RangeAddRangeMin<T> seg(const std::vector<T>& values)
```

長さ `n` の零初期化配列、または指定した配列から構築する。

**制約**

- `T` は符号付き整数型。
- $0 \le n$
- 初期値、更新後の値は単位元と異なる。
- 値と遅延作用の合成でオーバーフローしない。

**計算量**

- $O(n)$

## add(l, r, x)

区間 `[l, r)` に `x` を加算する。

**制約**

- $0 \le l \le r \le n$
- 加算後の各要素値と遅延作用の合成が `T` の範囲内に収まる。
- 更新後の値が単位元と異なる。

**計算量**

- $O(\log n)$

## add(p, x)

要素 `p` に `x` を加算する。

**制約**

- $0 \le p < n$
- 更新後の値が単位元と異なり、加算でオーバーフローしない。

**計算量**

- $O(\log n)$

## prod(l, r)

区間 `[l, r)` の最小値を返す。空区間では `numeric_limits<T>::max()` を返す。

**制約**

- $0 \le l \le r \le n$

**計算量**

- $O(\log n)$

## all_prod()

全要素の最小値を返す。

**計算量**

- $O(1)$

## get(p)

要素 `p` の値を返す。

**制約**

- $0 \le p < n$

**計算量**

- $O(\log n)$
