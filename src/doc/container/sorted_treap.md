# Sorted Treap (sorted_treap)

キーで BST、優先度でヒープになる treap による順序付き多重集合です。`std::multiset` に近い API で、イテレータはありません。挿入・削除・k 番目の取得・個数・存在判定・lower_bound / upper_bound を $O(\log n)$ で行います。

---

## テンプレート引数

- `K`: 要素の型（キー）。`Compare` で順序が定義されていること。
- `Compare`: 狭義弱順序（デフォルト `std::less<K>`）。比較は $O(1)$ と仮定します。

---

## コンストラクタ

```
sorted_treap<K, Compare> t;
```

空の多重集合で構築します。

**計算量**

- $O(1)$

---

## size

```
i32 t.size()
```

要素数を返します。

**計算量**

- $O(1)$

---

## empty

```
bool t.empty()
```

空なら `true`、そうでなければ `false` を返します。

**計算量**

- $O(1)$

---

## insert

```
void t.insert(const K& key);
```

要素 `key` を 1 つ追加します。重複可能です。

**計算量**

- $O(\log n)$

---

## erase

```
void t.erase(const K& key);
```

`key` を 1 つ削除します。存在しない場合は何もしません。

**計算量**

- $O(\log n)$

---

## kth

```
K t.kth(i32 k);
```

0-based で k 番目に小さい要素を返します。`kth(0)` が最小です。

**制約**

- $0 \leq k < n$

**計算量**

- $O(\log n)$

**注意**: 内部で split/merge を行うため、このメソッドは非 const です。

---

## count_lower

```
i32 t.count_lower(const K& key);
```

`key` 未満の要素の個数を返します。

**計算量**

- $O(\log n)$

---

## count

```
i32 t.count(const K& x);
i32 t.count(const K& lower, const K& upper);
```

- 1 引数: `x` と等しい要素の個数を返します。
- 2 引数: キーが $[\mathit{lower}, \mathit{upper})$ の要素の個数を返します。

**計算量**

- $O(\log n)$

---

## contains

```
bool t.contains(const K& x);
```

`x` が 1 つ以上存在すれば `true`、そうでなければ `false` を返します。

**計算量**

- $O(\log n)$

---

## lower_bound

```
pair<i32, tree> t.lower_bound(const K& x);
```

$x$ 以上の最小の要素の (0-based インデックス, ノード id) を返します。該当がなければ `(size(), NIL)` を返します。ノード id は `tree`（`i32`）型で、`get_val(id)` でキーを取得できます。

**計算量**

- $O(\log n)$

---

## upper_bound

```
pair<i32, tree> t.upper_bound(const K& x);
```

$x$ を超える最小の要素の (0-based インデックス, ノード id) を返します。該当がなければ `(size(), NIL)` を返します。

**計算量**

- $O(\log n)$

---

## get_val

```
static K sorted_treap::get_val(tree id);
```

ノード id のキーを返します。

**制約**

- `id != NIL`（違反時は assert）

**計算量**

- $O(1)$

---

## to_vec

```
vector<K> t.to_vec() const;
```

デバッグ用。キーを昇順に並べた `vector` を返します。

**計算量**

- $O(n)$

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
t.count_lower(2);      // 2（2 未満が 2 個）
t.erase(1);            // 1 を 1 つ削除
t.size();              // 3
t.count(1);            // 1 の個数
t.count(1, 3);        // キーが [1, 3) の個数
t.contains(2);        // true
auto [idx, id] = t.lower_bound(2);  // idx == 1, sorted_treap<int>::get_val(id) == 2
auto [idx2, id2] = t.upper_bound(2); // idx2 == 2
```
