# TODO (rebuild-v2)

gwen ライブラリの再構築・移行タスクの管理ファイル。

## フェーズ 1: 骨組み（Skeleton）の構築
- [x] 新ブランチ `rebuild-v2` の作成
- [x] `README.md` の新規作成（概要、構成、要件）
- [x] `GEMINI.md` の更新（テスト・verifyの徹底、形式的管理の明記、Concept利用方針）
- [x] テスト環境 (Google Test + CMake) の構築
  - [x] `CMakeLists.txt` の設定
  - [x] `test/unit/` 雛形の作成
- [x] `competitive-verifier` の設定
  - [x] GitHub Actions Workflow 構築
  - [x] `.verify-helper/docs/_config.yml` などの設定ファイル作成

## フェーズ 2: 共通基盤・設計ルールの策定
- [x] `io.hpp` 等のコア機能の再整備 (`types.hpp`, `io/vector.hpp`, `io/yesno.hpp` 作成済)
- [x] C++23 `concept` 定義 (Monoid, Graph等)
  - `alge/` や `graph/` 配下に直接配置する方針で確定

## フェーズ 2.5: `my_template.hpp` の移植と整理
- [x] `core` モジュールの移植 (マクロ、定数、グリッド、基本ユーティリティ、ビット操作)
- [x] `math` モジュールの移植 (整数演算、基本数学系)
- [x] `io` モジュールの移植 (vector入出力、Yes/No出力)
- [x] `algo` / `ds` モジュールの移植 (idxsort, runlength, 累積和)
- [x] 一括インクルード用ヘッダ `template.hpp` の作成

## フェーズ 3: 主要モジュールの移植とリファクタリング (完了)
- [x] **代数構造 (Algebra)**
  - `monoid`, `acted_monoid`, `ring`, 各種アフィン変換モノイドなど
- **グラフ (Graph)** (Verifyは未完了だが実装と単体テストは完了)
  - [x] `graph.hpp`, `edge.hpp`
  - [x] `csr.hpp`
  - [x] `dijkstra.hpp`
  - [x] `hld.hpp`
- **平衡二分探索木 (BBST)** (Verifyは未完了だが実装と単体テストは完了)
  - [x] `implicit_treap` (遅延・積付き含む)
  - [x] `sorted_treap` (遅延・積付き含む)
- [x] **セグメント木 (Segment Tree)** (Verifyまで完全完了)
  - `segment_tree.hpp`, `lazy_segment_tree.hpp`
  - `dynamic_segment_tree.hpp`, `lazy_dynamic_segment_tree.hpp`

## フェーズ 4: 新規モジュールの移植と未完了タスクの消化 (現在進行中)
- [x] Phase 4.1: 高頻度ラッパー & モノイド (`rolling_hash_monoid.hpp`, `range_add_update_sum.hpp`)
- [x] Phase 4.2: データ構造 & グラフアルゴリズム (済: `bit_vector.hpp`, `wavelet_matrix.hpp`, `weighted_dsu.hpp`, `forest.hpp`, `tree_lca.hpp`)
- [ ] Phase 4.3: 数学 & アルゴリズム (`matrix.hpp`, `lis.hpp`)
- [ ] Phase 4.4: クオリティ改善リライト (`xor_basis.hpp`, `zobrist.hpp` など)
- [ ] Graph, BBST, Math, Hash 等の `competitive-verifier` 向け Verify 追加実装

※ 詳細な移植対象は `planning/transplant_from_master.md` を参照。
