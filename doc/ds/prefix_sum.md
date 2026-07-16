---
title: 累積和 (prefix_sum.hpp)
documentation_of: //include/gwen/ds/prefix_sum.hpp
---

# 累積和

1次元および 2次元の累積和を提供します。

## **prefix_sum**

```cpp
template <typename T>
std::vector<T> prefix_sum(const std::vector<T>& vec)
```

1次元累積和を返します。要素数は元の配列より1大きくなります。

**制約**

- なし

**計算量**

- $O(N)$

## **PrefixSum2d**

```cpp
template <typename T>
struct PrefixSum2d
```

2次元累積和を構築し、矩形領域の和を取得します。

### **コンストラクタ**

```cpp
explicit PrefixSum2d(const std::vector<std::vector<T>>& vec)
```

2次元配列から累積和を構築します。

**制約**

- なし

**計算量**

- $O(HW)$（$H$: 行数、$W$: 列数）

### **get**

```cpp
T get(i32 l, i32 r, i32 u, i32 d) const
```

半開区間 $[u, d) \times [l, r)$ の矩形領域の和を取得します。

**制約**

- $0 \le l \le r \le W$
- $0 \le u \le d \le H$

**計算量**

- $O(1)$
