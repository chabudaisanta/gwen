# segtree / lazysegtree

`gwen::segtree` および `gwen::lazysegtree` は、AtCoder Library (ACL) の `atcoder::segtree` と `atcoder::lazy_segtree` のエイリアスです。独自のセグメント木実装は持たず、ACL の機能を利用します。

## ヘッダファイル

```cpp
#include "gwen/container/segtree.hpp"
```

## 概要

ACL のセグメント木は `atcoder::segtree<S, op, e>` というテンプレート引数を取りますが、本ライブラリでは `Monoid` 構造体一つを渡すことでインスタンス化できるようにラップしています。

## モノイド構造体の要件

`gwen::segtree` に渡す `Monoid` は以下のメンバを持つ必要があります（`basic_monoid.hpp` 等を参照）。

```cpp
struct Monoid {
    using S = ...;             // 要素の型
    static S op(S a, S b);     // 二項演算
    static S e();              // 単位元
};
```

`gwen::lazysegtree` に渡す `ActedMonoid` は以下のメンバを持つ必要があります。

```cpp
struct ActedMonoid {
    using S = ...;             // 要素の型
    static S op(S a, S b);     // 要素間の二項演算
    static S e();              // 要素の単位元
    
    using F = ...;             // 作用素の型
    static S mapping(F f, S x); // 作用素 f を要素 x に作用
    static F composition(F f, F g); // 作用素の合成 (f(g(x)))
    static F id();             // 作用素の恒等写像
};
```

## 使い方

```cpp
#include "gwen/container/segtree.hpp"
#include "gwen/algebra/basic_monoid.hpp"

// N要素の Range Sum Query (0初期化)
gwen::segtree<gwen::sum_monoid<long long>> seg(N);

// 値の更新 (1点更新)
seg.set(p, x);

// 区間取得 [l, r)
long long sum = seg.prod(l, r);
```

遅延セグ木についても同様に `gwen::lazysegtree<ActedMonoid>` として宣言し、ACL の `apply` 等のメソッドをそのまま使用できます。
