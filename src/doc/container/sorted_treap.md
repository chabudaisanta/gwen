# Sorted Treap (順序付き多重集合)

キーで BST、優先度でヒープになる treap による**順序付き多重集合**です。`std::multiset` に近い API で、イテレータはありません。

- 挿入・削除（1 要素ずつ）
- **k 番目に小さい要素**の取得（`kth`）
- key 未満の要素数（`count_lower`）
- 個数（`count(x)`, `count(lower, upper)`）、存在判定（`contains`）
- x 以上の最小・x を超える最小のインデックスとノード id（`lower_bound`, `upper_bound`）、`get_val(id)` で値を取得

**型**: `sorted_treap<K, Compare = std::less<K>>`

**テンプレート引数**

- `K` — 要素の型（キー）。`Compare` で順序が定義されていること。
- `Compare` — 狭義弱順序（デフォルト `std::less<K>`）。

**計算量**: 操作はすべて $O(\log n)$（$n$ は要素数）。比較が $O(1)$ と仮定。

---

## 主なメソッド

| メソッド | 説明 | 計算量 |
|----------|------|--------|
| `insert(key)` | 要素 `key` を 1 つ追加（重複可） | $O(\log n)$ |
| `erase(key)` | `key` を 1 つ削除（無ければ何もしない） | $O(\log n)$ |
| `kth(k)` | 0-based で k 番目に小さい要素（`kth(0)` が最小） | $O(\log n)$ |
| `count_lower(key)` | `key` 未満の要素の個数 | $O(\log n)$ |
| `count(x)` | `x` と等しい要素の個数 | $O(\log n)$ |
| `count(lower, upper)` | キーが $[\mathit{lower}, \mathit{upper})$ の要素の個数 | $O(\log n)$ |
| `contains(x)` | `x` が 1 つ以上存在するか | $O(\log n)$ |
| `lower_bound(x)` | $x$ 以上の最小の要素の (インデックス, ノード id)。無ければ `(size(), NIL)` | $O(\log n)$ |
| `upper_bound(x)` | $x$ を超える最小の要素の (インデックス, ノード id)。無ければ `(size(), NIL)` | $O(\log n)$ |
| `get_val(id)` (static) | ノード id のキー。`id == NIL` のときは assert | $O(1)$ |
| `size()` | 要素数 | $O(1)$ |
| `empty()` | 空なら `true` | $O(1)$ |
| `to_vec()` | デバッグ用：キーを昇順に並べた `vector` | $O(n)$ |

**注意**: `kth`, `count_lower`, `count`, `contains`, `lower_bound`, `upper_bound` は内部で split/merge するため非 const。ノード id は `tree`（`i32`）型で、`lower_bound` / `upper_bound` の第二要素。`NIL` は 0。

---

## 使用例

```cpp
#include "gwen/container/sorted_treap.hpp"
using namespace gwen;

sorted_treap<int> t;
t.insert(3);
t.insert(1);
t.insert(2);
t.insert(1);           // 多重集合なので 1 が 2 つ
t.kth(0);              // 1（最小）
t.kth(2);              // 2
t.count_lower(2);       // 2（2 未満が 2 個）
t.erase(1);            // 1 を 1 つ削除
t.size();             // 3
t.count(1);           // 1 の個数
t.count(1, 3);        // キーが [1, 3) の個数
t.contains(2);        // true
auto [idx, id] = t.lower_bound(2);  // idx == 1, sorted_treap<int>::get_val(id) == 2
auto [idx2, id2] = t.upper_bound(2); // idx2 == 2（2 を超える最小のインデックス）
```

比較を変える例（降順で「k 番目に大きい」が欲しい場合など）は、`Compare` を差し替えて使います。
