---
title: TreeLca
documentation_of: //include/gwen/graph/tree_lca.hpp
---

# TreeLca

単一の木に対する LCA (最小共通祖先) や距離を求めるクラスです。
ダブリングを用いており、各クエリに $O(\log N)$ で応答します。

## コンストラクタ

```cpp
TreeLca<Edge, IsDirected> lca(int root, const GraphBase<Edge, IsDirected>& G)
```

頂点 `root` を根として、グラフ $G$ から木のダブリング配列を構築します。

**制約**

- `Edge` は `gwen::edge` コンセプトを満たすこと。
- $G$ は木であること。
- $G$ の頂点数を $N$ としたとき、$0 \le N \le 10^8$
- $0 \le \text{root} < N$

**計算量**

- $O(N \log N)$

## kth_anc

```cpp
int lca.kth_anc(int v, int k)
```

頂点 $v$ から根の方向へ $k$ 個戻った祖先の頂点番号を返します。存在しない場合は `-1` を返します。

**制約**

- $0 \le v < N$
- $k \ge 0$

**計算量**

- $O(\log N)$

## depth

```cpp
int lca.depth(int v)
```

頂点 $v$ の根からの深さ（距離）を返します。根の深さは $0$ です。

**制約**

- $0 \le v < N$

**計算量**

- $O(1)$

## lca

```cpp
int lca.lca(int u, int v)
```

頂点 $u$ と $v$ の LCA (最小共通祖先) を返します。

**制約**

- $0 \le u < N$
- $0 \le v < N$

**計算量**

- $O(\log N)$

## len

```cpp
int lca.len(int u, int v)
```

頂点 $u$ と $v$ を結ぶ単純パスに含まれる頂点の数を返します（エッジ数 $+ 1$）。

**制約**

- $0 \le u < N$
- $0 \le v < N$

**計算量**

- $O(\log N)$
