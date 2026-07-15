---
title: Modulo Arithmetic (モジュラ演算基盤)
documentation_of: //include/gwen/mod/mod.hpp
---

# モジュラ演算基盤 (mod)

モジュラ演算に関する基本的な関数群を提供します。

## pow_mod

```cpp
i64 pow_mod(i64 x, i64 n, i64 m = 998244353)
```

$x^n \pmod m$ を計算します。

**制約**
- $n \ge 0$
- $m > 0$

**計算量**
- $O(\log n)$

## inv_mod

```cpp
i64 inv_mod(i64 a, i64 m = 998244353)
```

拡張ユークリッドの互除法を用いて、$a$ の $m$ を法とする逆元を計算します。

**制約**
- $a$ と $m$ は互いに素であること。
- $m > 0$

**計算量**
- $O(\log (\min(a, m)))$

## inv_mod_64

```cpp
u64 inv_mod_64(u64 a, u64 m)
```

64bit整数 (u64) に対応した逆元計算関数です。内部では `i128` 型を利用してオーバーフローを防いでいます。

**制約**
- $a$ と $m$ は互いに素であること。
- $m > 0$

**計算量**
- $O(\log (\min(a, m)))$
