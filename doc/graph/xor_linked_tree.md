# XOR Linked Tree（xor_tree_scan / get_dfs_order）

木を「XOR リンク」で辿って親配列と（オプションで）葉から根への順序を求めるユーティリティです。メモリを節約した木の走査に使います。

参照: [Codeforces Blog](https://codeforces.com/blog/entry/135239)

---

## xor_tree_scan

```
template <typename Edge, bool NeedOrd>
xor_tree_scan_info xor_tree_scan(const vector<Edge>& E, i32 root);
```

- **入力**:
  - `E`: 辺の列。各辺は `u`, `v` を持つ型。頂点数は `E.size() + 1` です。
  - `root`: 根とする頂点の ID。
- **出力**: `xor_tree_scan_info`。メンバは `par`（各頂点の親）と、`NeedOrd == true` のとき `ord`（葉から根方向の順序）です。
- `par[root]` は `-1` に設定されます。

**制約**

- $0 \leq \mathrm{root} < N$（$N = |E| + 1$）
- 入力は木であること（連結で閉路なし）

**計算量**

- $O(N)$

---

## xor_tree_scan_info

```
struct xor_tree_scan_info {
    vector<i32> par;  // 親。根は -1
    vector<i32> ord;  // NeedOrd が true のとき、葉→根の順序
};
```

---

## get_dfs_order

```
vector<i32> get_dfs_order(const xor_tree_scan_info& info);
```

`xor_tree_scan` の結果（`NeedOrd == true` で取得した `ord` が入っているもの）から、各頂点の DFS 行きがけ順のインデックスを計算します。

- `info.ord` の長さは `par.size() - 1`（根を除く頂点数）であることを仮定します。
- 返り値 `dfs_ord` は長さ $N$ の配列で、`dfs_ord[i]` が頂点 `i` の DFS 順の位置（0-based）です。

**計算量**

- $O(N)$

---

## 使用例

```cpp
#include "gwen/graph/xor_linked_tree.hpp"

using namespace gwen;

struct edge { i32 u, v; };
vector<edge> E = {{0, 1}, {1, 2}, {1, 3}};  // 0 が根の木
auto info = xor_tree_scan<edge, true>(E, 0);
// info.par: 0 の親=-1, 1 の親=0, 2 の親=1, 3 の親=1
// info.ord: 葉から根への順序
auto dfs_ord = get_dfs_order(info);
```
