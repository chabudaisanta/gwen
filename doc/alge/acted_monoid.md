# 作用付きモノイド (Acted Monoid)

遅延評価セグメント木やTreapなどで、要素（モノイド）に対して作用させる操作とその合成を定義するコンセプトです。

## 定義

`gwen::monoid` の要件に加え、以下の型・関数が定義された構造体 `M` は `gwen::acted_monoid` コンセプトを満たします。

- `F`: 作用素の型
- `mapping`: 作用 $F \times S \to S$
- `composition`: 作用素の合成 $F \times F \to F$ (`composition(f, g)` は $f(g(x))$ となるように定義)
- `id`: 作用素の恒等写像

## 例

```cpp
#include "gwen/alge/acted_monoid.hpp"
#include "gwen/alge/range_affine_range_sum_monoid.hpp"
#include "gwen/mod/modint.hpp"

using M = gwen::range_affine_range_sum_monoid<gwen::DynamicModInt64>;
static_assert(gwen::acted_monoid<M>);
```
