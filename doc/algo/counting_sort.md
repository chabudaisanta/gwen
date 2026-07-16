---
title: Counting Sort (カウンティングソート)
documentation_of: //include/gwen/algo/counting_sort.hpp
---

## 概要

配列内の値の出現回数を数え上げることで、要素の比較を行わずに安定ソートを実現するアルゴリズムです。
本関数は要素そのものを並べ替えるのではなく、ソート後の要素の元のインデックス配列（順列）を返します。

## 関数

### counting_sort

```cpp
template <typename T>
std::vector<i32> counting_sort(const std::vector<T>& a, i32 m)
```

値域が $[0, m]$ である整数列 `a` に対して、安定ソートされたインデックスの配列を返します。

**制約**

- `a` の各要素 $A_i$ は $0 \le A_i \le m$ を満たすこと。
- $0 \le N \le 10^7$ ($N$ は `a` の要素数)
- $0 \le m \le 10^7$

**計算量**

- $O(N + m)$
