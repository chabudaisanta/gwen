---
title: TransformableMatrix
documentation_of: //include/gwen/ds/transformable_matrix.hpp
---

# TransformableMatrix

行・列の交換、転置、回転などの操作を $O(1)$ で行える 2 次元配列のビューアーです。
内部的には 1 次元配列でデータを保持し、アクセス時に現在の軸情報に基づき元のインデックスを計算することで $O(1)$ 操作を実現しています。

## コンストラクタ

```cpp
TransformableMatrix<T>(const std::vector<std::vector<T>>& a)
```

- **制約**: `a` はすべての行が同じ長さを持つ 2 次元配列であること。
- **計算量**: $O(HW)$

## メソッド

### `h`

```cpp
i32 h() const
```

現在の見た目における行数を取得します。

- **計算量**: $O(1)$

### `w`

```cpp
i32 w() const
```

現在の見た目における列数を取得します。

- **計算量**: $O(1)$

### `operator()`

```cpp
T& operator()(i32 i, i32 j)
const T& operator()(i32 i, i32 j) const
```

現在の見た目における $(i, j)$ の要素への参照を取得します。

- **制約**: $0 \le i < h(), 0 \le j < w()$
- **計算量**: $O(1)$

### `swap_rows`

```cpp
void swap_rows(i32 i, i32 j)
```

現在の $i$ 行目と $j$ 行目を交換します。

- **制約**: $0 \le i < h(), 0 \le j < h()$
- **計算量**: $O(1)$

### `swap_columns`

```cpp
void swap_columns(i32 i, i32 j)
```

現在の $i$ 列目と $j$ 列目を交換します。

- **制約**: $0 \le i < w(), 0 \le j < w()$
- **計算量**: $O(1)$

### `transpose`

```cpp
void transpose()
```

行列を転置します。

- **計算量**: $O(1)$

### `rotate`

```cpp
void rotate(i32 num = 1)
```

行列を時計回りに 90 度ずつ `num` 回回転します。負の値を指定した場合は反時計回りの回転になります。

- **計算量**: $O(1)$

### `to_vec`

```cpp
std::vector<std::vector<T>> to_vec() const
```

現在の見た目における 2 次元配列を構築して返します。

- **計算量**: $O(HW)$
