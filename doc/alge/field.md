---
title: 体 (Field)
documentation_of: //include/gwen/alge/field.hpp
---

# 体 (Field)

`ring<T>` に除算を加えた可換体の構文要件を表す C++23 Concept です。

```cpp
#include "gwen/alge/field.hpp"
```

## field

```cpp
template <typename T>
concept field = ring<T> && std::equality_comparable<T> && requires(T a, T b) {
    { -a } -> std::same_as<T>;
    { a / b } -> std::same_as<T>;
    { a /= b } -> std::same_as<T&>;
};
```

`ring<T>` の要件に加えて、元同士の等値比較、単項マイナス `-`、除算 `/`、除算代入 `/=` が期待する型を返すことを要求します。

**制約**

- `T` は `ring<T>` を満たす。
- `T` は `std::equality_comparable<T>` を満たす。
- 0 以外のすべての元に乗法逆元が存在する。
- 体としての代数法則（乗法の可換性、逆元の存在等）は Concept 自体では検査されないため、利用者が保証する。

**計算量**

- コンパイル時の構文検査のみであり、実行時コストはありません。
