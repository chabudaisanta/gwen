# Polynomial Difference Array (いもす法・多項式拡張)

いもす法を多項式の加算に拡張したアルゴリズムです。
区間に対して任意の多項式（最大次数 $K$）を加算するクエリを $O(K)$ で処理し、最後にすべてのクエリ結果を足し合わせた配列を $O(NK)$ で構築します。

---

## コンストラクタ

```cpp
PolynomialDifferenceArray<Ring> imos(i32 n, i32 k);
```

- **入力**: 
  - `n`: 配列のサイズ。クエリの対象となる区間は $[0, n)$ 内であることを想定します。
  - `k`: 追加する多項式の最大次数。
- `Ring`: 要素の型 `S` や、加法に関する `op`, `inv`, 乗法に関する `mul`, `e_mul` などを定義した構造体（`basic_ring.hpp` の `sum_ring` などを用います）。

**計算量**

- $O(nk)$ （初期化処理）

---

## add_poly

```cpp
template <typename Container>
void imos.add_poly(i32 l, i32 r, const Container& poly);
```

区間 $[l, r)$ に対して、多項式 $f(i) = \sum_{d=0}^{|poly|-1} poly[d] \times i^d$ を加算します。
- **引数**:
  - `l, r`: 加算対象の区間 $[l, r)$。
  - `poly`: 多項式の係数配列（`std::vector<S>` や `std::array<S, N>` など、範囲for文でイテレート可能なもの）。`poly[d]` が $i^d$ の係数となります。

> **注意**: ここでの $i$ は配列の絶対インデックス（$0$ から $n-1$）を指します。区間の先頭を基準とした相対インデックス（$i-l$）で多項式を加算したい場合は、事前に $f(x+l)$ を展開して係数を計算したうえで本クラスに渡す必要があります。

**計算量**

- $O(|poly|) \le O(k)$

---

## add_term

```cpp
void imos.add_term(i32 l, i32 r, S c, i32 d);
```

区間 $[l, r)$ に対して、単一の項 $f(i) = c \times i^d$ を加算します。
- **引数**:
  - `l, r`: 加算対象の区間 $[l, r)$。
  - `c`: 項の係数。
  - `d`: 項の次数 ($d \le k$)。

**計算量**

- $O(1)$

---

## build

```cpp
std::vector<S> imos.build();
```

追加されたすべてのクエリを処理し、長さ $n$ の最終的な配列を構築して返します。
内部では各次数ごとに1次元のいもす法（累積和）を行い、各座標で `Ring::mul` を用いて $i^d$ を掛けて足し合わせる処理が行われます。

※型 `S` は `S((i64)i)` のような `i64` 型からの構築・型変換をサポートしている必要があります（`modint` や `i64` などを想定）。

**計算量**

- $O(nk)$

---

## 使用例

```cpp
#include "gwen/algebra/basic_ring.hpp"
#include "gwen/query/polynomial_difference_array.hpp"
#include "gwen/types.hpp"
#include <vector>

using namespace gwen;

// i64 型を要素とする環 (加算と乗算をサポート)
using Ring = sum_ring<i64>;

int main() {
    i32 n = 10;
    i32 k = 2;
    PolynomialDifferenceArray<Ring> imos(n, k);

    // [2, 6) に 1 + 2*i を加算
    std::vector<i64> poly = {1, 2};
    imos.add_poly(2, 6, poly);

    // [4, 8) に 3*i^2 を加算
    imos.add_term(4, 8, 3, 2);

    // 結果の構築
    std::vector<i64> res = imos.build();
    
    return 0;
}
```
