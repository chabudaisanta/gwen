---
title: Forest
documentation_of: //include/gwen/graph/forest.hpp
---

# Forest

複数の木（森）に対する LCA (最小共通祖先) や距離、k 番目の祖先を求めるデータ構造です。
内部でダブリングを用いており、クエリに $O(\log N)$ で応答します。

## コンストラクタ

```cpp
Forest<Edge, IsDirected> forest(const GraphBase<Edge, IsDirected>& G)
```

グラフ $G$ から森のダブリング配列を構築します。
連結成分ごとに最もインデックスの小さい頂点を根として扱います。

**制約**

- `Edge` は `gwen::edge` コンセプトを満たすこと。
- $G$ の頂点数を $N$ としたとき、$0 \le N \le 10^8$

**計算量**

- $O(N \log N)$

## get_root

```cpp
int forest.get_root(int p)
```

頂点 $p$ が属する木の根を返します。

**制約**

- $0 \le p < N$

**計算量**

- $O(1)$

## get_size

```cpp
int forest.get_size(int p)
```

頂点 $p$ が属する木の頂点数を返します。

**制約**

- $0 \le p < N$

**計算量**

- $O(1)$

## is_same

```cpp
bool forest.is_same(int u, int v)
```

頂点 $u$ と $v$ が同じ木に属するかを判定します。

**制約**

- $0 \le u < N$
- $0 \le v < N$

**計算量**

- $O(1)$

## get_depth

```cpp
int forest.get_depth(int p)
```

頂点 $p$ の根からの深さ（距離）を返します。根の深さは $0$ です。

**制約**

- $0 \le p < N$

**計算量**

- $O(1)$

## get_kth_ancestor

```cpp
int forest.get_kth_ancestor(int v, int k)
```

頂点 $v$ から根の方向へ $k$ 個戻った祖先の頂点番号を返します。存在しない場合は `-1` を返します。

**制約**

- $0 \le v < N$
- $k \ge 0$

**計算量**

- $O(\log N)$

## get_lca

```cpp
int forest.get_lca(int u, int v)
```

頂点 $u$ と $v$ の LCA (最小共通祖先) を返します。同じ木に属さない場合は `-1` を返します。

**制約**

- $0 \le u < N$
- $0 \le v < N$

**計算量**

- $O(\log N)$

## get_dist

```cpp
int forest.get_dist(int u, int v)
```

頂点 $u$ と $v$ 間の最短距離（エッジ数）を返します。同じ木に属さない場合は `-1` を返します。

**制約**

- $0 \le u < N$
- $0 \le v < N$

**計算量**

- $O(\log N)$

## **dump**

```cpp
std::string dump() const
```

デバッグ用にデータ構造の内部状態を文字列として返します。

**計算量**

- $O(N)$
