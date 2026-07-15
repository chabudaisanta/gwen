---
title: ユーティリティ (utils.hpp)
documentation_of: //include/gwen/core/utils.hpp
---

# ユーティリティ

最大値・最小値の更新や、範囲チェックなどの便利関数を提供します。

## 関数一覧
- `bool chmax(T1& a, T2 b)`: `a < b` なら `a` を `b` で更新し `true` を返す
- `bool chmin(T1& a, T2 b)`: `a > b` なら `a` を `b` で更新し `true` を返す
- `bool isIn(T x, T l, T r)`: `l <= x < r` か判定
- `bool isOut(T x, T l, T r)`: `x < l` または `r <= x` か判定
- `bool isOverlap(T l0, T r0, T l1, T r1)`: 区間 `[l0, r0)` と `[l1, r1)` が重なっているか判定
