---
title: Tree Diameter
documentation_of: //include/gwen/graph/tree_diameter.hpp
---

# Tree Diameter

非負整数重み付き木の直径、直径パス、および中心を求めます。中心が頂点に一致するか辺の途中にあるかも、`centers` で確認できます。

## **tree_diameter**

```cpp
template <edge Edge>
    requires(std::integral<typename Edge::weight_type> && !std::same_as<typename Edge::weight_type, bool>)
auto tree_diameter(const GraphBase<Edge, false>& G)
```

直径の重み付き長さ、両端、パス、中心を `TreeDiameterResult<Weight>` として返します。
`path` は `endpoint_u` から `endpoint_v` への順です。
`centers` は選択された直径パス上の幾何的な中心を表します。中心が頂点なら1頂点、正重み辺の途中ならその辺の両端2頂点です。0重み辺上で中心と同距離になる複数頂点がある場合は代表となる頂点を1つ返し、全ての中心を列挙するものではありません。
同率の最遠頂点が複数ある場合、直径の端点とパスはそのいずれかを返します。

**制約**

- `Edge` は `gwen::edge` を満たし、`Edge::weight_type` は `bool` 以外の整数型であること。
- `G` は頂点数 $N \ge 1$ の無向・連結・非巡回グラフであること。
- 全ての辺重みは非負であること。
- 木の任意のパスにおける重みの総和が `Edge::weight_type` に収まること。

**計算量**

- $O(N)$ 時間、$O(N)$ 追加領域

## **TreeDiameterResult**

```cpp
template <typename Weight> struct TreeDiameterResult
```

直径の長さ・端点・パス・中心を保持する結果型です。

- `distance`: 直径の重み付き長さ。
- `endpoint_u`, `endpoint_v`: 直径の両端。
- `path`: `endpoint_u` から `endpoint_v` への頂点列。
- `centers`: 直径の幾何的な中心の表現。
