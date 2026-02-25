# Weighted DSU（重み付き Union-Find）

頂点に「ポテンシャル」を持たせた Union-Find です。同一連結成分内の 2 頂点 $a$, $b$ について、`diff(a, b)` で「$P(a) - P(b)$」を取得できます。アーベル群の演算で重みを管理します。

---

## テンプレート引数

- `Abel`: アーベル群。`S`, `op(S a, S b)`, `inv(S a)`, `e()` を定義すること。

---

## コンストラクタ

```
(1) weighted_dsu<Abel> dsu;
(2) weighted_dsu<Abel> dsu(i32 n);
```

- **(1)** 頂点数 0 で構築します。
- **(2)** 頂点数 `n` で構築します。各頂点は自分自身のみの連結成分で、ポテンシャルは `e()` です。

**計算量**

- (2): $O(n)$

---

## leader

```
i32 dsu.leader(i32 a);
```

頂点 `a` の属する連結成分の代表（根）を返します。経路圧縮により木が更新されます。

**制約**

- $0 \leq a < n$

**計算量**

- ならし $O(\alpha(n))$

---

## same

```
bool dsu.same(i32 a, i32 b);
```

頂点 `a` と `b` が同じ連結成分に属するかどうかを返します。

**制約**

- $0 \leq a, b < n$

**計算量**

- ならし $O(\alpha(n))$

---

## potential

```
S dsu.potential(i32 a);
```

頂点 `a` のポテンシャルと代表のポテンシャルの差「$P(a) - P(\mathrm{leader}(a))$」を返します。

**制約**

- $0 \leq a < n$

**計算量**

- ならし $O(\alpha(n))$

---

## diff

```
S dsu.diff(i32 a, i32 b);
```

$P(a) - P(b)$ を返します。`a` と `b` が同じ連結成分でない場合の挙動は未定義（または実装依存）です。

**制約**

- $0 \leq a, b < n$
- `same(a, b) == true` であること（推奨）

**計算量**

- ならし $O(\alpha(n))$

---

## merge

```
i32 dsu.merge(i32 a, i32 b, S w);
```

「$P(a) - P(b) = w$」となるように $a$ の連結成分と $b$ の連結成分をマージします。

- 成功時: 新しい代表の頂点 ID を返します。
- 失敗時: 制約が矛盾する（既に同じ連結成分で $w$ と合わない）場合は `-1` を返します。

**制約**

- $0 \leq a, b < n$

**計算量**

- ならし $O(\alpha(n))$

---

## size

```
i32 dsu.size(i32 a);
```

頂点 `a` の属する連結成分のサイズ（頂点数）を返します。

**制約**

- $0 \leq a < n$

**計算量**

- ならし $O(\alpha(n))$

---

## groups

```
vector<vector<i32>> dsu.groups();
```

各連結成分を、代表を先頭とする頂点 ID の列の列として返します。

**計算量**

- $O(n)$

---

## 使用例

```cpp
#include "gwen/graph/weighted_dsu.hpp"
#include "gwen/algebra/basic_monoid.hpp"

using namespace gwen;

weighted_dsu<sum_monoid<i64>> dsu(4);
dsu.merge(0, 1, 10);   // P(0) - P(1) = 10
dsu.merge(1, 2, 5);    // P(1) - P(2) = 5
dsu.diff(0, 2);        // 15
dsu.same(0, 2);        // true
```
