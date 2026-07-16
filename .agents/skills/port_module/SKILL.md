---
name: port_module
description: 既存モジュールを新ブランチに移植する、または新規機能を追加する際の標準ワークフローを実行する。
---

# モジュール移植・実装スキル (Port Module Skill)

このスキルは `gwen` プロジェクトでモジュールの移植や新規作成を依頼された際に自動的に発動します。
AIエージェントは必ず以下のステップに従って作業を進行してください。

## Code Reviewer サブエージェントの定義
本フローにおいて `define_subagent` を用いて定義する `code_reviewer` には、以下の厳格なシステムプロンプトを設定してください。

**System Prompt**:
> You are a strict Code Reviewer for the gwen C++ library project. Your job is to review specifications, code, and documentation against the project rules.
> Key Rules:
> 1. All classes/structs must use PascalCase (e.g., `FenwickTree`).
> 2. All methods/variables must use snake_case (e.g., `add`).
> 3. Doxygen comments (`@brief`, `@tparam`, etc.) must be provided for all public APIs.
> 4. C++23 `concept` must be used for type constraints (e.g., `abel`, `monoid`). Do not append `_concept` to concept names (e.g., use `edge`, not `edge_concept`).
> 5. Markdown documentation must follow the AC Library style (with `## メソッド名`, `**制約**`, `**計算量**`).
> 6. Google Tests (`test/unit/`) and Verify codes (`verify/`) must be properly planned and implemented.
> Please rigorously point out any violations. If everything is perfect, explicitly approve it.

---

## ワークフロー

### 1. タスクの準備と仕様策定 (Plan)
- 対象のタスクに合わせて、メインブランチから新しい作業用Gitブランチを作成します（例: `git checkout -b feature/add-matrix`）。
- `planning/` ディレクトリにタスク用の Markdown ファイル（例: `planning/task_mod_xxx.md`）を作成します。
- クラス名、メソッド名、依存関係、C++23 Concept の適用方針などの設計を明記します。

### 2. 事前レビュー (Pre-Review)
- `code_reviewer` サブエージェントを `invoke_subagent` で呼び出します。
- **依頼内容 (Prompt)**: 「仕様書 (`task_mod_xxx.md`) を読み、クラス・メソッドの命名規則、Conceptの適用方針、テストとVerifyコード作成の計画がルール通り網羅されているかチェックしてください。違反があれば指摘し、なければ承認してください。」
- 指摘された問題があれば修正します。

### 3. ユーザー承認
- 事前レビューを通過した仕様を `implementation_plan.md` 等でユーザーに提示し、承認（Proceed）を得ます。
- **注意**: 承認を得るまでは絶対にコードの実装へ進まないでください。

### 4. 実装とテスト (Execute)
- 承認後、以下のファイルを一括で作成します。
  - **本体**: `include/gwen/` 以下の `.hpp` (Doxygenコメント必須)
  - **ユニットテスト**: `test/unit/` 以下の `.cpp` (Google Test)
  - **Verifyコード**: `verify/yosupo/` などの `.test.cpp` (`competitive-verifier` 向け)
  - **ドキュメント**: `doc/` 以下の Markdown (AC Libraryスタイル。また `index.md` へリンク追加)
- ターミナルでテスト（`cmake --build .build && ctest ...`）を実行し、自力でエラーを修正します。

### 5. 事後レビュー (Post-Review)
- すべての実装が完了しテストをパスしたら、再度 `code_reviewer` サブエージェントを呼び出します。
- **依頼内容 (Prompt)**: 「実装された `.hpp`、テストコード、Verifyコード、およびドキュメント (Markdown) をレビューしてください。命名規則、Doxygenコメント、AC Libraryスタイルの遵守、テストの完全性を厳格にチェックし、1つでも違反があれば指摘してください。完璧であれば承認してください。」
- 指摘された問題があれば修正します。

### 6. 完了報告とコミット (Finalize)
- 実装が完了したモジュールについて、`planning/TODO.md` の該当項目に完了チェック (`[x]`) を入れます。
- ユーザーに `walkthrough.md` で作業内容を報告し、問題がなければ Conventional Commits 形式でコミットします。

### 7. クリーンアップ (Cleanup)
- 作業完了後、`planning/` に置いたタスク固有の作業ファイル（`task_mod_xxx.md`など）を `planning/old/` へ移動します。
