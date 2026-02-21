# Mo's Algorithm（区間クエリのオフライン処理）

区間 $[l, r)$ に対するクエリを、Hilbert 曲線の順でソートして処理するオフラインアルゴリズムです。区間の伸縮コストが $O(1)$ のとき、全体で $O(n \sqrt{q})$ 程度になります。

参照: [AC Library document_ja](https://atcoder.github.io/ac-library/master/document_ja/)  
参考: [take44444 Algorithm-Book](https://take44444.github.io/Algorithm-Book/range/mo/main.html)

---

## コンストラクタ

```
mo_algorithm mo(i32 n);
```

- **入力**: 列の長さ `n`。クエリの区間は $[0, n)$ 内であることを想定します。
- 内部で Hilbert 順用に $n$ を 2 のべきに切り上げた値 `bc` を計算します。

**計算量**

- $O(1)$

---

## add_query

```
void mo.add_query(i32 l, i32 r);
```

区間 $[l, r)$ に対するクエリを 1 件追加します。追加順がクエリの ID（0-based）になります。

**制約**

- $0 \leq l \leq r \leq n$

**計算量**

- $O(1)$

---

## solve

```
auto mo.solve(IL increment_l, DL decrement_l, IR increment_r, DR decrement_r, GR get_res);
```

Hilbert 順にクエリを並べ替え、現在の区間 $[l, r)$ を伸縮しながら各クエリの答えを取得します。

- **引数**:
  - `increment_l(l, r)`: 区間の左端を 1 減らす（$l$ を $l-1$ にして、$l-1$ を区間に含める処理）。
  - `decrement_l(l, r)`: 区間の左端を 1 増やす（$l$ を区間から外す処理）。
  - `increment_r(l, r)`: 区間の右端を 1 増やす（$r$ を $r+1$ にして、$r$ を区間に含める処理）。呼び出し時点では `r` はまだ増えていないので、`r` を追加する処理を書く。
  - `decrement_r(l, r)`: 区間の右端を 1 減らす（$r-1$ を区間から外す処理）。
  - `get_res(idx)`: 現在の区間がクエリ `idx` の区間と一致したときに、そのクエリの答えを返す関数。
- **返り値**: クエリ ID 順の答えの列（`vector<S>` など）。

**計算量**

- 伸縮が $O(1)$ なら、全体 $O(n \sqrt{q} + q \log q)$ 程度（Hilbert 順により定数が良い）。

---

## 使用例

```cpp
#include "gwen/query/mo.hpp"

using namespace gwen;

i32 n = 100, q = 10;
mo_algorithm mo(n);
mo.add_query(0, 10);
mo.add_query(5, 15);
vector<i32> a(n);

auto res = mo.solve(
    [&](i32 l, i32 r) { /* l を左に伸ばす */ },
    [&](i32 l, i32 r) { /* l を縮める */ },
    [&](i32 l, i32 r) { /* r を右に伸ばす: a[r] を追加 */ },
    [&](i32 l, i32 r) { /* r を縮める */ },
    [&](i32 idx) { return /* 現在の区間の答え */; }
);
```
