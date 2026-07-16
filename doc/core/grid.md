---
title: グリッド探索 (grid.hpp)
documentation_of: //include/gwen/core/grid.hpp
---

# グリッド探索

2次元グリッド探索用の方向配列や補助関数を提供します。

## 定数

- **`mv`**: 8方向＋停止の (y, x) 座標増分配列
- **`dw`**: 8方向＋停止のX方向増分配列
- **`dh`**: 8方向＋停止のY方向増分配列

## **mv_to**

```cpp
inline std::pair<i32, i32> mv_to(i32 hi, i32 wi, i32 dir)
```

座標 `(hi, wi)` から `dir` 方向へ移動した後の座標を返します。

**制約**

- $0 \le $ `dir` $ < 9$

**計算量**

- $O(1)$
