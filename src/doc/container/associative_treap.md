# Associative Treap (associative_treap)

キーで BST、優先度でヒープになる treap に、キー→値の対応と遅延伝搬を載せたものです。キー区間 $[key_l, key_r)$ に対して、遅延セグ木と同様の区間積・区間作用ができます（動的・キーで split）。キーは一意（map 相当）です。

---

## テンプレート引数

- `Key`: キーの型。`Compare` で順序が定義されていること。
- `Monoid`: 値のモノイドと作用。`S`, `F`, `op(S,S)`, `e()`, `mapping(F,S)`, `composition(F,F)`, `id()` を定義すること。
- `Compare`: キーの狭義弱順序（デフォルト `std::less<Key>`）。

キー区間は半開区間 $[key_l, key_r)$（$key_l \leq k < key_r$ のキーが対象）です。

---

## コンストラクタ

```
associative_treap<Key, Monoid, Compare> t;
```

空の連想配列で構築します。

**計算量**

- $O(1)$

---

## size

```
i32 t.size()
```

登録されているキー数を返します。

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

## set

```
void t.set(const Key& key, const S& value);
```

キー `key` の値を `value` に設定します。既に存在する場合は上書きします。

**計算量**

- $O(\log n)$

---

## erase

```
void t.erase(const Key& key);
```

キー `key` を削除します。存在しない場合は何もしません。

**計算量**

- $O(\log n)$

---

## get

```
S t.get(const Key& key);
```

キー `key` の値を返します。存在しない場合は `Monoid::e()` を返します。

**計算量**

- $O(\log n)$

---

## prod

```
S t.prod(const Key& key_l, const Key& key_r);
```

キーが $[key_l, key_r)$ の区間の総積を返します。該当キーが無い場合は `Monoid::e()` を返します。

**計算量**

- $O(\log n)$

---

## apply

```
void t.apply(const Key& key_l, const Key& key_r, const F& f);
```

キーが $[key_l, key_r)$ の区間に作用 `f` を適用します。

**計算量**

- $O(\log n)$

---

## all_prod

```
S t.all_prod();
```

全キーに関する総積を返します。空のときは `Monoid::e()` を返します。

**計算量**

- $O(\log n)$

---

## all_apply

```
void t.all_apply(const F& f);
```

全キーに作用 `f` を適用します。遅延のみで $O(1)$ です。

**計算量**

- $O(1)$

---

## contains

```
bool t.contains(const Key& key);
```

キー `key` が存在すれば `true`、そうでなければ `false` を返します。

**計算量**

- $O(\log n)$

---

## count

```
i32 t.count(const Key& key);
```

キーは一意のため、`key` が存在すれば 1、そうでなければ 0 を返します。`contains` と同等です。

**計算量**

- $O(\log n)$

---

## kth

```
pair<Key, S> t.kth(i32 k);
```

0-based で k 番目に小さいキーとその値 `(Key, S)` を返します。

**制約**

- $0 \leq k < n$

**計算量**

- $O(\log n)$

---

## lower_bound

```
pair<i32, tree> t.lower_bound(const Key& key);
```

`key` 以上の最小のキーをもつ要素の (0-based インデックス, ノード id) を返します。該当がなければ `(size(), NIL)` を返します。`get_key(id)` / `get_val(id)` でキー・値を取得できます。

**計算量**

- $O(\log n)$

---

## upper_bound

```
pair<i32, tree> t.upper_bound(const Key& key);
```

`key` を超える最小のキーをもつ要素の (0-based インデックス, ノード id) を返します。該当がなければ `(size(), NIL)` を返します。

**計算量**

- $O(\log n)$

---

## get_key

```
static Key associative_treap::get_key(tree id);
```

ノード id のキーを返します。

**制約**

- `id != NIL`（違反時は assert）

**計算量**

- $O(1)$

---

## get_val

```
static S associative_treap::get_val(tree id);
```

ノード id の値を返します。遅延を push してから返します。

**制約**

- `id != NIL`（違反時は assert）

**計算量**

- $O(\log n)$

---

## to_vec

```
vector<pair<Key, S>> t.to_vec();
```

デバッグ用。`(key, val)` をキー昇順に並べた `vector` を返します。

**計算量**

- $O(n)$

---

## 使用例（区間アフィン・区間和）

```cpp
#include "gwen/container/associative_treap.hpp"
#include "gwen/algebra/range_affine_range_sum_monoid.hpp"
using namespace gwen;

using Monoid = range_affine_range_sum_monoid<i64>;
associative_treap<i32, Monoid> t;

t.set(0, Monoid::unit(10));   // キー 0 の値を 10 に
t.set(2, Monoid::unit(20));   // キー 2 の値を 20 に
t.get(0);                     // {10, 1}
t.prod(0, 3).val;             // キー 0,2 の和 = 30
t.apply(1, 3, Monoid::F{2, 0}); // キー [1,3) に x -> 2*x を作用
t.all_prod();
t.contains(2);                // true
t.count(1);                   // 0
auto [k, v] = t.kth(1);       // k == 2, v はキー 2 の値
auto [idx, id] = t.lower_bound(1); // idx == 1, get_key(id) == 2
```
