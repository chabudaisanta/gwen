---
title: 凸包 (Convex Polygon)
documentation_of: //include/gwen/geo/convex_polygon.hpp
---

## 概要
与えられた点集合から、凸多角形（凸包）を構築します。
内部で Andrew's Monotone Chain アルゴリズムを使用しています。

## convex_polygon

**制約**
- なし

**計算量**
- 点の数を $N$ として $O(N \log N)$

点の配列 `pts` から、凸多角形を構成する点の列を反時計回りで返します。
同一直線上にある中間の点は取り除かれます。
