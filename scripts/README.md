---
layout: default
title: スクリプト使用方法
---

# Scripts
`gwen` ライブラリのビルド、テスト、および検証（verify）をターミナル上で簡潔に実行するためのユーティリティスクリプト群です。
すべてのスクリプトは、プロジェクトのルートディレクトリ、またはどのサブディレクトリから実行しても正しくプロジェクトルートを認識して実行されます。

## 前提条件 (Prerequisites)
スクリプトを実行する前に、以下の環境が整っていることを確認してください。
- **Bash** (または互換性のあるシェル)
- **CMake** (3.14以上) - ビルドとテストに必要です。
- **Python** (3.8以上) - `competitive-verifier` の実行に必要です。

---

## 1. `build.sh`
テスト環境の CMake の構成とビルドを行うスクリプトです。

### 使い方
```bash
./scripts/build.sh
```

### 処理内容
1. `cmake -S . -B .build` を実行し、`.build` ディレクトリに CMake のビルド環境を構成します（C++23標準を使用）。
2. `cmake --build .build` を実行し、構成したテスト等のターゲットをコンパイルします。

---

## 2. `test.sh`
単体テスト（Google Test）をビルドし、実行するスクリプトです。

### 使い方
```bash
./scripts/test.sh
```

### 処理内容
1. 内部で `./scripts/build.sh` を呼び出し、最新のコードでビルドが完了していることを保証します。
2. `.build` ディレクトリに移動し、`ctest --output-on-failure` を実行して単体テストを走らせます。テストに失敗した場合は詳細なエラーが出力されます。
3. テストの実行後、Google Testによる詳細な出力（標準出力や標準エラー出力など）を含むログファイルを、`test/test.log` としてコピー・保存します。これにより、デバッグ時の出力結果などをいつでも簡単に確認できます。

---

## 3. `verify.sh`
`competitive-verifier` を用いて、オンラインジャッジの問題に対する検証コードを実行するスクリプトです。

### 使い方
```bash
./scripts/verify.sh
```

### 処理内容
1. `competitive-verifier oj-resolve` で対象と依存関係を解決し、`competitive-verifier verify` で `verify/` ディレクトリ配下の検証コードをコンパイル・実行・判定します。
2. 実行時には全テストへ既定で 10.0 秒の TLE を設定し、実行エラーも検査します。
3. ANSI エスケープシーケンスを除去したログを `verify/verify.log` に保存し、`result.json` を集計して成否の要約を表示します。

---

## 4. `format.sh`
`clang-format` を用いて、プロジェクト内のC++ソースコード（`.cpp`, `.hpp`）をフォーマットするスクリプトです。

### 使い方
```bash
./scripts/format.sh
```

### 処理内容
1. `include`, `test`, `verify` ディレクトリ配下にあるすべての `.cpp` および `.hpp` ファイルを再帰的に検索します。
2. `clang-format -i` を実行し、ソースコードをプロジェクトのコーディングスタイル (`.clang-format`) に従って上書きフォーマットします。
