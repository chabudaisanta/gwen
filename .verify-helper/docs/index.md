---
layout: default
title: gwen Library Documentation
---

# gwen C++ Library

Chaby(https://x.com/chbdi)の競技プログラミング用 C++23 ライブラリのドキュメントサイトです。
AtCoder(https://atcoder.jp/users/chaby)

## ドキュメント一覧

### 統合テンプレート
- [統合テンプレート (template.hpp)](doc/template.md)

### 基本機能 (Core)
- [マクロ (macro.hpp)](doc/core/macro.md)
- [定数 (constants.hpp)](doc/core/constants.md)
- [グリッド探索 (grid.hpp)](doc/core/grid.md)
- [ユーティリティ (utils.hpp)](doc/core/utils.md)
- [ビット操作 (bit.hpp)](doc/core/bit.md)
- [型エイリアス (types.hpp)](include/gwen/types.hpp)
- [エラー出力マクロ(dump.hpp)](include/gwen/dump.hpp)
- [時間計測・制限時間管理 (timer.hpp)](doc/utils/timer.md)
- [XorShift乱数 (xorshift.hpp)](doc/utils/xorshift.md)
- [座標圧縮 (compress.hpp)](doc/utils/compress.md)
- [ノードプール (node_pool.hpp)](doc/utils/node_pool.md)

### 数学 (Math)
- [素数・素因数分解 (prime.hpp)](doc/math/prime.md)
- [順列・組合せ (combination.hpp)](doc/math/combination.md)
- [整数演算 (integer.hpp)](doc/math/integer.md)
- [基本数学 (basic.hpp)](doc/math/basic.md)

### 剰余演算 (Mod)
- [モジュラ演算基盤 (mod.hpp)](doc/mod/mod.md)
- [Dynamic ModInt (modint.hpp)](doc/mod/modint.md)
- [ModInt61 (mod61.hpp)](doc/mod/mod61.md)

### 入出力 (IO)
- [std::vector入出力 (vector.hpp)](doc/io/vector.md)
- [Yes/No 出力 (yesno.hpp)](doc/io/yesno.md)

### アルゴリズム (Algo)
- [インデックスソート (idxsort.hpp)](doc/algo/idxsort.md)
- [ランレングス圧縮 (runlength.hpp)](doc/algo/runlength.md)

### ハッシュ (Hash)
- [ローリングハッシュ (rolling_hash.hpp)](doc/hash/rolling_hash.md)

### 代数構造 (Algebra)
- [モノイド (monoid.hpp)](include/gwen/alge/monoid.hpp)
- [アーベル群 (abel.hpp)](include/gwen/alge/abel.hpp)
- [環 (ring.hpp)](doc/alge/ring.md)
- [作用付きモノイド (acted_monoid.hpp)](doc/alge/acted_monoid.md)
- [区間アフィン変換・区間和モノイド (range_affine_range_sum_monoid.hpp)](doc/alge/range_affine_range_sum_monoid.md)

### 平衡二分探索木 (BBST)
- [ImplicitTreap (implicit_treap.hpp)](doc/bbst/implicit_treap.md)
- [ProdImplicitTreap (prod_implicit_treap.hpp)](doc/bbst/prod_implicit_treap.md)
- [LazyImplicitTreap (lazy_implicit_treap.hpp)](doc/bbst/lazy_implicit_treap.md)
- [SortedTreap (sorted_treap.hpp)](doc/bbst/sorted_treap.md)
- [ProdSortedTreap (prod_sorted_treap.hpp)](doc/bbst/prod_sorted_treap.md)
- [LazySortedTreap (lazy_sorted_treap.hpp)](doc/bbst/lazy_sorted_treap.md)

### データ構造 (Data Structures)
- [累積和 (prefix_sum.hpp)](doc/ds/prefix_sum.md)
- [Fenwick Tree (fenwick_tree.hpp)](include/gwen/ds/fenwick_tree.hpp)
- [Segment Tree (segment_tree.hpp)](doc/ds/segment_tree.md)
- [Lazy Segment Tree (lazy_segment_tree.hpp)](doc/ds/lazy_segment_tree.md)
- [Dynamic Segment Tree (dynamic_segment_tree.hpp)](doc/ds/dynamic_segment_tree.md)
- [Lazy Dynamic Segment Tree (lazy_dynamic_segment_tree.hpp)](doc/ds/lazy_dynamic_segment_tree.md)
- [Wavelet Matrix (wavelet_matrix.hpp)](doc/ds/wavelet_matrix.md)

### グラフ (Graph)
- [グラフ基盤 (graph.hpp)](doc/graph/graph.md)
- [CSR形式 (csr.hpp)](doc/graph/csr.md)
- [エッジ (edge.hpp)](doc/graph/edge.md)
- [単一始点最短経路 (dijkstra.hpp)](doc/graph/dijkstra.md)
- [HL分解 (hld.hpp)](doc/graph/hld.md)
- [重み付きUnion-Find (weighted_dsu.hpp)](doc/graph/weighted_dsu.md)
- [森のLCA・ダブリング (forest.hpp)](doc/graph/forest.md)
- [木のLCA・ダブリング (tree_lca.hpp)](doc/graph/tree_lca.md)

### クエリ (Query)
- [Range Add Update Sum (range_add_update_sum.hpp)](doc/query/range_add_update_sum.md)

### 開発用資料
- [リポジトリ全体README(README.md)](README.md)
- [開発手順書 (DEVELOPMENT.md)](doc/DEVELOPMENT.md)
- [CI/CDとドキュメントの仕組み (COMPETITIVE_VERIFIER.md)](doc/COMPETITIVE_VERIFIER.md)
- [コーディング規則およびドキュメント作成規則(RULES.md)](doc/RULES.md)
- [testlib.hの使用方法(test/third_party/README.md)](test/third_party/README.md)
- [テスト用ユーティリティの使用方法(test/utils/README.md)](test/utils/README.md)
- [スクリプト使用方法(scripts/README.md)](scripts/README.md)

---
*GitHubリポジトリ: [chabudaisanta/gwen](https://github.com/chabudaisanta/gwen)*
