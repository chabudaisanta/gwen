---
title: ユーティリティ (utils.hpp)
documentation_of: //include/gwen/core/utils.hpp
---

# ユーティリティ

最大値・最小値の更新や、範囲チェックなどの便利関数を提供します。

## **chmax**

```cpp
template <typename T1, typename T2>
bool chmax(T1& a, T2 b)
```

`a < b` なら `a` を `b` で更新し `true` を返します。

**制約**

- `T1` と `T2` が比較可能な型であること

**計算量**

- $O(1)$

## **chmin**

```cpp
template <typename T1, typename T2>
bool chmin(T1& a, T2 b)
```

`a > b` なら `a` を `b` で更新し `true` を返します。

**制約**

- `T1` と `T2` が比較可能な型であること

**計算量**

- $O(1)$

## **isIn**

```cpp
template <typename T>
bool isIn(T x, T l, T r)
```

`l <= x < r` か判定します。

**制約**

- なし

**計算量**

- $O(1)$

## **isOut**

```cpp
template <typename T>
bool isOut(T x, T l, T r)
```

`x < l` または `r <= x` か判定します。

**制約**

- なし

**計算量**

- $O(1)$

## **isOverlap**

```cpp
template <typename T>
bool isOverlap(T l0, T r0, T l1, T r1)
```

区間 `[l0, r0)` と `[l1, r1)` が重なっているか判定します。

**制約**

- なし

**計算量**

- $O(1)$
