# 開発手順書 (Development Workflow)

このドキュメントは、本プロジェクト（gwen）において新規モジュールや機能を追加する際の、標準的な作業手順をまとめたものです。作業漏れを防ぐためのチェックリストとしてもご活用ください。

## 1. ヘッダーファイルの実装
- `include/gwen/` 以下の適切なディレクトリに `.hpp` ファイルを作成します（例: `include/gwen/io.hpp`）。
- C++23 の仕様に準拠し、汎用的な制約が必要な場合は `include/gwen/concept/` に `concept` の定義を追加・利用します。

## 2. ドキュメントの作成
- `doc/` ディレクトリ以下に、モジュールの仕様や使い方をまとめた Markdown ファイルを作成します（例: `doc/io.md`）。
- 簡単なサンプルコード（`#include "gwen/...` から始まるもの）を含めると後で使いやすくなります。

## 3. 単体テスト (Google Test) の作成と登録
1. `test/unit/` ディレクトリ以下にテストファイルを作成します（例: `test/unit/test_io.cpp`）。
2. `test/unit/CMakeLists.txt` を開き、作成したテストをビルド対象として追記します。
   ```cmake
   # 追記例
   add_executable(test_io test_io.cpp)
   target_link_libraries(test_io gwen gtest_main)
   include(GoogleTest)
   gtest_discover_tests(test_io)
   ```

## 4. 単体テストのビルドと実行
ターミナルでプロジェクトのルートディレクトリ（`gwen/`）を開き、以下のコマンドでテストを実行します。
```bash
# 1. CMakeの構成（初回または CMakeLists.txt 更新時）
cmake -S . -B .build

# 2. ビルド
cmake --build .build

# 3. テストの実行
cd .build && ctest --output-on-failure
```
※ `.build/` ディレクトリは `.gitignore` の対象となるため、リポジトリは汚れません。

## 5. 競技プログラミング用 Verify の作成
※ 型定義（`types.hpp`）など、ロジックを持たないものは省略可能です。
1. データ構造やアルゴリズムを実装した場合は、`verify/` ディレクトリ以下にオンラインジャッジ検証用のコード（例: `verify/graph/dijkstra_verify.cpp`）を作成します。
2. `competitive-verifier` のコマンド（またはCI）を用いて、正しく Verify が通るか確認します。

## 6. タスク管理 (TODO.md) の更新
- 作業が一段落したら、`planning/TODO.md` の該当タスクに完了のチェック `[x]` を入れ、現在の進捗を記録します。
