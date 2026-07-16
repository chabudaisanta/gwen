---
title: ランレングス圧縮 (runlength.hpp)
documentation_of: //include/gwen/algo/runlength.hpp
---

# ランレングス圧縮

イテレータの範囲に対してランレングス圧縮（連長圧縮）を行う関数を提供します。

## **runlength**

```cpp
template <typename Iterator>
auto runlength(Iterator begin, Iterator end)
```

値と連続する回数のペア `std::pair<T, i32>` のベクターを返します。

**制約**

- なし

**計算量**

- $O(N)$（$N$ は範囲の長さ）
