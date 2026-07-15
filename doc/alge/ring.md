---
title: 環 (Ring)
documentation_of: //include/gwen/alge/ring.hpp
---

# 環 (Ring)

代数構造としての「環（Ring）」の要件を定義する C++23 の `concept` です。

## 概要

和・差・積の四則演算とその代入演算子（`+`, `-`, `*`, `+=`, `-=`, `*=`）が定義されており、期待通りの型を返すことを要求します。

## concept

```cpp
template <typename T>
concept ring = requires(T a, T b) {
    { a + b } -> std::same_as<T>;
    { a - b } -> std::same_as<T>;
    { a * b } -> std::same_as<T>;
    { a += b } -> std::same_as<T&>;
    { a -= b } -> std::same_as<T&>;
    { a *= b } -> std::same_as<T&>;
};
```
