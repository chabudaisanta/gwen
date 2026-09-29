---
title: 線分 (Segment)
documentation_of: //include/gwen/geo/segment.hpp
---

両端点を含む線分を扱います。名前空間は `gwen::geo`、座標型 `T` は `std::floating_point` を満たす型です。通常は `f64` または `f80` を使用します。

## **Segment(a, b)**

```cpp
Segment<T> segment(Point<T> a, Point<T> b);
```

公開メンバ `a`、`b` に端点を保持します。`a == b` の長さ0の線分は1点として扱います。

**制約**
- 座標は有限です。構築後にメンバを変更した場合も同じ制約が適用されます。

**計算量**
- $O(1)$

## **projection(p)**

```cpp
Point<T> segment.projection(const Point<T>& p) const;
```

線分上で点 `p` に最も近い点を返します。直線への射影が線分の外なら、近い方の端点を返します。長さ0なら `a` を返します。

**制約**
- 全座標は有限です。
- 座標差・内積等の中間演算と非退化時の正の長さが `T` で表現できる必要があります。

**計算量**
- $O(1)$

## **distance(p)**

```cpp
T segment.distance(const Point<T>& p) const;
```

点 `p` と線分の最短距離を返します。

**制約**
- `projection(p)` と同じです。

**計算量**
- $O(1)$

## **intersects(other, eps)**

```cpp
bool segment.intersects(const Segment<T>& other, T eps = T(1e-10)) const;
```

交差する場合、または線分間の最短距離が `eps` 以下の場合に `true` を返します。端点の接触、共線で重なる線分、長さ0の線分にも対応します。

外積の符号で内部の交差を検出し、外積が0の端点は座標範囲で接触・共線重複を検出します。それ以外は両線分の4端点から相手線分までの距離で判定します。`eps` は座標と同じ単位の絶対許容誤差です。

**制約**
- 全座標と `eps` は有限、`eps >= 0`。
- 中間演算と非退化時の正の長さが `T` で表現できる必要があります。
- `eps = 0` でも浮動小数点の丸め誤差は残ります。整数幾何の厳密な判定を保証する API ではありません。
- `f32` や大きい座標では、入力スケールに合った `eps` を明示してください。

**計算量**
- $O(1)$
