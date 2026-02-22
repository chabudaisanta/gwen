# Implicit Treap (implicit_treap)

添字で split する implicit treap です。モノイドで抽象化し、区間への作用（lazy）と区間反転をサポートします。区間の総積取得・区間作用・区間反転・任意位置の挿入・削除を $O(\log n)$ で行います。添字は 0-based、区間は半開区間 $[l, r)$ です。

[Library Checker: Dynamic Sequence Range Affine Range Sum](https://judge.yosupo.jp/problem/dynamic_sequence_range_affine_range_sum) が解けます。

---

## テンプレート引数

- `Monoid`: 以下を定義した型。
  - `S` — 要素／区間の型
  - `F` — 作用の型
  - `op(S a, S b)` — 総積
  - `e()` — 単位元
  - `mapping(F f, S x)` — 作用の適用
  - `composition(F f, F g)` — 作用の合成（先に g、後に f）
  - `id()` — 恒等作用

---

## コンストラクタ

```
(1) implicit_treap<Monoid> t;
(2) implicit_treap<Monoid> t(vector<S> vec);
```

- **(1)** 空の列で構築します。
- **(2)** `vec` の内容をそのまま持つ列で構築します。

**計算量**

- (1): $O(1)$
- (2): $O(n)$

---

## size

```
i32 t.size()
```

列の長さ（要素数）を返します。

**計算量**

- $O(1)$

---

## empty

```
bool t.empty()
```

列が空なら `true`、そうでなければ `false` を返します。

**計算量**

- $O(1)$

---

## insert

```
void t.insert(i32 pos, const S& x);
```

位置 `pos` に要素 `x`（型 `S`）を挿入します。

**制約**

- $0 \leq \mathrm{pos} \leq n$

**計算量**

- $O(\log n)$

---

## erase

```
void t.erase(i32 pos);
```

位置 `pos` の要素を削除します。

**制約**

- $0 \leq \mathrm{pos} < n$

**計算量**

- $O(\log n)$

---

## set

```
void t.set(i32 pos, const S& x);
```

位置 `pos` の要素を `x` に書き換えます。

**制約**

- $0 \leq \mathrm{pos} < n$

**計算量**

- $O(\log n)$

---

## reverse

```
void t.reverse(i32 l, i32 r);
```

区間 $[l, r)$ を反転します。

**制約**

- $0 \leq l \leq r \leq n$

**計算量**

- $O(\log n)$

---

## apply

```
void t.apply(i32 l, i32 r, const F& f);
```

区間 $[l, r)$ に作用 `f`（型 `F`）を適用します。

**制約**

- $0 \leq l \leq r \leq n$

**計算量**

- $O(\log n)$

---

## all_apply

```
void t.all_apply(const F& f);
```

列全体に作用 `f` を適用します。遅延のみで $O(1)$ です。

**計算量**

- $O(1)$

---

## prod

```
S t.prod(i32 l, i32 r);
```

区間 $[l, r)$ の総積（型 `S`）を返します。$l = r$ のときは `Monoid::e()` を返します。

**制約**

- $0 \leq l \leq r \leq n$

**計算量**

- $O(\log n)$

---

## all_prod

```
S t.all_prod();
```

列全体の総積を返します。空のときは `Monoid::e()` を返します。

**計算量**

- $O(\log n)$

---

## get

```
S t.get(i32 pos);
```

位置 `pos` の要素（型 `S`）を返します。

**制約**

- $0 \leq \mathrm{pos} < n$

**計算量**

- $O(\log n)$

---

## concat

```
void t.concat(implicit_treap& other);
static implicit_treap implicit_treap::concat(implicit_treap& t0, implicit_treap& t1);
```

- **メンバ**: 自分の末尾に `other` の列を連結し、`other` を空にします。
- **static**: `t0` と `t1` を連結した新しい implicit_treap を返し、`t0` と `t1` を空にします。

**計算量**

- $O(\log n)$

---

## push_back

```
void t.push_back(const S& x);
```

列の末尾に `x` を追加します。`insert(t.size(), x)` と同じです。

**計算量**

- $O(\log n)$

---

## push_front

```
void t.push_front(const S& x);
```

列の先頭に `x` を追加します。`insert(0, x)` と同じです。

**計算量**

- $O(\log n)$

---

## to_vec

```
vector<S> t.to_vec();
```

デバッグ用。in-order DFS で走査し、遅延・反転を反映した列の順で `val` を並べた `vector` を返します。

**計算量**

- $O(n)$

---

## 使用例（区間アフィン・区間和）

`range_affine_range_sum_monoid<T>` を使う場合。1 要素は `Monoid::unit(x)` で作ります。

```cpp
#include "gwen/container/implicit_treap.hpp"
#include "gwen/algebra/range_affine_range_sum_monoid.hpp"
#include <atcoder/all>
using mint = atcoder::modint998244353;
using Monoid = gwen::range_affine_range_sum_monoid<mint>;
using namespace gwen;

implicit_treap<Monoid> t;
t.insert(i, Monoid::unit(mint(x)));
t.erase(i);
t.reverse(l, r);
t.apply(l, r, Monoid::F{mint(b), mint(c)});
cout << t.prod(l, r).val.val() << '\n';
```
