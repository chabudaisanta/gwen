---
title: Mo's Algorithm (Mo法)
documentation_of: //include/gwen/query/mo.hpp
---

## 概要

オフラインの区間クエリを処理する Mo's Algorithm (Mo法) を実装したクラスです。平方分割とヒルベルト曲線を組み合わせており、配列の長さ $N$、クエリ数 $Q$ としたとき、$O((N+Q)\sqrt{N})$ で各クエリの答えを求めることができます。

## コンストラクタ

```cpp
Mo(i32 n)
```

対象となる配列の要素数 $n$ で初期化します。

**制約**

- $0 \le n \le 10^8$

**計算量**

- $O(1)$

## メソッド

### add_query

```cpp
void add_query(i32 l, i32 r)
```

クエリとして区間 $[l, r)$ を追加します。

**制約**

- $0 \le l \le r \le n$

**計算量**

- $O(1)$

---

### solve (左右共通の処理の場合)

```cpp
template <
    std::invocable<i32> Add,
    std::invocable<i32> Del,
    std::invocable<i32> Calc>
auto solve(Add add, Del del, Calc calc)
```

追加されたすべてのクエリを処理し、結果の配列を返します。
区間の拡張時と縮小時に左右で共通の処理を行える場合（例：区間和や区間内の要素の出現回数など）はこちらのオーバーロードが便利です。

- `add(idx)` : インデックス `idx` の要素を区間に追加する処理を記述します。
- `del(idx)` : インデックス `idx` の要素を区間から削除する処理を記述します。
- `calc(query_id)` : 現在の区間の状態をもとに、`query_id` 番目のクエリの答えを返す処理を記述します。

**制約**

- コールバック関数 `Add`, `Del`, `Calc` が `std::invocable` の制約を満たすこと。

**計算量**

- $O((N+Q)\sqrt{N} \times (\text{コールバック関数の計算量}))$

---

### solve (左右で異なる処理の場合)

```cpp
template <
    std::invocable<i32, i32> IL,
    std::invocable<i32, i32> DL,
    std::invocable<i32, i32> IR,
    std::invocable<i32, i32> DR,
    std::invocable<i32> GR>
auto solve(IL increment_l, DL decrement_l, IR increment_r, DR decrement_r, GR get_res)
```

区間の拡張・縮小方向（左端か右端か）によって処理が異なる場合に使用する汎用バージョンです。
（例：区間転倒数のように、左に追加するか右に追加するかで処理が変わる場合）

- `increment_l(l, r)` : 左端 `l` をインクリメント (区間を縮小) する際に呼ばれる関数。**縮小前の区間** $[l, r)$ が渡されます。
- `decrement_l(l, r)` : 左端 `l` をデクリメント (区間を拡張) する際に呼ばれる関数。**拡張後の区間** $[l, r)$ が渡されます。
- `increment_r(l, r)` : 右端 `r` をインクリメント (区間を拡張) する際に呼ばれる関数。**拡張前の区間** $[l, r)$ が渡されます。
- `decrement_r(l, r)` : 右端 `r` をデクリメント (区間を縮小) する際に呼ばれる関数。**縮小後の区間** $[l, r)$ が渡されます。
- `get_res(query_id)` : 現在の区間の状態をもとに、`query_id` 番目のクエリの答えを返す処理を記述します。

**制約**

- コールバック関数群がすべて適切に定義されていること。

**計算量**

- $O((N+Q)\sqrt{N} \times (\text{コールバック関数の計算量}))$
