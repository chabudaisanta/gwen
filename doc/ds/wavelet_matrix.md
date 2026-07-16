---
title: WaveletMatrix
documentation_of: //include/gwen/ds/wavelet_matrix.hpp
---

## 概要

`WaveletMatrix` は非負整数列に対して、区間内の特定の値の出現回数（range frequency）や、区間内の k 番目に小さい値（range quantile）を効率的に求めるデータ構造です。
依存構造として `BitVector` (簡潔ビットベクトル) を使用しています。

各操作は要素の最大値のビット数 $O(\log(\max A))$ に比例する時間で行われます。

## コンストラクタ

```cpp
WaveletMatrix<T> wm(const std::vector<T>& vec)
```

初期配列 `vec` を指定してウェーブレット行列を構築します。配列要素は非負整数である必要があります。

**計算量**
- $O(n \log(\max A))$

## access

```cpp
T wm.access(i32 i)
```

元の配列の $i$ 番目の要素を返します。

**計算量**
- $O(\log(\max A))$

## kth_smallest

```cpp
T wm.kth_smallest(i32 l, i32 r, i32 k)
```

区間 $[l, r)$ 内の要素の中で $k$ 番目（0-indexed）に小さい値を返します。

**制約**
- $0 \le k < r - l$

**計算量**
- $O(\log(\max A))$

## kth_largest

```cpp
T wm.kth_largest(i32 l, i32 r, i32 k)
```

区間 $[l, r)$ 内の要素の中で $k$ 番目（0-indexed）に大きい値を返します。

**制約**
- $0 \le k < r - l$

**計算量**
- $O(\log(\max A))$

## range_freq

```cpp
i32 wm.range_freq(i32 l, i32 r, T upper)
i32 wm.range_freq(i32 l, i32 r, T lower, T upper)
```

区間 $[l, r)$ のうち、値が `upper` 未満（または `[lower, upper)` に含まれる）要素の個数を返します。

**計算量**
- $O(\log(\max A))$

## prev_value

```cpp
std::optional<T> wm.prev_value(i32 l, i32 r, T upper)
```

区間 $[l, r)$ で `upper` 未満の要素のうち最大のものを返します。存在しない場合は `std::nullopt` を返します。

**計算量**
- $O(\log(\max A))$

## next_value

```cpp
std::optional<T> wm.next_value(i32 l, i32 r, T lower)
```

区間 $[l, r)$ で `lower` 以上の要素のうち最小のものを返します。存在しない場合は `std::nullopt` を返します。

**計算量**
- $O(\log(\max A))$

## size

```cpp
i32 wm.size()
```

配列の長さ $n$ を返します。

**計算量**
- $O(1)$
