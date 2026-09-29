---
title: 直線 (Line)
documentation_of: //include/gwen/geo/line.hpp
---

異なる2点を通る直線を扱います。名前空間は `gwen::geo`、座標型 `T` は `std::floating_point` を満たす型です。通常は `f64` または `f80` を使用します。

## **Line(a, b)**

```cpp
Line<T> line(Point<T> a, Point<T> b);
```

公開メンバ `a`、`b` に直線上の2点を保持します。点の順序を逆にしても同じ直線です。

**制約**
- 両点の座標は有限、`a != b`。
- 構築後にメンバを変更した場合もこの制約を満たす必要があります。構築時と演算時に `assert` で検査します。

**計算量**
- $O(1)$

## **projection(p)**

```cpp
Point<T> line.projection(const Point<T>& p) const;
```

点 `p` から直線へ下ろした垂線の足を返します。定義点 `a`、`b` の間に限定しません。

**制約**
- 座標は有限、`a != b`。
- 座標差・内積等の中間演算と正の方向ベクトルの長さが `T` で表現できる必要があります。

**計算量**
- $O(1)$

## **distance(p)**

```cpp
T line.distance(const Point<T>& p) const;
```

点 `p` と直線の最短距離を返します。

**制約**
- `projection(p)` と同じです。

**計算量**
- $O(1)$

浮動小数点演算の丸め誤差を含みます。幾何演算の結果を比較する際は、問題の座標スケールに合った許容誤差を使用してください。
