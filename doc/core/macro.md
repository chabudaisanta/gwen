---
title: マクロ (macro.hpp)
documentation_of: //include/gwen/core/macro.hpp
---

# マクロ

ループ等の競技プログラミング向け汎用マクロを提供します。

## **rep**

```cpp
#define rep(i, l, r)
```

`l` から `r-1` までのループ。`for (i = l; i < r; ++i)` と等価です。

## **rp**

```cpp
#define rp(i, n)
```

`0` から `n-1` までのループ。`rep(i, 0, n)` と等価です。

## **rrep**

```cpp
#define rrep(i, l, r)
```

`r-1` から `l` までの逆ループ。

## **all / rall**

```cpp
#define all(a)
#define rall(a)
```

`all(a)` は `(a).begin(), (a).end()`、`rall(a)` は `(a).rbegin(), (a).rend()` に展開されます。

## **BAR / S_BAR**

```cpp
#define BAR
#define S_BAR
```

`LOCAL` 定義時のみ標準エラー出力へ区切り線を出力します。
