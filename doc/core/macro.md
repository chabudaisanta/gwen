---
title: マクロ (macro.hpp)
documentation_of: //include/gwen/core/macro.hpp
---

# マクロ

ループ等の競技プログラミング向け汎用マクロを提供します。

## マクロ一覧
- `rep(i, l, r)`: `l` から `r-1` までのループ
- `rp(i, n)`: `0` から `n-1` までのループ
- `rrep(i, l, r)`: `r-1` から `l` までの逆ループ
- `all(a)`: `(a).begin(), (a).end()`
- `rall(a)`: `(a).rbegin(), (a).rend()`
- `BAR`, `S_BAR`: デバッグ用の区切り線（`LOCAL` 定義時のみ出力）
