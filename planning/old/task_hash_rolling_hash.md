# `hash/rolling_hash.hpp` 移植タスク仕様案

## 1. 概要
文字列の一致判定や最長共通接頭辞 (LCP) を $O(1) \sim O(\log N)$ で処理するローリングハッシュを移植します。
このタスクでは、依存先となっている乱数生成器 (`xorshift.hpp`) も同時に移植します。

## 2. 移植対象と構成
以下の2つのファイルを移植・再設計します。
1. `include/gwen/utils/xorshift.hpp`
2. `include/gwen/hash/rolling_hash.hpp`

※ 元の `master` ブランチでは `rolling_hash_monoid` が `algebra/` ディレクトリに置かれていましたが、用途がローリングハッシュに特化しているため、今回は単一の `hash/rolling_hash.hpp` の内部に `monoid` も統合するか、もしくは `hash/` ディレクトリ内に配置することを提案します。

## 3. 各モジュールの設計方針

### 3.1. `utils/xorshift.hpp`
- 元の実装では `static internal::xorshift rng;` を用いておりスレッドセーフではありません。
- 競技プログラミングにおいて乱数のスレッドセーフティが要求される場面は極めて稀であるため、元の使い勝手を維持しつつ、Doxygen コメントに **スレッドセーフではない旨を明記** して移植します。

### 3.2. `hash/rolling_hash.hpp` (および Monoid)
- 既に移植済みの `ModInt61` (`include/gwen/mod/mod61.hpp`) を活用し、モジュラ演算を `ModInt61` のメソッドに置き換えることで、コードを極めて簡潔かつ安全にします。
- **基数 (Base) の生成**: `template <i32 ID = 0>` を用いて、ID ごとに異なる基数 $r$ をコンパイル時（初回の実行時初期化）に `xorshift` を使ってランダム生成する仕組みはそのまま踏襲します。（これにより、ハッシュ衝突を意図的に狙うハックへの耐性を高めます）
- **モノイドの統合**: セグメント木などにも乗せられる `rolling_hash_monoid` 構造体は、非常に強力な機能であるため引き続き提供しますが、ファイル構造をシンプルにするため `hash/rolling_hash.hpp` の中に統合（または同じ `hash/` ディレクトリ内に配置）します。

## 4. クラス・関数のシグネチャ案

```cpp
// utils/xorshift.hpp
namespace gwen {
    u64 rand64();
    u32 rand32();
    u32 rand32(u32 l, u32 r);
}
```

```cpp
// hash/rolling_hash.hpp
namespace gwen {
namespace rhash {
    template <i32 ID = 0> struct rolling_hash_monoid {
        struct S { u64 v, p; /* ... */ };
        static S op(S a, S b);
        static S e();
        // ...
    };
}

template <i32 ID = 0> struct rolling_hash {
    explicit rolling_hash(const auto& seq);
    rhash::rolling_hash_monoid<ID>::S get(i32 l, i32 r) const;
    bool equal(i32 l1, i32 r1, i32 l2, i32 r2) const;
    i32 lcp(i32 l1, i32 l2) const;
};
}
```

## 5. 計算量
- `rolling_hash` の初期化: $O(N)$
- `get`, `equal`: $O(1)$
- `lcp`: 二分探索により $O(\log N)$

## 6. レビュー依頼事項（Subagent向け）
- モノイドを `rolling_hash.hpp` 内に統合する設計の妥当性。
- `ModInt61` への完全な置き換え（`add_mod`, `mul_mod` の廃止）によるパフォーマンスや設計上の懸念点の有無。
- `xorshift` のスレッドセーフティに対する割り切り（注記のみでの対応）の是非。
