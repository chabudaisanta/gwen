# matrix（行列）

`std::vector` による密行列。加減乗算・累乗のほか、行列式・ランク・逆行列・転置をサポートします。

---

## テンプレート

```
template <typename T> struct matrix;
```

`det` / `rank` / `inverse` には `matrix_field<T>`（`0`, `1`, 四則演算, `==`）が必要です。`int`, `long long`, modint, `double` など。

`int` での `inverse` は、消去過程が整数のまま閉じる場合（例: 行列式 $\pm 1$）にのみ正しくなります。一般には `double` や modint を使ってください。

---

## コンストラクタ

```
matrix();
matrix(i32 N);           // N×N
matrix(i32 N, i32 M, T val = T{});
```

---

## 形状

```
i32 rows() const;
i32 cols() const;
bool is_square() const;
```

---

## 演算子

| 演算 | 説明 |
|------|------|
| `A + B`, `A - B` | 同形状の加減 |
| `A * B` | 積（`A.cols() == B.rows()`） |
| `A * k` | スカラー倍 |
| `A += B`, `A -= B`, `A *= B`, `A *= k` | 代入版（`+=`/`-=`/`k` はインプレース） |

---

## transpose

```
matrix transpose() const;
```

$A_{ij} \to A^{\mathsf T}_{ji}$。結果は $M \times N$。

---

## det

```
T det() const;
```

正方行列の行列式。ガウス消去 $O(N^3)$。特異なら `0`。

---

## rank

```
i32 rank() const;
```

ランク。ガウス消去 $O(NM^2)$ 程度。

---

## trace

```
T trace() const;
```

正方行列の対角和。

---

## inverse

```
matrix inverse() const;
```

正方行列の逆行列。特異なら **空行列**（`rows() == 0`）を返します。

---

## eye / pow

```
static matrix eye(i32 N);
static matrix pow(matrix A, U n);  // A は正方、二乗乗算
```

---

## 使用例

```cpp
#include "gwen/math/matrix.hpp"

using namespace gwen;

matrix<int> A(2);
A[0][0] = 1; A[0][1] = 2;
A[1][0] = 3; A[1][1] = 4;

A.det();                    // -2
A.inverse();                // 2×2 逆行列
(matrix<int>::eye(2) * A);  // 積
matrix<int>::pow(A, 10);    // 累乗
```
