---
title: Rerooting (全方位木 DP)
documentation_of: //include/gwen/graph/rerooting.hpp
---

# Rerooting

無向木のすべての頂点を根とした DP の値を線形時間で求める。

## rerooting

```cpp
template <edge E, std::copyable T, typename Merge, typename AddVertex, typename Lift>
std::vector<T> rerooting(const GraphBase<E, false>& g, T identity, Merge merge, AddVertex add_vertex, Lift lift)

template <monoid M, edge E, typename AddVertex, typename Lift>
std::vector<typename M::S> rerooting(const GraphBase<E, false>& g, AddVertex add_vertex, Lift lift)
```

`merge` で隣接頂点からの寄与を結合し、`add_vertex` で頂点自身を加える。`lift` は隣接頂点で完成した値を辺越しの寄与に変換する。

`rerooting<M>(g, add_vertex, lift)` は `M::S` を値型、`M::e()` を単位元、`M::op` を結合演算として使う。

各頂点 `v` では `g.edges(v)` の順に寄与を結合する。非可換な `merge` も利用できる。`lift(value, e)` に渡す辺は、値を受け取る頂点 `e.u` から送信元 `e.v` へ向く。戻り値の `answer[v]` は `v` を根とした値を表す。空グラフでは空配列を返す。

**制約**

- `g.build()` を呼び出した無向木を渡す。空グラフも許す。
- `merge` は結合律を満たし、`identity` は両側単位元である。
- `T` は `std::copyable` を満たし、`merge(T,T)`、`add_vertex(T,i32)`、`lift(T,const E&)` は `T` に変換可能な値を返す。
- モノイド指定版では `M` が `gwen::monoid`、`M::S` が `std::copyable` を満たす。`add_vertex` と `lift` は `M::S` に変換可能な値を返す。

**計算量**

- コールバックが定数時間なら $O(N)$ 時間・空間。

## 使用例

各頂点から最も遠い頂点までの重み付き距離を求める。

```cpp
NonDirectedGraph<i64> g(n);
// 辺を追加してから g.build() を呼ぶ。
auto height = rerooting<max_monoid<i64>>(
    g, [](i64 value, i32) { return std::max(value, i64{0}); },
    [](i64 value, const Edge<i64>& e) { return value + e.w; });
```
