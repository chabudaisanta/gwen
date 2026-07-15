---
title: 累積和 (prefix_sum.hpp)
documentation_of: //include/gwen/ds/prefix_sum.hpp
---

# 累積和

1次元および2次元の累積和を提供します。

## 関数・構造体
- `std::vector<T> prefix_sum(const std::vector<T>& vec)`
  - 1次元累積和を返します。要素数は元の配列より1大きくなります。
- `struct prefix_sum_2d<T>`
  - 2次元累積和を構築し、`get(l, r, u, d)` で半開区間 `[u, d) x [l, r)` の和を $O(1)$ で取得します。
