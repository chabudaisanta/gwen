---
title: 半環 (Semiring)
documentation_of: //include/gwen/alge/semiring.hpp
---

# 半環 (Semiring)

加法、乗法、0、1を持つ型の構文要件を表す C++23 Concept です。

```cpp
#include "gwen/alge/semiring.hpp"
```

## semiring

```cpp
template <typename T>
concept semiring;
```

型 `T` が次の式を持つことを要求します。

- `a + b` の結果が `T`
- `a * b` の結果が `T`
- `T(0)` と `T(1)` が構築可能

**制約**

- 加法と乗法はそれぞれ結合律を満たす。
- 加法は可換律を満たす。
- `T(0)` は加法単位元、`T(1)` は乗法単位元である。
- 乗法は加法に対して左右の分配律を満たす。
- `T(0)` は乗法の吸収元である。
- これらの代数法則は Concept 自体では検査されないため、利用者が保証する。

**計算量**

- コンパイル時の構文検査のみであり、実行時コストはありません。

## 使用例

```cpp
static_assert(gwen::semiring<int>);
```
