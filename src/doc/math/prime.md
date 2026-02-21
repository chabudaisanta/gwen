# Prime（素因数分解・素数判定）

素因数分解（LPF テーブル）と、64bit までの素数判定（Miller–Rabin）を提供します。

参照: [AC Library document_ja](https://atcoder.github.io/ac-library/master/document_ja/)

---

## factorize

```
vector<i32> factorize(i32 x);
```

正の整数 `x` を素因数分解し、素因数の列を**昇順**で返します。$x \leq 1$ のときは空の列を返します。

**制約**

- $0 \leq x \leq 10^8$

**計算量**

- テーブルが足りていれば $O(\log x)$。足りなければ拡張 $O(n)$。

---

## is_prime_small

```
bool is_prime_small(i32 n);
```

`n` が素数かどうかを返します。内部で LPF テーブルを使います。

**制約**

- $n \leq 10^8$

**計算量**

- テーブルが足りていれば $O(1)$。

---

## miller

```
bool miller(u64 n);
```

64bit 非負整数 `n` が素数かどうかを Miller–Rabin で判定します。$n \leq 1$ は素数でない、$n = 2$ は素数とし、それ以外は奇数として判定します。

- $n < 2^{23}$ のときは `is_prime_small` に委譲します。
- $n < 4759123141$ のときは 32bit 用の基底（2, 7, 61）で判定します。
- それ以上は 64bit 用の基底 7 個で判定します。

**計算量**

- $O(\log n)$（試行回数は定数）

---

## miller32 / miller64

```
bool miller32(u32 n);
bool miller64(u64 n);
```

それぞれ 32bit・64bit 専用の Miller–Rabin です。`miller` から自動で切り替わるため、通常は `miller` のみ使えば十分です。

**制約**

- `miller32`: $n < 4759123141$

---

## 使用例

```cpp
#include "gwen/math/prime.hpp"

using namespace gwen;

auto fac = factorize(12);  // {2, 2, 3}
bool b = is_prime_small(7);  // true
bool c = miller(1000000007ULL);  // true
```
