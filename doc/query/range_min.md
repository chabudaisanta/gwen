---
title: RangeMin
documentation_of: //include/gwen/query/range_min.hpp
---

# RangeMin

`SegmentTree<min_monoid<T>>` の型エイリアス。一点更新と区間最小値取得に使う。

## インクルード

```cpp
#include "gwen/query/range_min.hpp"
```

## RangeMin<T>

```cpp
template <typename T>
using RangeMin = SegmentTree<min_monoid<T>>;
```

`SegmentTree` と同じコンストラクタ・メソッドを持つ。

**制約**

- `min_monoid<T>` の演算と単位元が有効である。

**計算量**

- 構築 $O(n)$、一点更新・区間取得 $O(\log n)$、全体取得 $O(1)$。
