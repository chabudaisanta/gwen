# competitive-verifier 導入時のノウハウ・トラブルシューティングまとめ

当プロジェクト（`gwen`）において、競技プログラミング用ライブラリの自動テストおよびドキュメント生成ツールである `competitive-verifier` を導入した際の手順と、発生した様々なエラーの解決策のまとめです。

## 1. ワークフロー構成 (`.github/workflows/verify.yml`)

公式の推奨手順に従い、以下の3つのジョブに分割して構成しました。

### ① `setup` ジョブ（依存関係の解決）
- `competitive-verifier/actions/oj-resolve@v2` を使って、リポジトリ内のコードを解析し `verify_files.json` と展開済みのコード（`.competitive-verifier/bundled`）を生成します。
- GTest を利用している `test/` ディレクトリ配下は、`oj-resolve` がマクロ展開に失敗するため `exclude: test/**` で除外しました。
- 次のジョブへデータを渡すため、`upload-verify-artifact` で `verify_files.json` をアップロードし、通常の `upload-artifact` で `.competitive-verifier/bundled` もアップロードします。

### ② `verify` ジョブ（テストの実行）
- `setup` ジョブで生成された `verify_files.json` をダウンロードし、`competitive-verifier/actions/verify@v2` を実行します。
- **重要**: デフォルトでは結果ファイルが出力されないため、`destination: verify-result.json` を明示的に指定して出力させ、それをアップロードします。

### ③ `docs` ジョブ（ドキュメント生成と GitHub Pages へのデプロイ）
- 必要なファイル（`verify-result.json`, `verify_files.json`, `.competitive-verifier/bundled`）をすべてダウンロードします。
- `competitive-verifier/actions/docs@v2` を実行し、Markdown ファイル群を生成します。
  - **重要**: 後述の Jekyll のパス問題を防ぐため、`destination: _jekyll` を指定して生成先を `_jekyll` ディレクトリにします。
- `actions/jekyll-build-pages@v1` で `_jekyll` の内容を元に HTML (`_site` ディレクトリ) をビルドします。
- 最後に `actions/upload-pages-artifact@v3` で `_site` をアップロードし、`deploy-pages@v4` で公開します。

---

## 2. コンパイラ・インクルードパスの設定 (`config.toml`)

C++23 環境と GTest、および外部ライブラリ（AtCoder Library）を使うために、`config.toml` に設定を記述しました。

```toml
[languages.cpp]

[[languages.cpp.environments]]
CXX = "g++-15"
CXXFLAGS = ["-I", "include", "-I", "references/nyaan", "-std=gnu++23", "-O2", "-mtune=native", "-march=native", "-fconstexpr-depth=1024", "-fconstexpr-loop-limit=524288", "-fconstexpr-ops-limit=2097152"]
```

---

## 3. 発生した問題と解決策（トラブルシューティング）

### 1. `oj-resolve` が GTest の内部コードでクラッシュする
- **原因**: `gtest.h` を読み込んでいるテストファイルにおいて、コンパイラ固有の組み込みマクロを解決できず `oj-resolve` が失敗しました。
- **解決策**: `verify.yml` の `oj-resolve` ステップで `exclude: test/**` を指定し、テストディレクトリを検証の依存関係ツリー解決から除外しました。

### 2. AtCoder Library のヘッダーが見つからない
- **原因**: 通常の ACL は拡張子なし (`#include <atcoder/lazysegtree>`) ですが、プロジェクト内でサブモジュールとして利用している Nyaan's Library 同梱版の ACL は `.hpp` 拡張子がついていました。
- **解決策**: 利用側のコードで `#include <atcoder/lazysegtree.hpp>` のように `.hpp` を明示的につけるように修正しました。

### 3. `FileNotFoundError` (bundled フォルダが二重になる問題)
- **原因**: 過去に `.competitive-verifier` フォルダが誤って Git の管理下（Tracked）に追加されてしまっていました。そのため `oj-resolve` が展開済みのソースをさらに展開し、`.competitive-verifier/bundled/.competitive-verifier/bundled/...` という異常なパスが生成されました。
- **解決策**: `git rm -r --cached .competitive-verifier` を実行し、`.gitignore` に `.competitive-verifier/` を追加しました。

### 4. `docs` ジョブで `verify_files.json` が見つからない
- **原因**: GitHub Actions はジョブごとに環境がリセットされるため、`setup` ジョブで生成されたファイルを `docs` ジョブにも明示的に渡す必要がありました。
- **解決策**: `docs` ジョブ内で `competitive-verifier/actions/download-verify-artifact@v2` を追加し、さらに `bundled` ディレクトリも `actions/download-artifact` でダウンロードするようにしました。

### 5. Jekyll ビルドで `No such file or directory @ dir_chdir0` エラー
- **原因**: `actions/jekyll-build-pages@v1` は Docker コンテナ上で動きますが、ソースディレクトリが `.competitive-verifier/docs` のように ドット(`.`)で始まる隠しフォルダ だと、正しくマウント/認識されずエラーになりました。
- **解決策**: `competitive-verifier docs` の出力先を `_jekyll` に変更し、Jekyll のソースディレクトリも `_jekyll` としました。

### 6. Liquid Exception: Liquid syntax error (C++の `{{` 問題)
- **原因**: Markdown 内の C++ コードで `vector<edge> es = {{0, 1}};` のように `{` を二重に書くと、Jekyll (Liquid テンプレート) が変数埋め込みタグだと勘違いして構文エラーを起こします。
- **解決策**: C++の文法として問題ないように、波括弧の間に半角スペースを入れて `{ {0, 1} }` のように記述するか、または `{% raw %}` `{% endraw %}` で囲んでエスケープするようにしました。
