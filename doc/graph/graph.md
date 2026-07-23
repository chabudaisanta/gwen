---
title: GraphBase
documentation_of: //include/gwen/graph/graph.hpp
---

# GraphBase

CSR形式をベースにした静的グラフの基底クラスです。
頂点数とエッジのリストを登録し、`build()` を呼び出すことで高速な隣接リストを構築します。

## **コンストラクタ**

```cpp
GraphBase(i32 n)
GraphBase(i32 n, const std::vector<EdgeType>& edges)
```

- $n$ 頂点のグラフを作成します。

**制約**

- $n \ge 0$

**計算量**

- $O(N)$

## **add_edge**

```cpp
void add_edge(const EdgeType& e)
void add_edge(i32 u, i32 v)
void add_edge(i32 u, i32 v, weight_type w)
```

グラフにエッジを追加します。`build()` 呼び出し前のみ可能です。

**制約**

- `build()` 呼び出し前であること

**計算量**

- $O(1)$ 償却

## **build**

```cpp
void build()
```

追加されたエッジ情報をもとに、内部のCSRデータ構造を構築します。以降のクエリに応答するために必須です。

**計算量**

- $O(N + M)$

## **edges**

```cpp
std::span<const EdgeType> edges(i32 u) const
```

頂点 $u$ から出るエッジ（`EdgeType`）のビューを取得します。

**計算量**

- $O(1)$

## **adjacent / operator[]**

```cpp
auto adjacent(i32 u) const
auto operator[](i32 u) const
```

頂点 $u$ に隣接する頂点（終点 `v`）のビューを取得します。

**使用例**
```cpp
for (i32 v : g[u]) {
    // 頂点 v に対する処理
}
```

**計算量**

- $O(1)$

## **dump**

```cpp
std::string dump() const
```

デバッグ用にデータ構造の内部状態を文字列として返します。

**計算量**

- $O(V + E)$
