---
title: ローリングハッシュ (rolling_hash.hpp)
documentation_of: //include/gwen/hash/rolling_hash.hpp
---

# ローリングハッシュ (rolling_hash.hpp)

文字列の一致判定や最長共通接頭辞 (LCP) などを高速に計算するローリングハッシュです。
法として $2^{61}-1$ を使用し、ハッシュの衝突確率を極めて低く抑えています。

## rolling_hash

```cpp
template <i32 ID = 0>
class rolling_hash
```

静的文字列に対するローリングハッシュを管理するクラスです。
`ID` を変えることで、異なる基数（コンパイル時または実行時に動的に初期化される乱数）を用いるローリングハッシュをインスタンス化できます。

## コンストラクタ

```cpp
template <typename Container>
explicit rolling_hash(const Container& seq)
```

`std::string` や `std::vector` などのシーケンスからローリングハッシュを構築します。

**制約**

- `seq` の各要素が `ModInt61` に変換可能であること。

**計算量**

- $O(N)$ （$N$ はシーケンスの長さ）

## size

```cpp
i32 size() const
```

シーケンスの長さ（要素数）を返します。

**制約**

- なし

**計算量**

- $O(1)$

## get

```cpp
S get(i32 l, i32 r) const
```

区間 $[l, r)$ のハッシュ値と長さの組を取得します。

**制約**

- $0 \le l \le r \le n$

**計算量**

- $O(1)$

## rotl

```cpp
S rotl(i32 l, i32 r, i32 k) const
```

区間 $[l, r)$ を左に $k$ 文字巡回シフトしたハッシュを取得します（負の場合は右シフトとして扱われます）。

**制約**

- $0 \le l \le r \le n$

**計算量**

- $O(1)$

## rotr

```cpp
S rotr(i32 l, i32 r, i32 k) const
```

区間 $[l, r)$ を右に $k$ 文字巡回シフトしたハッシュを取得します（負の場合は左シフトとして扱われます）。

**制約**

- $0 \le l \le r \le n$

**計算量**

- $O(1)$

## equal

```cpp
bool equal(i32 l1, i32 r1, i32 l2, i32 r2) const
```

区間 $[l1, r1)$ と $[l2, r2)$ のハッシュが一致するか判定します。

**制約**

- $0 \le l1 \le r1 \le n$
- $0 \le l2 \le r2 \le n$

**計算量**

- $O(1)$

## lcp

```cpp
i32 lcp(i32 l1, i32 l2) const
```

接尾辞 $[l1, n)$ と $[l2, n)$ の最長共通接頭辞 (LCP) の長さを求めます。

**制約**

- $0 \le l1 \le n$
- $0 \le l2 \le n$

**計算量**

- 二分探索により $O(\log N)$

## rolling_hash_monoid

```cpp
template <i32 ID = 0>
struct rolling_hash_monoid
```

セグメント木等に乗せるためのローリングハッシュ用モノイドです。
要素 `S` はハッシュ値 `v` と基数のべき乗 `p` の組となります。

## op

```cpp
static S op(S a, S b)
```

2つのハッシュ値を結合します。左側のハッシュ値 `a` に、右側のハッシュ値 `b` を連結します。

**制約**

- なし

**計算量**

- $O(1)$

## e

```cpp
static S e()
```

単位元（空文字列のハッシュ）を返します。

**制約**

- なし

**計算量**

- $O(1)$

## unit

```cpp
template <typename T>
static S unit(T x)
```

1文字からハッシュ（長さ1の要素）を生成します。

**制約**

- なし

**計算量**

- $O(1)$

## range

```cpp
template <typename Iterator>
static S range(Iterator begin, Iterator end)
```

イテレータ範囲から全体のハッシュ値を計算します。

**制約**

- なし

**計算量**

- $O(N)$ （$N$ は範囲の長さ）

## 使用例

```cpp
#include <iostream>
#include <string>
#include "gwen/hash/rolling_hash.hpp"

using namespace gwen;

int main() {
    std::string s = "abracadabra";
    rolling_hash<0> rh(s);
    
    // "abra" == "abra" -> 1 (true)
    std::cout << rh.equal(0, 4, 7, 11) << "\n";
    
    // "abracadabra" と "adabra" のLCP -> 1 ("a" だけ一致)
    std::cout << rh.lcp(0, 5) << "\n";
    
    return 0;
}
```
