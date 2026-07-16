---
title: Edge
documentation_of: //include/gwen/graph/edge.hpp
---

# Edge

グラフのエッジを表す構造体と、エッジが満たすべき要件を定義する `concept` を提供します。

## 構造体 (Edge)

### コンストラクタ

```cpp
Edge()
Edge(i32 u, i32 v)
Edge(i32 u, i32 v, T w)
```
- `u`: 始点
- `v`: 終点
- `w`: 重み (デフォルトは1)

## コンセプト (edge)

```cpp
template <typename T>
concept edge;
```
型 `T` が以下の要件を満たすことを要求します。
- `t.u` が `i32` に変換可能であること。
- `t.v` が `i32` に変換可能であること。
- `T::weight_type` が定義されていること。
