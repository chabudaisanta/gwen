---
title: 区間アフィン変換・区間和モノイド
documentation_of: //include/gwen/alge/range_affine_range_sum_monoid.hpp
---

# range_affine_range_sum_monoid

遅延評価セグメント木（Lazy Segment Tree）などで、「区間に対するアフィン変換の作用」と「区間和の取得」を同時に行うための作用付きモノイド（代数構造）です。

## コンストラクタ / テンプレート引数

```cpp
template <ring T>
struct range_affine_range_sum_monoid;
```

- `T`: 値の型。`gwen::ring<T>` コンセプト（加法および乗法が定義されていること）を満たす必要があります。通常は `DynamicModInt64` や `StaticModInt` などが利用されます。

## モノイド要素 (`S`)

区間和と区間の長さを保持します。

```cpp
struct S {
    T val;  // 区間の和
    T len;  // 区間の長さ
};
```

- **二項演算** `op(S a, S b)`: 二つの区間を結合します。`{a.val + b.val, a.len + b.len}` を返します。
- **単位元** `e()`: 空の区間を表します。`{T(0), T(0)}` を返します。
- **初期化** `unit(T x)`: 長さ1の区間（初期配列の要素）を生成します。`{x, T(1)}` を返します。

## 作用素 (`F`)

区間に対するアフィン変換 $f(x) = ax + b$ を表します。

```cpp
struct F {
    T a;  // 係数
    T b;  // 定数項
};
```

- **作用** `mapping(F f, S x)`: 区間全体にアフィン変換を適用します。各要素に $f$ が作用すると、区間和は $a \times (\text{和}) + b \times (\text{長さ})$ に変化します。したがって `{f.a * x.val + f.b * x.len, x.len}` を返します。
- **合成** `composition(F f, F g)`: 作用素を合成します（先に $g$、次に $f$ を適用）。$f(g(x)) = (f.a \times g.a) x + (f.a \times g.b + f.b)$ となるため `{f.a * g.a, f.a * g.b + f.b}` を返します。
- **恒等写像** `id()`: 何も変化させない作用素（$f(x) = x$）です。`{T(1), T(0)}` を返します。
