---
title: Edge
documentation_of: //include/gwen/graph/edge.hpp
---

# Edge

グラフのエッジを表す構造体と、エッジが満たすべき要件を定義する `concept` を提供します。

## **コンストラクタ**

```cpp
Edge()
Edge(i32 u, i32 v)
Edge(i32 u, i32 v, T w)
```

グラフのエッジを構築します。

**制約**

- `u`: 始点の頂点番号
- `v`: 終点の頂点番号
- `w`: 重み（デフォルトは 1）

**計算量**

- $O(1)$

## **edge コンセプト**

```cpp
template <typename T>
concept edge;
```

型 `T` がエッジとして満たすべき要件を定義します。

**制約**

- `t.u` が `i32` に変換可能であること
- `t.v` が `i32` に変換可能であること
- `T::weight_type` が定義されていること
