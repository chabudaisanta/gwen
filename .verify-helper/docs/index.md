---
layout: default
title: gwen Library Documentation
---

# gwen C++ Library

Chaby(https://x.com/chbdi)の競技プログラミング用 C++23 ライブラリのドキュメントサイトです。
AtCoder(https://atcoder.jp/users/chaby)

## ドキュメント一覧

### 統合テンプレート
- [統合テンプレート (template.hpp)](include/gwen/template.hpp)

### 基本機能 (Core)
- [マクロ (macro.hpp)](include/gwen/core/macro.hpp)
- [定数 (constants.hpp)](include/gwen/core/constants.hpp)
- [グリッド探索 (grid.hpp)](include/gwen/core/grid.hpp)
- [ユーティリティ (utils.hpp)](include/gwen/core/utils.hpp)
- [ビット操作 (bit.hpp)](include/gwen/core/bit.hpp)
- [型エイリアス (types.hpp)](include/gwen/types.hpp)
- [エラー出力マクロ(dump.hpp)](include/gwen/dump.hpp)
- [時間計測・制限時間管理 (timer.hpp)](include/gwen/utils/timer.hpp)
- [XorShift乱数 (xorshift.hpp)](include/gwen/utils/xorshift.hpp)
- [座標圧縮 (compress.hpp)](include/gwen/utils/compress.hpp)
- [ノードプール (node_pool.hpp)](include/gwen/utils/node_pool.hpp)

### 数学 (Math)
- [素数・素因数分解 (prime.hpp)](include/gwen/math/prime.hpp)
- [順列・組合せ (combination.hpp)](include/gwen/math/combination.hpp)
- [行列 (matrix.hpp)](include/gwen/math/matrix.hpp)
- [XOR基底 (xor_basis.hpp)](include/gwen/math/xor_basis.hpp)
- [整数演算 (integer.hpp)](include/gwen/math/integer.hpp)
- [基本数学 (basic.hpp)](include/gwen/math/basic.hpp)

### 剰余演算 (Mod)
- [モジュラ演算基盤 (mod.hpp)](include/gwen/mod/mod.hpp)
- [ModInt (modint.hpp)](include/gwen/mod/modint.hpp)
- [ModInt61 (mod61.hpp)](include/gwen/mod/mod61.hpp)

### 入出力 (IO)
- [std::vector入出力 (vector.hpp)](include/gwen/io/vector.hpp)
- [Yes/No 出力 (yesno.hpp)](include/gwen/io/yesno.hpp)

### アルゴリズム (Algo)
- [インデックスソート (idxsort.hpp)](include/gwen/algo/idxsort.hpp)
- [ランレングス圧縮 (runlength.hpp)](include/gwen/algo/runlength.hpp)
- [カウンティングソート (counting_sort.hpp)](include/gwen/algo/counting_sort.hpp)
- [最長増加部分列 (lis.hpp)](include/gwen/algo/lis.hpp)

### ハッシュ (Hash)
- [ローリングハッシュ (rolling_hash.hpp)](include/gwen/hash/rolling_hash.hpp)

### 代数構造 (Algebra)
- [モノイド (monoid.hpp)](include/gwen/alge/monoid.hpp)
- [アーベル群 (abel.hpp)](include/gwen/alge/abel.hpp)
- [環 (ring.hpp)](include/gwen/alge/ring.hpp)
- [作用付きモノイド (acted_monoid.hpp)](include/gwen/alge/acted_monoid.hpp)
- [区間アフィン変換・区間和モノイド (range_affine_range_sum_monoid.hpp)](include/gwen/alge/range_affine_range_sum_monoid.hpp)

### 平衡二分探索木 (BBST)
- [ImplicitTreap (implicit_treap.hpp)](include/gwen/bbst/implicit_treap.hpp)
- [ProdImplicitTreap (prod_implicit_treap.hpp)](include/gwen/bbst/prod_implicit_treap.hpp)
- [LazyImplicitTreap (lazy_implicit_treap.hpp)](include/gwen/bbst/lazy_implicit_treap.hpp)
- [SortedTreap (sorted_treap.hpp)](include/gwen/bbst/sorted_treap.hpp)
- [ProdSortedTreap (prod_sorted_treap.hpp)](include/gwen/bbst/prod_sorted_treap.hpp)
- [LazySortedTreap (lazy_sorted_treap.hpp)](include/gwen/bbst/lazy_sorted_treap.hpp)

### データ構造 (Data Structures)
- [累積和 (prefix_sum.hpp)](include/gwen/ds/prefix_sum.hpp)
- [Fenwick Tree (fenwick_tree.hpp)](include/gwen/ds/fenwick_tree.hpp)
- [Segment Tree (segment_tree.hpp)](include/gwen/ds/segment_tree.hpp)
- [Lazy Segment Tree (lazy_segment_tree.hpp)](include/gwen/ds/lazy_segment_tree.hpp)
- [Dynamic Segment Tree (dynamic_segment_tree.hpp)](include/gwen/ds/dynamic_segment_tree.hpp)
- [Lazy Dynamic Segment Tree (lazy_dynamic_segment_tree.hpp)](include/gwen/ds/lazy_dynamic_segment_tree.hpp)
- [Wavelet Matrix (wavelet_matrix.hpp)](include/gwen/ds/wavelet_matrix.hpp)
- [Trie木 (trie.hpp)](include/gwen/ds/trie.hpp)
- [FastSet (fast_set.hpp)](include/gwen/ds/fast_set.hpp)

### グラフ (Graph)
- [グラフ基盤 (graph.hpp)](include/gwen/graph/graph.hpp)
- [CSR形式 (csr.hpp)](include/gwen/graph/csr.hpp)
- [エッジ (edge.hpp)](include/gwen/graph/edge.hpp)
- [単一始点最短経路 (dijkstra.hpp)](include/gwen/graph/dijkstra.hpp)
- [HL分解 (hld.hpp)](include/gwen/graph/hld.hpp)
- [重み付きUnion-Find (weighted_dsu.hpp)](include/gwen/graph/weighted_dsu.hpp)
- [森のLCA・ダブリング (forest.hpp)](include/gwen/graph/forest.hpp)
- [木のLCA・ダブリング (tree_lca.hpp)](include/gwen/graph/tree_lca.hpp)

### クエリ (Query)
- [Range Add Update Sum (range_add_update_sum.hpp)](include/gwen/query/range_add_update_sum.hpp)
- [Mo's Algorithm (mo.hpp)](include/gwen/query/mo.hpp)
- [SWAG (swag.hpp)](include/gwen/query/swag.hpp)

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
