# Combination（組み合わせ・階乗）

階乗 $n!$、階乗の逆元、二項係数 $\binom{n}{k}$ を、静的テーブルで前計算しながら $O(1)$ で返す関数群です。型 `T` は乗法と逆元が定義されている必要があります（例: `gwen::modint`）。

---

## fact

```
template <typename T>
T fact(i32 n);
```

$n!$ を返します。内部で `n` がテーブルサイズを超えるまでテーブルを拡張します。

**制約**

- $n \geq 0$

**計算量**

- 初回または拡張時: $O(n)$。以降は $O(1)$。

---

## fact_inv

```
template <typename T>
T fact_inv(i32 n);
```

$(n!)^{-1}$ を返します。内部で `n` がテーブルサイズを超えるまでテーブルを拡張します。

**制約**

- $n \geq 0$

**計算量**

- 初回または拡張時: $O(n)$。以降は $O(1)$。

---

## comb

```
template <typename T>
T comb(i32 n, i32 k);
```

二項係数 $\binom{n}{k}$ を返します。$k < 0$ または $k > n$ のときは `T(0)` を返します。

**計算量**

- テーブルが足りていれば $O(1)$。足りなければ拡張 $O(n)$。

---

## 使用例

```cpp
#include "gwen/math/combination.hpp"
#include "gwen/mod/modint.hpp"

using namespace gwen;

using mint = modint998244353;
mint a = fact<mint>(5);      // 120
mint b = fact_inv<mint>(3);  // 1/6
mint c = comb<mint>(5, 2);   // 10
```
