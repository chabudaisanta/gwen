<RULE>
## 開発・ファイル管理の基本
- 本ライブラリはヘッダーオンリーです。実装は `include/gwen/` 配下の適切なディレクトリに `.hpp` で作成してください。
- グラフや代数構造などの汎用コンポーネントは C++23 `concept` を用いて厳密な型制約を設けてください。
- ビルド成果物は `.build/` へ出力し、AIエージェントが一時的なファイルを作成する場合は `temp/` ディレクトリを使用してください。
</RULE>

<RULE>
## ドキュメントとテストの必須対応
- 新規実装・移植時は、必ず「Google Testによる単体テスト (`test/unit/`)」と「Verifyコード (`verify/`)」の両方を作成し、テストをパスさせてください。
- ドキュメント (`doc/`) は必ず **AC Library スタイル**（メソッド名、制約、計算量を太字で記載する形式）で記述し、全てのドキュメントへのリンクを必ず `.verify-helper/docs/index.md` の目次に登録してください。
</RULE>

<RULE>
## コーディング規約 (Naming Conventions)
- **クラス・構造体**: データを直接扱うデータ構造（`FenwickTree` 等）は PascalCase を使用してください。ただし、実体のない代数構造（モノイドや群などの定義）は snake_case (`sum_monoid` 等) を許容します。
- **メソッド・変数・コンセプト名**: snake_case (`add`, `is_same`, `edge` 等) を使用してください (コンセプトに `_concept` などの接尾辞はつけないこと)。
- すべての公開 API (クラス、関数、Concept等) に Doxygen 形式のコメント (`@brief`, `@tparam` 等) を付与してください。
</RULE>

<RULE>
## コミットメッセージのルール
緩やかな Conventional Commits を使用してください。
- `feat:` : 新機能・新しいモジュール
- `fix:` : バグ修正
- `docs:` : ドキュメント更新
- `test:` : テスト・検証コード追加
- `chore:` : CI/CD、その他雑務
</RULE>

<RULE>
## AIエージェントの作業ワークフロー
新規モジュールの実装や移植を行う場合は、以下の4ステップを必ず守ってください。
1. **Plan & Pre-Review**: `planning/` に仕様（設計、パス、Concept）をまとめ、`code_reviewer` サブエージェントを呼んで事前レビューを受ける。
2. **User Approval**: サブエージェントの承認後、ユーザーに承認をもらう。
3. **Execute**: 実装、Google Test、Verifyコード、ドキュメント作成を行い、テストを自力でパスさせる。
4. **Post-Review & Finalize**: 再度 `code_reviewer` サブエージェントを呼んで事後レビューを受け、完了後にユーザーへ報告してコミットする。
5. **Cleanup**: 作業完了後、`planning/` に置いた作業ファイルを `planning/old/` へ移動する。
</RULE>

<RULE>
## コミュニケーション
- ユーザーへの最終的な回答、コメント、ドキュメントはすべて自然な **日本語** で行ってください。
</RULE>

<RULE>
## 型エイリアスの使用
- プリミティブな型（`int`, `long long`, `size_t` 等）は極力直接使用せず、`gwen/types.hpp` で定義されている型（`i32`, `i64`, `usize` 等）をインクルードして使用してください。
</RULE>
