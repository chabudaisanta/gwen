# Treap (treap)

[モノイド](https://ja.wikipedia.org/wiki/%E3%83%A2%E3%83%8E%E3%82%A4%E3%83%89) $(S, \cdot: S \times S \to S, e \in S)$ に対し使用できる implicit treap（平衡二分探索木）です。長さ $n$ の列に対し、区間の総積の取得・任意位置への挿入・削除を $O(\log n)$ で行います。添字は 0-based、区間は半開区間 $[l, r)$ です。

---

## テンプレート引数

- `Monoid`: `S`, `op(S a, S b)`, `e()` を定義した型（例: `gwen::sum_monoid<i64>`, `gwen::min_monoid<i32>`）。オラクルは定数時間と仮定します。

---

## コンストラクタ

```
(1) treap<Monoid> t;
(2) treap<Monoid> t(vector<S> vec);
```

- **(1)** 空の列で構築します。
- **(2)** `vec` の内容をそのまま持つ列で構築します。`vec[i]` が位置 $i$ の要素になります。

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

位置 `pos` に要素 `x` を挿入します。挿入後、`x` は `pos` 番目（0-based）になります。

**制約**

- $0 \leq \mathrm{pos} \leq n$（$n$ は挿入前の長さ）

**計算量**

- $O(\log n)$

---

## erase

```
void t.erase(i32 pos);
```

位置 `pos` の要素を 1 つ削除します。

**制約**

- $0 \leq \mathrm{pos} < n$

**計算量**

- $O(\log n)$

---

## prod

```
S t.prod(i32 l, i32 r);
```

$op(a[l], a[l+1], \ldots, a[r-1])$ を返します。$l = r$ のときは `Monoid::e()` を返します。

**制約**

- $0 \leq l \leq r \leq n$

**計算量**

- $O(\log n)$

**注意**: 内部で split/merge を行うため、このメソッドは非 const です。

---

## get

```
S t.get(i32 pos);
```

位置 `pos` の要素 $a[\mathrm{pos}]$ を返します。`prod(pos, pos + 1)` と同じです。

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

## all_prod

```
S t.all_prod();
```

列全体の総積 $op(a[0], \ldots, a[n-1])$ を返します。空のときは `Monoid::e()` を返します。

**計算量**

- $O(1)$

---

## concat

```
void t.concat(treap& other);
static treap treap::concat(treap& t0, treap& t1);
```

- **メンバ**: 自分の列の末尾に `other` の列を連結します。呼び出し後、`other` は空になります。
- **static**: `t0` の列と `t1` の列を連結した新しい treap を返します。呼び出し後、`t0` と `t1` は空になります。

**計算量**

- $O(\log n)$（$n$ は連結後の長さ）

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
vector<S> t.to_vec() const;
```

デバッグ用。in-order DFS で走査し、添字順（列の順序）に `val` を並べた `vector` を返します。

**計算量**

- $O(n)$

---

## 使用例

```cpp
#include "gwen/container/treap.hpp"
#include "gwen/algebra/basic_monoid.hpp"

using namespace gwen;

using T = treap<sum_monoid<i64>>;
T t;
t.push_back(1);
t.push_back(2);
t.insert(1, 10);           // 列は [1, 10, 2]
t.prod(0, 3);              // 1 + 10 + 2 = 13
t.get(1);                  // 10
t.erase(1);                // 列は [1, 2]
t.size();                  // 2
```

初期値付きで構築する例:

```cpp
std::vector<i64> v = {1, 2, 3, 4, 5};
treap<sum_monoid<i64>> t(v);
t.prod(0, 5);              // 15
t.insert(2, 100);          // [1, 2, 100, 3, 4, 5]
```
