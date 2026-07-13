# TODO (rebuild-v2)

gwen ライブラリの再構築・移行タスクの管理ファイル。

## フェーズ 1: 骨組み（Skeleton）の構築
- [x] 新ブランチ `rebuild-v2` の作成
- [x] `README.md` の新規作成（概要、構成、要件）
- [x] `GEMINI.md` の更新（テスト・verifyの徹底、形式的管理の明記、Concept利用方針）
- [ ] テスト環境 (Google Test + CMake) の構築
  - [ ] `CMakeLists.txt` の設定
  - [ ] `test/unit/` 雛形の作成
- [ ] `competitive-verifier` の設定
  - [ ] GitHub Actions Workflow 構築
  - [ ] `.verify-helper/docs/_config.yml` などの設定ファイル作成

## フェーズ 2: 共通基盤・設計ルールの策定
- [ ] `types.hpp`, `io.hpp` 等のコア機能の再整備
- [ ] C++20 `concept` 定義 (Monoid, Graph等)
  - `include/gwen/concept/` などの共通ディレクトリにまとめるか検討

## フェーズ 3: モジュールごとの移植とリファクタリング
（以下は移植時に詳細なタスクを書き出していく）

### algebra
- [ ] 未着手

### container
- [ ] 未着手

### graph
- [ ] APIの統一（基底クラス、concept）
- [ ] 各種アルゴリズム移植

### math
- [ ] 未着手

### mod
- [ ] 未着手

### misc
- [ ] 未着手
