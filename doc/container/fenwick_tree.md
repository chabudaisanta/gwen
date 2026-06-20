# fenwick_tree

Fenwick Tree (Binary Indexed Tree, BIT) を提供します。1点加算と区間和を $O(\log N)$ で処理するデータ構造です。

## ヘッダファイル

```cpp
#include "gwen/container/fenwick_tree.hpp"
```

## 概要

要素の型と演算を定義する `Abel` (アーベル群) 構造体をテンプレート引数として取ります。

## Abel 構造体の要件

`Abel` は以下のメンバを持つ必要があります（交換法則を満たす必要があります）。

```cpp
struct Abel {
    using S = ...;             // 要素の型
    static S op(S a, S b);     // 二項演算 (加算など)
    static S e();              // 単位元 (0など)
    static S inv(S a);         // 逆元 (減算用: a の逆元を返す)
};
```

## コンストラクタ

```cpp
fenwick_tree()
explicit fenwick_tree(i32 n)
explicit fenwick_tree(const std::vector<S>& vec)
```
- `n`: 要素数。初期値は `Abel::e()` で埋められます。
- `vec`: 初期配列。$O(N)$ で構築されます。

## メンバ関数

### `void add(i32 p, S x)`
- 位置 `p` (0-indexed) の要素に `x` を加算(`op`)します。
- **計算量**: $O(\log N)$

### `S sum(i32 r)`
- 区間 `[0, r)` の和を返します。`r = 0` の場合は単位元が返ります。
- **計算量**: $O(\log N)$

### `S sum(i32 l, i32 r)`
- 区間 `[l, r)` の和を返します。内部で `op(inv(sum(l)), sum(r))` を計算します。
- **計算量**: $O(\log N)$

### `i32 max_right(auto&& f) const`
### `i32 max_right(i32 l, auto&& f) const`
- 与えられた関数 `f` が `true` となる最大の `r` を返します。
- `f(sum(l, r))` が `true` を満たすような最大の `r` を二分探索で求めます（`f(Abel::e())` は `true` である必要があります）。
- **計算量**: $O(\log N)$
