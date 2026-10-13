---
title: 区間加算・区間最大値モノイド
documentation_of: //include/gwen/alge/range_add_range_max_monoid.hpp
---

# 区間加算・区間最大値モノイド

区間加算と区間最大値を遅延評価セグメント木で扱う作用付きモノイドを提供する。

## インクルード

```cpp
#include "gwen/alge/range_add_range_max_monoid.hpp"
```

## range_add_range_max_monoid<T>

状態 `S` と作用 `F` は符号付き整数型 `T` とする。空区間の単位元は `numeric_limits<T>::lowest()` で表す。

**制約**

- `T` は `std::signed_integral` を満たす。
- 初期値と更新後の全要素値は単位元と異なる。
- 要素値と遅延作用の合成で発生する全ての加算は `T` の範囲内に収まる。
- 区間加算は順序を保つ。

番兵値を実データまたは更新後の値として使うと、空区間と区別できない。符号付き整数のオーバーフローは未定義動作となるため、制約を利用者が保証する。

**計算量**

- 各演算は $O(1)$。

### op(a, b)

2つの最大値を結合する。単位元は他方の値を返す。

### e()

空区間を表す単位元を返す。

### mapping(f, x)

`x` が単位元ならそのまま返し、それ以外は `x + f` を返す。

### composition(f, g)

先に `g`、後に `f` を適用する作用を `f + g` で返す。

### id()

加算しない作用 `0` を返す。
