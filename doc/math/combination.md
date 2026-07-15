---
title: 順列・組合せ (combination.hpp)
documentation_of: //include/gwen/math/combination.hpp
---

# 順列・組合せ (combination.hpp)

階乗、階乗の逆元、順列、組合せを定数時間で計算するクラスを提供します。
`DynamicModInt64` のように実行時に法が変化する状況でも、インスタンスごとに状態を持たせることで安全に利用できます。

## Combination

```cpp
template <gwen::modint T>
class Combination
```

**制約**

- `T` は `gwen::modint` コンセプトを満たす型（`DynamicModInt64`, `ModInt61` など）であること。

## コンストラクタ

```cpp
explicit Combination(i32 n = 0)
```

初期サイズ `n` で前計算テーブルを構築します。
※指定したサイズ以上の値を要求された場合、自動的にサイズを倍加して拡張する（`extend`）ため、初期値は厳密な最大値でなくても構いません。

**制約**

- `n >= 0`
- 初期化および自動拡張の際、$O(N)$ のメモリと時間を消費するため、想定される最大値が $10^8$ などを超える場合は MLE / TLE に注意してください。

**計算量**

- $O(n)$

## fact

```cpp
T comb.fact(i32 n)
```

$n!$ を返します。

**制約**

- $0 \le n < \text{T::mod()}$

**計算量**

- ならし $O(1)$ （※テーブルが不足している場合は内部で拡張処理 $O(n)$ が走ります）

## fact_inv

```cpp
T comb.fact_inv(i32 n)
```

$n!$ の逆元を返します。

**制約**

- $0 \le n < \text{T::mod()}$

**計算量**

- ならし $O(1)$

## comb

```cpp
T comb.comb(i32 n, i32 k)
```

二項係数 $\binom{n}{k}$ （$n$ 個から $k$ 個を選ぶ組合せの数）を返します。
$k < 0$ または $n < k$ の場合は $0$ を返します。

**制約**

- $0 \le n < \text{T::mod()}$

**計算量**

- ならし $O(1)$

## perm

```cpp
T comb.perm(i32 n, i32 k)
```

順列 ${}_n P_k$ （$n$ 個から $k$ 個を選んで並べる数）を返します。
$k < 0$ または $n < k$ の場合は $0$ を返します。

**制約**

- $0 \le n < \text{T::mod()}$

**計算量**

- ならし $O(1)$

## 使用例

```cpp
#include <iostream>
#include "gwen/math/combination.hpp"
#include "gwen/mod/modint.hpp"

using namespace gwen;

int main() {
    using mint = DynamicModInt64;
    mint::set_mod(998244353);
    
    // インスタンス化 (初期サイズを指定しなくても必要な時に自動で拡張されます)
    Combination<mint> comb(100);
    
    // 10C3 = 120
    std::cout << comb.comb(10, 3).val() << "\n";
    
    // 5P2 = 20
    std::cout << comb.perm(5, 2).val() << "\n";
    
    // 5! = 120
    std::cout << comb.fact(5).val() << "\n";
    
    return 0;
}
```
