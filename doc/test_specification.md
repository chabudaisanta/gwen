# gwen テスト仕様書 ＆ 使用方法

`gwen` ライブラリでは、ビルドツールに **CMake**、テストフレームワークに **Google Test (gtest)** を採用しています。本ドキュメントでは、テストの概要、詳細な仕様、および使用方法について記述します。

---

## 1. テスト構成の概要

テストは以下の2つの異なる役割を持つカテゴリで構成されています。

| テストカテゴリ | 格納ディレクトリ | 検証内容 | 実行方法の特色 |
|---|---|---|---|
| **1. ユニットテスト (Unit)** | `test/unit/` | 個別の `.hpp` ヘッダーモジュールが正しくビルドでき、基本的な機能や表明（assert）を満たすか | `gwen_tests` バイナリ内で直接 C++ コードとして呼び出して検証（超高速） |
| **2. 問題入出力テスト (Integration)** | `test/problem_runner/`<br>`test/problems/` | 実際のオンラインジャッジの形式に基づき、C++ ソルバに標準入力を与えた結果が出力と合致するか | テストランナーが自動生成され、親プロセスが各ソルバの子プロセスを起動して検証（オンラインジャッジに近い形式） |

---

## 2. ディレクトリ構成

```text
gwen/
├── CMakeLists.txt              # プロジェクト全体のビルド設定（gtest取得含む）
├── test/
│   ├── CMakeLists.txt          # テスト関連のビルド・ctest連携設定 (※ターゲットは自動挿入されます)
│   ├── unit/                   # 種類1：ユニットテストコード
│   │   ├── test_factorize.cpp
│   │   └── ...
│   ├── problem_runner/         # 種類2：入出力検証用テストランナー
│   │   ├── runner_utils.hpp    # ポータブルな子プロセス実行・出力正規化ヘルパー
│   │   └── test_problems.cpp   # gtest 用のケース登録・呼び出しファイル (※自動生成されます)
│   └── problems/               # 種類2：テスト用問題コードとケース
│       └── <problem_id>/
│           ├── solution.cpp    # 解答プログラム
│           ├── meta.yaml       # 制約・問題文などのメタデータ定義
│           ├── problem.md      # meta.yaml から自動生成される問題仕様書
│           ├── validator.cpp   # (任意) testlib.h を用いた入力チェッカー
│           └── cases/          # 入出力ケース
│               ├── 01.in / 01.out
│               └── ...
└── scripts/
    ├── run_tests.sh            # テストビルド＆実行用のラッパースクリプト
    └── manage_problems.py      # [NEW] テストケース検出、コード生成、バリデーションを行う万能スクリプト
```

---

## 3. 使用方法（実行コマンド）

### A. 全テストの一括実行
プロジェクトルートで以下のスクリプトを実行します。自動的に `manage_problems.py` が走り、テストケースの検出と `test_problems.cpp` の生成を行った後、CMake によるビルドと Google Test が実行されます。
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

---

## 4. 詳細仕様

### ① 種類1: ユニットテストの仕様・追加手順
* **仕様**:
  各モジュールの機能確認を行うコードです。Google Test をインクルードし、`TEST(<TestSuiteName>, <TestName>)` マクロを使用してアサーション（`EXPECT_EQ`, `EXPECT_TRUE` 等）を記述します。
* **追加手順**:
  1. `test/unit/` 配下に `test_xxxx.cpp` を作成します。
  2. `test/CMakeLists.txt` の `gwen_tests` ターゲットに新しく作成した cpp ファイルを追加します。

### ② 種類2: 問題入出力テストの仕様・追加手順
* **仕様と自動化の仕組み**:
  * **テスト自動生成**: `scripts/manage_problems.py` が `test/problems/` 以下の全フォルダと `cases/*.in` をスキャンし、Google Test のケース関数定義 (`test_problems.cpp`) および CMake のソルバーターゲット定義 (`test/CMakeLists.txt` 内のマーカー間) を全自動で生成・更新します。
  * **ドキュメント生成**: 各問題の `meta.yaml` を読み取り、問題文・制約・サンプルケースを整形した美しい Markdown ファイル (`problem.md`) を自動生成します。
  * **検証ランナー**: ユニットテスト側から `popen` を通じてビルド済みのソルバーに標準入力として `.in` を流し込み、出力結果を正規化 (行末空白・空行削除) した上で比較検証します。

* **追加手順**:
  1. `test/problems/<problem_id>/` ディレクトリを作成します。
  2. 解答プログラム `solution.cpp` を作成します。
  3. 問題文や制約を記述した `meta.yaml` を作成します。
  4. `cases/` 配下に `.in` / `.out` テストケースを配置します。
  5. **以上で完了です。** `CMakeLists.txt` や `test_problems.cpp` を手動で編集する必要はありません！ `./scripts/run_tests.sh` を実行するだけで自動的に認識・テストされます。

### ③ 高度な機能 (manage_problems.py)
問題管理スクリプトを直接実行することで、以下の高度な機能を利用できます。

* **入力ケースのバリデーション (testlib.h)**
  問題フォルダに `validator.cpp` (testlib.h使用) を作成しておくと、すべての入力 `.in` が制約を満たしているかチェックできます。初回実行時に自動的に公式の `testlib.h` がダウンロードされます。
  ```bash
  python3 scripts/manage_problems.py validate-inputs
  ```

* **期待される出力 (.out) の一括生成 (naive.cpp による正答生成)**
  自己検証を防ぐため、出力の自動生成には `solution.cpp` を使用しません。問題フォルダに **`naive.cpp` (バグりにくい愚直解法プログラム)** が配置されている場合のみ、それをコンパイルしてすべての `.out` ファイルを自動的に生成・上書きします。
  ```bash
  python3 scripts/manage_problems.py generate-outputs
  ```
