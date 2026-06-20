# basic_monoid

セグメント木 (`gwen::segtree` / `atcoder::segtree`) 等に乗せるための、基本的な代数構造（モノイド）のテンプレート実装群を提供します。

## ヘッダファイル

```cpp
#include "gwen/algebra/basic_monoid.hpp"
```

## 提供されているモノイド構造体

各構造体は要素の型 `T` をテンプレート引数として取り、要素型 `S`、二項演算 `op`、単位元 `e` を持ちます。

### `sum_monoid<T>`
- **演算**: 加算 ($a + b$)
- **単位元**: 0

### `xor_monoid<T>`
- **演算**: XOR ($a \oplus b$)
- **単位元**: 0

### `min_monoid<T>`
- **演算**: 最小値 ($\min(a, b)$)
- **単位元**: `std::numeric_limits<T>::max()`

### `max_monoid<T>`
- **演算**: 最大値 ($\max(a, b)$)
- **単位元**: `std::numeric_limits<T>::min()`

### `minmax_monoid<T>`
- **要素の型**: `struct { i32 min, max; }` (※実装上型固定の箇所に注意)
- **演算**: 区間の最小値と最大値を同時に管理します。
- **単位元**: `min = max()`, `max = min()`

### `gcd_monoid<T>`
- **演算**: 最大公約数 ($\gcd(a, b)$)
- **単位元**: 0

### `affine_monoid<T>`
- **要素の型**: `struct { T a, b; }` （$ax + b$ を表す）
- **演算**: 1次関数の合成 $f(g(x))$ 
- **単位元**: $1x + 0$ (a = 1, b = 0)

### `no_op_monoid<T>`
- 何も演算を行わないダミーのモノイド・作用素です。遅延セグ木等で、片方の演算が実質的に不要な場合などにプレースホルダーとして利用できます。

## 使用例

```cpp
#include "gwen/container/segtree.hpp"
#include "gwen/algebra/basic_monoid.hpp"

// 区間の最大値を管理するセグメント木
gwen::segtree<gwen::max_monoid<long long>> seg(N);
```
