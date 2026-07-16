---
title: インデックスソート (idxsort.hpp)
documentation_of: //include/gwen/algo/idxsort.hpp
---

# インデックスソート

配列のインデックスを元の値に基づいてソートする関数を提供します。

## **idxsort**

```cpp
template <typename T>
std::vector<i32> idxsort(const std::vector<T>& vec, bool rev = false)
```

`vec` の要素を元に、インデックス `0, 1, ..., n-1` をソートした結果を返します。`rev` が `true` の場合は降順にソートします。

**制約**

- なし

**計算量**

- $O(N \log N)$（$N$ は `vec` のサイズ）
