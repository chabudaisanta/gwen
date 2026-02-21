# Associative Treap (キー区間の遅延セグ木)

キーで BST、優先度でヒープになる treap に、キー→値の対応と**遅延伝搬**を載せたものです。キー区間 $[key_l, key_r)$ に対して、遅延セグ木と同様の区間積・区間作用ができます（動的・キーで split）。

- キーに対応する値の set / get / erase
- キー区間の総積（`prod`）
- キー区間への作用（`apply`）

**型**: `associative_treap<Key, Monoid, Compare = std::less<Key>>`

**テンプレート引数**

- `Key` — キーの型。`Compare` で順序が定義されていること。
- `Monoid` — 値のモノイドと作用。`implicit_treap` と同様に以下を用意する。
  - `S`, `F`, `op(S,S)`, `e()`, `mapping(F,S)`, `composition(F,F)`, `id()`
- `Compare` — キーの狭義弱順序（デフォルト `std::less<Key>`）。

**キー区間**: 半開区間 $[key_l, key_r)$（$key_l \leq k < key_r$ のキーだけが対象）。

**計算量**: 各操作 $O(\log n)$（$n$ は登録キー数）。比較・モノイド演算は $O(1)$ と仮定。

---

## 主なメソッド

| メソッド | 説明 | 計算量 |
|----------|------|--------|
| `set(key, value)` | キー `key` の値を `value` に設定（上書き）。key は一意（map 相当） | $O(\log n)$ |
| `erase(key)` | キー `key` を削除（無ければ何もしない） | $O(\log n)$ |
| `get(key)` | キー `key` の値（無ければ `Monoid::e()`） | $O(\log n)$ |
| `prod(key_l, key_r)` | キーが $[key_l, key_r)$ の区間の総積 | $O(\log n)$ |
| `apply(key_l, key_r, f)` | キーが $[key_l, key_r)$ の区間に作用 `f` を適用 | $O(\log n)$ |
| `all_prod()` | 全キーに関する総積（空なら `e()`） | $O(\log n)$ |
| `all_apply(f)` | 全キーに作用 `f` を適用（遅延のみ $O(1)$） | $O(1)$ |
| `size()`, `empty()` | キー数・空判定 | $O(1)$ |
| `to_vec()` | デバッグ用：`(key, val)` をキー昇順に並べた `vector` | $O(n)$ |

---

## 使用例（区間アフィン・区間和）

`range_affine_range_sum_monoid<T>` を使い、キー（例: 座標）ごとに値を 1 つ持つ場合。

```cpp
#include "gwen/container/associative_treap.hpp"
#include "gwen/algebra/range_affine_range_sum_monoid.hpp"
using namespace gwen;

using Monoid = range_affine_range_sum_monoid<i64>;
associative_treap<i32, Monoid> t;

t.set(0, Monoid::unit(10));   // キー 0 の値を 10 に
t.set(2, Monoid::unit(20));   // キー 2 の値を 20 に
t.get(0);                     // {10, 1}
t.prod(0, 3).val;             // キー 0,2 の和 = 30（キー 1 は未設定なので e() で 0）
t.apply(1, 3, Monoid::F{2, 0}); // キー [1,3) に x -> 2*x を作用（キー 2 の値が 40 に）
t.all_prod();
```

キーが存在しない区間は「そのキーは無い」として扱われ、`prod` では `e()` が使われます。遅延セグ木でできる区間クエリ・区間更新は、キー区間で同様に扱えます。
