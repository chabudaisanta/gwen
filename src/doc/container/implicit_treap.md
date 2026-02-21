# Implicit Treap (区間作用・区間反転・区間取得)

添字で split する implicit treap です。`treap.hpp` と同様にモノイドで抽象化し、さらに区間への作用（lazy）と区間反転をサポートします。

- 区間の総積（モノイド積）の取得
- 区間への作用の適用
- 区間反転
- 任意位置への挿入・削除

[Library Checker: Dynamic Sequence Range Affine Range Sum](https://judge.yosupo.jp/problem/dynamic_sequence_range_affine_range_sum) が解けます。

**型**: `implicit_treap<Monoid>`

**Monoid** に必要なもの:

- `S` — 要素／区間の型（1 要素は `S` の値 1 つで表現）
- `F` — 作用の型
- `op(S a, S b)` — 総積
- `e()` — 単位元
- `mapping(F f, S x)` — 作用の適用
- `composition(F f, F g)` — 作用の合成（先に g、後に f）
- `id()` — 恒等作用

**区間**: 0-based の半開区間 $[l, r)$。

---

## 主なメソッド

| メソッド | 説明 | 計算量 |
|----------|------|--------|
| `insert(pos, x)` | 位置 `pos` に要素 `x`（型 `S`）を挿入 | $O(\log n)$ |
| `erase(pos)` | 位置 `pos` の要素を削除 | $O(\log n)$ |
| `reverse(l, r)` | $[l, r)$ を反転 | $O(\log n)$ |
| `apply(l, r, f)` | $[l, r)$ に作用 `f`（型 `F`）を適用 | $O(\log n)$ |
| `prod(l, r)` | $[l, r)$ の総積（型 `S`）を返す | $O(\log n)$ |
| `get(pos)` | 位置 `pos` の要素（型 `S`）を返す | $O(\log n)$ |

---

## 使用例（区間アフィン・区間和）

`range_affine_range_sum_monoid<T>` を使う場合。`S = {val, len}`、`F = {a, b}`（$x \mapsto ax + b$）、1 要素は `Monoid::unit(x)` で作ります。

```cpp
#include "gwen/container/implicit_treap.hpp"
#include "gwen/algebra/range_affine_range_sum_monoid.hpp"
#include <atcoder/all>
using mint = atcoder::modint998244353;
using Monoid = gwen::range_affine_range_sum_monoid<mint>;
using namespace gwen;

implicit_treap<Monoid> t;
// 0: insert(i, x)  →  t.insert(i, Monoid::unit(mint(x)));
// 1: erase(i)       →  t.erase(i);
// 2: reverse(l, r)  →  t.reverse(l, r);
// 3: apply(l, r, b, c)  →  t.apply(l, r, Monoid::F{mint(b), mint(c)});
// 4: prod(l, r) の和を出力  →  t.prod(l, r).val
t.insert(i, Monoid::unit(mint(x)));
t.erase(i);
t.reverse(l, r);
t.apply(l, r, Monoid::F{mint(b), mint(c)});
cout << t.prod(l, r).val.val() << '\n';
```
