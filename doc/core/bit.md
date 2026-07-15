---
title: ビット操作 (bit.hpp)
documentation_of: //include/gwen/core/bit.hpp
---

# ビット操作

整数の各ビットにアクセスしたり変更したりする補助関数を提供します。

## 関数一覧
- `T1 getBit(T1 bit, T2 i)`: `i` 番目のビットを取得
- `T1 setBit(T1 bit, T2 i)`: `i` 番目のビットを1にする
- `T1 clearBit(T1 bit, T2 i)`: `i` 番目のビットを0にする
- `T1 toggleBit(T1 bit, T2 i)`: `i` 番目のビットを反転する
