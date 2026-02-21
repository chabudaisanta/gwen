# Rolling Hash

列の区間のハッシュを $O(1)$ で取得するデータ構造です。`rhash::rolling_hash_monoid` と組み合わせて、区間のハッシュ値と「長さに対応するベキ」のペアを返します。文字列の一致判定や LCP に使えます。

参照: [AC Library document_ja](https://atcoder.github.io/ac-library/master/document_ja/)

---

## テンプレート引数

- `ID`: 複数モジュールを使い分けるときの識別子（デフォルト `0`）。異なる `ID` で異なる基数・法が使われます。

---

## コンストラクタ

```
template <typename Container>
rolling_hash<ID> rh(const Container& seq);
```

列 `seq`（`std::size` と `seq[i]` でアクセス可能なもの）から構築します。接尾辞区間のハッシュと $r^{\mathrm{len}}$ を前計算します。

**計算量**

- $O(n)$

---

## size

```
i32 rh.size() const;
```

列の長さを返します。

---

## get

```
S rh.get(i32 l, i32 r) const;
```

区間 $[l, r)$ のハッシュを返します。戻り値は `Monoid::S` 型で、通常は `{v, p}` のペアです。

- `v`: 区間のハッシュ。先頭要素の重みを $r^0$ とする正規化された値（$a_l \cdot r^0 + a_{l+1} \cdot r^1 + \ldots + a_{r-1} \cdot r^{r-l-1}$）。
- `p`: $r^{r-l}$（長さに対応するベキ）。

$l = r$ のときは `Monoid::e()` を返します。

**制約**

- $0 \leq l \leq r \leq n$

**計算量**

- $O(1)$

---

## equal

```
bool rh.equal(i32 l1, i32 r1, i32 l2, i32 r2) const;
```

区間 $[l_1, r_1)$ と $[l_2, r_2)$ のハッシュが等しいかどうかを返します。長さが異なれば `false` です。

**制約**

- 各区間が $[0, n]$ 内であること

**計算量**

- $O(1)$

---

## lcp

```
i32 rh.lcp(i32 l1, i32 l2) const;
```

接尾辞 $[l_1, n)$ と $[l_2, n)$ の最長共通接頭辞の長さを返します。二分探索で求めます。

**制約**

- $0 \leq l_1, l_2 \leq n$

**計算量**

- $O(\log n)$

---

## 使用例

```cpp
#include "gwen/hash/rolling_hash.hpp"

using namespace gwen;

string s = "abcabc";
rolling_hash<> rh(s);
rh.get(0, 3);           // "abc" のハッシュ
rh.equal(0, 3, 3, 6);   // true（"abc" == "abc"）
rh.lcp(0, 3);           // 3
```
