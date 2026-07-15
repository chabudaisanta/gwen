---
title: ランレングス圧縮 (runlength.hpp)
documentation_of: //include/gwen/algo/runlength.hpp
---

# ランレングス圧縮

イテレータの範囲に対してランレングス圧縮（連長圧縮）を行う関数を提供します。

## 関数一覧
- `auto runlength(Iterator begin, Iterator end)`
  - 値と連続する回数のペア `std::pair<T, i32>` のベクターを返します。
