---
layout: default
title: CI/CD とドキュメント生成の仕組み
---

# CI/CD とドキュメント生成の仕組み
本プロジェクト（gwen）では、`competitive-verifier` を用いて、テストの自動検証と GitHub Pages へのドキュメント自動デプロイを行っています。ここでは各設定ファイルとその紐付きについて解説します。

## 1. 全体フロー

GitHub にソースコードを Push すると、GitHub Actions によって以下のフローが自動実行されます。

1. **`oj-resolve`**: `#include` などの依存関係を解析し、どのファイルが検証対象かを `verify_files.json` にリストアップします。（※この際、Google Test等の環境に依存する `test/` フォルダは意図的に除外しています）
2. **`verify`**: 実際にテストコード（`verify/` 配下の提出用コードなど）をコンパイル・実行し、成功/失敗の結果を `result.json` に出力します。
3. **`docs`**: ソースファイル、`verify_files.json`、`result.json`、および `doc/` 等の Markdown ドキュメントを結合し、Jekyll（静的サイトジェネレータ）用のサイトデータを `.competitive-verifier/_jekyll` フォルダに自動生成します。
4. **`actions-gh-pages`**: 生成された `_jekyll` フォルダの中身を `gh-pages` という専用ブランチにプッシュし、GitHub Pages として公開します。

---

## 2. 重要な設定ファイルと役割

### `config.toml` (リポジトリ直下)
- `competitive-verifier` 本体の実行環境設定ファイルです。
- どのコンパイラ（`g++`）を使うか、コンパイルオプション（`CXXFLAGS` = `-std=gnu++23` など）はどうするかを定義しています。
- これを読み込ませないと、最新の C++23 構文をパーサーが理解できずに依存関係解析（`oj-resolve`）でエラーになることがあります。

### `.verify-helper/docs/_config.yml`
- GitHub Pages（Jekyll）の見た目や挙動を決める設定ファイルです。
- MathJax（数式表示）の有効化や、サイトのタイトル、デザインテーマ（`jekyll-theme-cayman`）などを指定しています。

### `.verify-helper/docs/index.md`
- ドキュメントサイト（GitHub Pages）の **トップページ** になるファイルです。
- これが存在しない場合、リポジトリ直下の `README.md` がそのままトップページになってしまいます。
- 新しいモジュールやドキュメント（例: `doc/io.md`）を作った際は、この `index.md` にリンクを追加することで、サイト上から誰でもアクセスできるようになります。

---

## 3. GitHub Actions の設定 (`.github/workflows/verify.yml`) の工夫点

- **テストフォルダの除外**: 
  `competitive-verifier` は Google Test などの外部ライブラリをそのままでは解釈できないため、`oj-resolve` 実行時に `--exclude test/` を指定して単体テスト用のディレクトリを解析対象から外しています。
- **デプロイ先ディレクトリ**:
  デプロイステップで `publish_dir: ./.competitive-verifier/_jekyll` と指定し、ドキュメントの生成結果だけを `gh-pages` ブランチに送信するように調整しています。

---

## 4. 作業時の注意点（まとめ）
- 新しい機能を `include/gwen/` に追加し、仕様を `doc/` に書いたら、必ず **`.verify-helper/docs/index.md`** に目次としてリンクを追記してください。
- 競技プログラミング用の検証コードは `verify/` 配下に置くことで、自動的に依存関係が解決され、検証の合否がドキュメントサイトにバッジとして表示されるようになります。

---

## 5. トラブルシューティング: `BundleError: no such header` の罠と対策

`competitive-verifier` の仕様により、自作ライブラリを `#include "gwen/...` のようにインクルードしている場合、内部のバンドラは **環境変数 `CPLUS_INCLUDE_PATH` や `-I` オプションを無視し、必ずプロジェクトのルートディレクトリのみを起点としてパスを探す** というハードコードされた挙動をします。

本プロジェクトは一般的な C++ ライブラリ構成である `include/gwen/...` の階層を採用しているため、バンドラは `gwen/` フォルダをプロジェクト直下に見つけることができず、静かに `BundleError: no such header` を発生させてしまいます（しかも GitHub Actions のステップ自体は成功扱いとなるため、ドキュメント画面にエラーが表示されて初めて気づくことが多いです）。

**対策（導入済み）**:
この問題を回避するため、プロジェクトの直下に **`gwen -> include/gwen` へのシンボリックリンク** を配置しています。
これにより、バンドラはプロジェクト直下に `gwen` フォルダがあるように錯覚し、正しくソースコードを展開できるようになっています。誤ってこのシンボリックリンクを削除しないよう注意してください。

---

## 6. ローカルテスト (`verify.sh`) の独自拡張

手元で効率よく検証コードを実行できるよう、`scripts/verify.sh` にいくつかの便利な拡張機能を入れています。

### 制限時間 (TLE) のデフォルト設定
`verify.sh` 経由で実行した場合、**自動的に全テストに 10.0 秒の TLE (制限時間) が適用** されます。各テストの先頭に `// competitive-verifier: TLE 10.0` のような特別なアノテーションを記述しなくても、無限ループなどを確実に検知・遮断できます。

### MLE / RE の検知
`competitive-verifier` は実行時に裏で `online-judge-tools` を呼び出しており、以下のエラーを自動検知します。
- **RE (Runtime Error)**: セグメンテーションフォールトやアサーションエラーなど、プロセスが 0 以外で終了した場合に自動的に RE となります。
- **MLE (Memory Limit Exceeded)**: テスト先（Yosupo等）の既定メモリ制限（通常 1024MB など）を超過すると MLE と判定されます。

### テスト結果のサマリー出力
テストが完了すると、`verify/verify.log` にクリーンなログ（ANSIエスケープシーケンス除去済み）が出力されるほか、生成された `result.json` を `scripts/summary.py` が自動解析します。
結果として、**何個のテストがパスし (AC)、何個のテストが落ちたか (Failed)、および落ちたテストのパス** がコンソールにわかりやすくサマリー表示されます。
