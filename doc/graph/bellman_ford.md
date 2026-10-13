---
title: Bellman-Ford
documentation_of: //include/gwen/graph/bellman_ford.hpp
---

# Bellman-Ford

負辺を含むグラフの単一始点最短距離と負閉路の影響範囲を求める。各関数は結果を値で返す。

辺の重みは 64 bit 以下の符号付き整数型とする。計算中の緩和には `i128` を用いる。各有限な最短距離は重み型で表現でき、`std::numeric_limits<weight_type>::max()` と異なる必要がある。

以下では $N$ を頂点数、$M$ を `g.edges(u)` に現れる有向辺の総数とする。無向グラフの辺は両方向の有向辺として扱う。

## **BellmanFordResult**

```cpp
template <std::signed_integral T>
struct BellmanFordResult {
    std::vector<T> dist;
    std::vector<bool> negative_infinite;
};
```

`negative_infinite[v]` は、始点から到達できる負閉路を経由して頂点 $v$ に到達できる場合に真となる。その場合 `dist[v]` の `0` はダミー値であり、距離として使えない。到達不能な頂点の `dist[v]` は重み型の最大値となる。

**制約**

- `T` は符号付き整数型。

**計算量**

- 結果の保持に $O(N)$ の空間を使う。

## **bellman_ford**

```cpp
template <graph G>
std::vector<typename G::weight_type> bellman_ford(const G& g, i32 s);
```

負閉路を検出せず、始点 $s$ から各頂点への最短距離を返す。到達不能な頂点は重み型の最大値となる。

**制約**

- `g` は構築済みで、$0 \le s < N$。
- `G::weight_type` は 64 bit 以下の符号付き整数型。
- $s$ から負閉路に到達できない。
- 有限な最短距離は重み型で表現でき、最大値と異なる。

**計算量**

- 時間 $O(NM+N+M)$、空間 $O(N+M)$。

## **bellman_ford_with_negative_cycle**

```cpp
template <graph G>
BellmanFordResult<typename G::weight_type>
bellman_ford_with_negative_cycle(const G& g, i32 s);
```

始点 $s$ からの最短距離と、始点から到達可能な負閉路の影響を受ける頂点を返す。始点から到達できない負閉路は結果に影響しない。

**制約**

- `g` は構築済みで、$0 \le s < N$。
- `G::weight_type` は 64 bit 以下の符号付き整数型。
- 有効な有限最短距離は重み型で表現でき、最大値と異なる。

**計算量**

- 時間 $O(NM+N+M)$、空間 $O(N+M)$。

## **vertices_reaching_negative_cycle**

```cpp
template <graph G>
std::vector<bool> vertices_reaching_negative_cycle(const G& g);
```

各頂点からグラフ内のいずれかの負閉路へ到達できるかを返す。始点には依存しない。

**制約**

- `g` は構築済み。
- `G::weight_type` は 64 bit 以下の符号付き整数型。

**計算量**

- 時間 $O(NM+N+M)$、空間 $O(N+M)$。

## **vertices_reachable_from_negative_cycle**

```cpp
template <graph G>
std::vector<bool> vertices_reachable_from_negative_cycle(const G& g);
```

グラフ内のいずれかの負閉路から各頂点へ到達できるかを返す。始点には依存しない。

**制約**

- `g` は構築済み。
- `G::weight_type` は 64 bit 以下の符号付き整数型。

**計算量**

- 時間 $O(NM+N+M)$、空間 $O(N+M)$。
