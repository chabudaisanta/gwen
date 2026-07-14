---
title: abel
documentation_of: //include//gwen/alge/abel.hpp
---

# abel.hpp

モノイドの性質に加え、逆元を持つ「群 (Group)」および可換性を満たす「アーベル群 (Abelian Group)」のコンセプトと構造体を提供するヘッダーです。
Fenwick Tree など、逆元（引き算や打ち消し）を用いた区間クエリが必要なデータ構造で要求されます。

## インクルード

```cpp
#include "gwen/alge/abel.hpp"
```

## コンセプト
```cpp
template <typename T>
concept abel = monoid<T> && requires {
    { T::inv(std::declval<typename T::S>()) } -> std::convertible_to<typename T::S>;
};
```
モノイドの要件に加えて、逆元を計算する `inv(S)` 関数が要求されます。

## 定義済みアーベル群構造体

- `sum_abel<T>`: 加法アーベル群（`inv(x)` は `-x` を返します）
- `xor_abel<T>`: 排他的論理和アーベル群（`inv(x)` は `x` を返します）
