# gwen テスト仕様書 ＆ 使用方法

`gwen` ライブラリでは、ビルドツールに **CMake**、テストフレームワークに **Google Test (gtest)** を採用しています。本ドキュメントでは、テストの概要、詳細な仕様、および使用方法について記述します。

---

## 1. テスト構成の概要

テストは以下の2つの異なる役割を持つカテゴリで構成されています。

| テストカテゴリ | 格納ディレクトリ | 検証内容 | 実行方法の特色 |
|---|---|---|---|
| **1. ユニットテスト (Unit)** | `test/unit/` | 個別の `.hpp` ヘッダーモジュールが正しくビルドでき、基本的な機能や表明（assert）を満たすか | `gwen_tests` バイナリ内で直接 C++ コードとして呼び出して検証（超高速） |
| **2. 問題入出力テスト (Integration)** | `test/problem_runner/`<br>`test/problems/` | 実際のオンラインジャッジの形式に基づき、C++ ソルバに標準入力を与えた結果が出力と合致するか | CMake で各ソルバを個別ビルドし、テスト親プロセスが子プロセスを起動して検証（オンラインジャッジに近い形式） |

---

## 2. ディレクトリ構成

```
gwen/
├── CMakeLists.txt              # プロジェクト全体のビルド設定（gtest取得含む）
├── test/
│   ├── CMakeLists.txt          # テスト関連のビルド・ctest連携設定
│   ├── unit/                   # 種類1：ユニットテストコード
│   │   ├── test_factorize.cpp
│   │   ├── test_mdarray.cpp
│   │   └── ...
│   ├── problem_runner/         # 種類2：入出力検証用テストランナー
│   │   ├── runner_utils.hpp    # ポータブルな子プロセス実行・出力正規化ヘルパー
│   │   └── test_problems.cpp   # gtest 用のケース登録・呼び出しファイル
│   └── problems/               # 種類2：テスト用問題コードとケース
│       └── <problem_id>/
│           ├── solution.cpp    # 解答プログラム
│           ├── meta.yaml       # 属性定義（任意）
│           └── cases/          # 入出力ケース
│               ├── 01.in / 01.out
│               └── ...
└── scripts/
    └── run_tests.sh            # テストビルド＆実行用のラッパースクリプト
```

---

## 3. 使用方法（実行コマンド）

### A. 全テストの一括実行
プロジェクトルートで以下のスクリプトを実行します。自動的に CMake が走り、Google Test ライブラリをダウンロードしてコンパイルとテスト実行を行います。
```bash
./scripts/run_tests.sh
```

### B. 特定のテストのみ実行する (Google Test フィルタ)
Google Test の標準フィルタ機能（`--gtest_filter`）をスクリプト経由で渡すことができます。
```bash
# 素数関連のユニットテストのみ実行
./scripts/run_tests.sh --gtest_filter=PrimeTest.*

# LCA（最小共通祖先）の入出力テストケース 01 のみ実行
./scripts/run_tests.sh --gtest_filter=ProblemIntegrationTest.Lca_01

# 問題入出力テスト（Integration）のみ一括実行
./scripts/run_tests.sh --gtest_filter=ProblemIntegrationTest.*
```

### C. テスト一覧の表示
登録されているすべてのテストケースの名称を表示します。
```bash
./scripts/run_tests.sh --gtest_list_tests
```

---

## 4. 詳細仕様

### ① 種類1: ユニットテストの仕様・追加手順
* **仕様**:
  各モジュールの機能確認を行うコードです。Google Test をインクルードし、`TEST(<TestSuiteName>, <TestName>)` マクロを使用してアサーション（`EXPECT_EQ`, `EXPECT_TRUE` 等）を記述します。
* **追加手順**:
  1. `test/unit/` 配下に `test_xxxx.cpp` を作成します。
  2. `test/CMakeLists.txt` の `gwen_tests` ターゲットに新しく作成した cpp ファイルを追加します。
     ```cmake
     add_executable(gwen_tests
       ...
       unit/test_xxxx.cpp # 追記
     )
     ```

### ② 種類2: 問題入出力テストの仕様・追加手順
* **仕様**:
  * **ビルド**: 各問題の `solution.cpp` は CMake によって個別の実行ファイル（`problem_<problem_id>`）として `.build/test/` 内にビルドされます。これにより各解答プログラムはライブラリ本体（`include/`）のみに依存する形でコンパイル確認されます。
  * **検証ランナーの仕組み**: `runner_utils.hpp` 内の `test_utils::run_problem_exe` が POSIX の `popen` システムコールを使用し、`exe_path < input_path` というリダイレクトコマンドを実行します。
  * **正規化 (Normalization)**: オンラインジャッジの仕様に合わせるため、`normalize_output` 関数によって「各行の末尾の空白（およびキャリッジリターン `\r`）」と「出力全体の末尾にある空行」を取り除いた状態で比較検証を行います。
* **追加手順**:
  1. `test/problems/<problem_id>/` ディレクトリを作成し、解答コード `solution.cpp` および `cases/` 配下に `.in` / `.out` テストケースを作成します。
  2. `test/CMakeLists.txt` に問題用実行ターゲットを追加します。
     ```cmake
     add_executable(problem_xxxx problems/xxxx/solution.cpp)
     target_link_libraries(problem_xxxx PRIVATE gwen)
     ```
  3. `test/problem_runner/test_problems.cpp` に対応するテストスイートを追記します。
     ```cpp
     TEST(ProblemIntegrationTest, Xxxx_01) { run_test_case("xxxx", "01"); }
     ```
