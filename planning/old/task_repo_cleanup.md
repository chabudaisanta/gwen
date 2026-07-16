# タスク仕様書: リポジトリのクリーンアップおよび規約・ドキュメントの不整合是正

本タスクでは、プロジェクトのルール設定、手順書、およびディレクトリ構成に存在する不整合やクリーンアップ漏れ（P1〜P8）を一括で修正します。

## 修正内容一覧

### 1. リポジトリのクリーンアップ
- **a.out の除外**:
  - `git rm --cached a.out` を実行し、Gitの追跡対象から除外する。
  - `.gitignore` に `.build/` などと並べて `a.out` を追記する。

### 2. ルール・手順書の表記修正
- **AGENTS.md**:
  - `## AIエージェントの作業ワークフロー` の「以下の4ステップを必ず守ってください。」を「以下のステップを必ず守ってください。」または「以下の5ステップを必ず守ってください。」に修正する。
- **DEVELOPMENT.md**:
  - `1. ヘッダーファイルの実装` 内の `include/gwen/concept/` という記述を、実態に合わせて `include/gwen/alge/` や `include/gwen/graph/` 等の適切なフォルダを示す説明に修正する。
- **RULES.md**:
  - サンプルの Frontmatter に記載されている `documentation_of: //include/gwen/algebra/fenwick_tree.hpp` を、正しいパス `//include/gwen/ds/fenwick_tree.hpp` に変更する。
  - 定数の命名規則「定数名: 大文字のスネークケース」に対して、「ただし、`mod998` や `iINF` などの競プロの慣習に基づく小文字混在の定数は例外として許容する」という旨を追記する。
- **SKILL.md** (`.agents/skills/port_module/SKILL.md`):
  - ワークフローの最後に「5. **Cleanup** (作業完了後、`planning/` に置いた作業ファイルを `planning/old/` へ移動する)」を追記し、AGENTS.md の記述と同期させる。

### 3. ディレクトリ構造の整合 (Trieの整理)
- **Trie の移動**:
  - `include/gwen/container/trie.hpp` を `include/gwen/ds/trie.hpp` に移動する。
  - `doc/container/trie.md` を `doc/ds/trie.md` に移動し、内部の `documentation_of` のパスを `//include/gwen/ds/trie.hpp` に修正する。
  - 移動後、空になった `include/gwen/container/` および `doc/container/` ディレクトリを削除する。
- **index.md の更新**:
  - `.verify-helper/docs/index.md` の Trie木へのリンクを `doc/container/trie.md` から `doc/ds/trie.md` に書き換える。

## 検証プラン
- Trieの移動に伴い、ユニットテスト等のインクルードパスやビルドが壊れていないか確認する。
- `cmake -S . -B .build` の再構成
- `cmake --build .build`
- `ctest --test-dir .build`
