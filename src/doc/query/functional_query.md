# Functional Query（関数の繰り返し適用・ダブリング）

各要素 $i$ が「次の位置」$P[i]$ を持つ列に対して、「$i$ から $k$ 回遷移した先」と「その区間のモノイド積」を $O(\log k)$ で求めるデータ構造です。ダブリングで前計算します。

参照: [AC Library document_ja](https://atcoder.github.io/ac-library/master/document_ja/)

---

## コンストラクタ

```
functional_query<Monoid> fq(const vector<i32>& P, i64 k_max, auto&& f);
```

- **入力**:
  - `P`: 長さ $N$ の配列。$P[i]$ は $i$ の「次の位置」です。
  - `k_max`: クエリで与えうる最大ステップ数。前計算のダブリング段数は $\lceil \log_2(k_{\max}+1) \rceil$ です。
  - `f(from, to)`: 区間 $[\mathrm{from}, \mathrm{to})$ に対応するモノイドの値を返す関数。`f(i, P[i])` で「$i$ から $P[i]$ への 1 ステップの値」を渡します。
- **前計算**: 各 $i$ について $2^l$ ステップ先の位置と、その区間のモノイド積を格納します。

**計算量**

- $O(N \log k_{\max})$

---

## query

```
pair<i32, S> fq.query(i32 p, i64 k, S initial_value = Monoid::e());
```

位置 `p` から `k` 回遷移した先の位置と、その区間のモノイド積を返します。

- 返り値: `{ 遷移先の位置, 区間の積 }`。区間の積は `initial_value` に「$p$ から遷移先までの積」を左から掛けた値です。

**制約**

- $0 \leq p < N$
- $0 \leq k \leq k_{\max}$（前計算の範囲内）

**計算量**

- $O(\log k)$

---

## max_step

```
tuple<i32, i64, S> fq.max_step(i32 p, auto&& f, S initial_value = Monoid::e());
```

位置 `p` から、「$f(x) = \mathrm{true}$ である限り」進み続けたときの、到達位置・歩数・区間の積を返します。

- `f(x)` は現在までの区間の積 `x` を受け取り、進んでよいかどうかを返します。
- 返り値: `{ 到達位置, 歩数, 区間の積 }`。区間の積は `initial_value` に左から掛けた値です。

**計算量**

- $O(\log k_{\max})$

---

## 使用例

```cpp
#include "gwen/query/functional_query.hpp"
#include "gwen/algebra/basic_monoid.hpp"

using namespace gwen;

vector<i32> P = {1, 2, 0};  // 0->1, 1->2, 2->0
vector<i64> A = {10, 20, 30};
auto fq = functional_query<sum_monoid<i64>>(
    P, 100,
    [&](i32 i, i32 j) { return A[i]; });
auto [pos, sum] = fq.query(0, 3);  // pos=0, sum=60
auto [pos2, step, sum2] = fq.max_step(0, [](i64 x) { return x < 50; });
```
