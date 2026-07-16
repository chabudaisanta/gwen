---
title: CSR (Compressed Sparse Row)
documentation_of: //include/gwen/graph/csr.hpp
---

# Csr

CSR (Compressed Sparse Row) 形式によるグラフの内部表現です。
構築後に変更を加えない静的なグラフ構造に対して、高いメモリ効率とキャッシュ効率を提供します。通常は `GraphBase` 内部で利用されます。

## コンストラクタ

```cpp
Csr(i32 n, const std::vector<E>& edges, bool directed = true)
```

- $n$ 頂点のグラフを構築します。
- `directed = true` の場合は有向グラフ、`false` の場合は無向グラフとして扱います（無向の場合は各エッジの逆辺も追加されます）。

**計算量**

- $O(n + |edges|)$

## edges

```cpp
std::span<E> edges(i32 v)
```

頂点 $v$ から出るエッジのリスト（ビュー）を返します。

**計算量**

- $O(1)$
