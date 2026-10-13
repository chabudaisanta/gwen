---
title: RangeAddRangeSum
documentation_of: //include/gwen/query/range_add_range_sum.hpp
---

# RangeAddRangeSum

区間加算と区間和をサポートする遅延評価セグメント木。

## インクルード

```cpp
#include "gwen/query/range_add_range_sum.hpp"
```

## コンストラクタ

```cpp
RangeAddRangeSum<T> seg(i32 n)
RangeAddRangeSum<T> seg(const std::vector<T>& values)
```

長さ `n` の零初期化配列、または指定した配列から構築する。

**制約**

- `T` は `semiring` を満たす。
- $0 \le n$
- 構築時を含む内部演算の全ての加算・乗算結果が `T` で表現できる。長さの集約、区間和、遅延作用の合成、作用と区間長の積も含む。

**計算量**

- $O(n)$

## add(l, r, x)

区間 `[l, r)` に `x` を加算する。

**制約**

- $0 \le l \le r \le n$
- 内部で行う全ての加算と乗算の結果が `T` で表現できる。加算後の要素値・区間和のほか、遅延作用の合成、区間長の加算、作用と区間長の積も含む。

**計算量**

- $O(\log n)$

## add(p, x)

要素 `p` に `x` を加算する。

**制約**

- $0 \le p < n$
- 要素値、区間和、遅延作用の合成など、内部で行う全ての加算・乗算結果が `T` で表現できる。

**計算量**

- $O(\log n)$

## prod(l, r)

区間 `[l, r)` の和を返す。空区間では `T(0)` を返す。

**制約**

- $0 \le l \le r \le n$

**計算量**

- $O(\log n)$

## all_prod()

全要素の和を返す。

**計算量**

- $O(1)$

## get(p)

要素 `p` の値を返す。

**制約**

- $0 \le p < n$

**計算量**

- $O(\log n)$
