---
title: 環 (Ring)
documentation_of: //include/gwen/alge/ring.hpp
---

# 環 (Ring)

`semiring<T>` に減算を加えた型の構文要件を表す C++23 Concept です。

```cpp
#include "gwen/alge/ring.hpp"
```

## ring

```cpp
template <typename T>
concept ring = semiring<T> && requires(T a, T b) {
    { a - b } -> std::same_as<T>;
    { a += b } -> std::same_as<T&>;
    { a -= b } -> std::same_as<T&>;
    { a *= b } -> std::same_as<T&>;
};
```

`semiring<T>` の要件に加えて、`-`、`+=`、`-=`、`*=` が期待する型を返すことを要求します。

**制約**

- `T` は `semiring<T>` を満たす。
- 加法について逆元が存在する。
- 環としての代数法則は Concept 自体では検査されないため、利用者が保証する。

**計算量**

- コンパイル時の構文検査のみであり、実行時コストはありません。
