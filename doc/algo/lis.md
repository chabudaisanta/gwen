---
title: 最長増加部分列 (Longest Increasing Subsequence)
documentation_of: //include/gwen/algo/lis.hpp
---

## 概要

与えられた配列の最長増加部分列 (LIS) または最長非減少部分列を $O(n \log n)$ で求め、具体的な部分列の1つを構築して返します。

## 関数

### lis_strict

```cpp
template <std::totally_ordered T>
std::vector<T> lis_strict(const std::vector<T>& vec)
```

配列 `vec` の「真に単調増加」する最長の部分列を1つ返します。

**制約**

- `T` は全順序 (`std::totally_ordered`) を満たすこと。

**計算量**

- $O(n \log n)$ （$n$ は `vec` のサイズ）

### lis

```cpp
template <std::totally_ordered T>
std::vector<T> lis(const std::vector<T>& vec)
```

配列 `vec` の「広義単調増加（非減少）」な最長の部分列を1つ返します。

**制約**

- `T` は全順序 (`std::totally_ordered`) を満たすこと。

**計算量**

- $O(n \log n)$ （$n$ は `vec` のサイズ）
