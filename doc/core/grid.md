---
title: グリッド探索 (grid.hpp)
documentation_of: //include/gwen/core/grid.hpp
---

# グリッド探索

2次元グリッド探索用の方向配列や補助関数を提供します。

## 定数と関数
- `mv`, `dw`, `dh`: 8方向＋停止の座標増分配列
- `std::pair<int, int> mv_to(int hi, int wi, int dir)`: 座標 `(hi, wi)` から `dir` 方向へ移動した後の座標を返します。
