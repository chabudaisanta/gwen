# gwen ローカルテスト仕様

## 目的

`verify/` はオンラインジャッジへの**提出コード**（Library Checker, AtCoder 等）専用とする。
開発中の品質確認は `test/` で行う。

`test/` では **問題ベース** のテストを pytest で実行する。各モジュールに 1 ファイルの単体テストを置くのではなく、ABC C〜E 程度の「小さな問題」を多数用意し、gwen の機能が実戦問題で動くことを確認する。

## verify と test の役割分担

| | verify/ | test/ |
|---|---------|-------|
| 用途 | 外部ジャッジ提出 | ローカル回帰テスト |
| 実行 | verify-helper / 手動提出 | `pytest` |
| 入力 | ジャッジのテストケース | 同梱の `.in` / `.out` |
| 数 | 問題ごとに 1 ソルバ | 1 問題 × 複数ケース |

## ディレクトリ構成

```
test/
├── specification.md          # 本書
├── conftest.py               # pytest 設定・フィクスチャ
├── test_problems.py          # 問題テストのエントリポイント
├── helpers/
│   ├── runner.py             # コンパイル・実行・出力比較
│   └── discover.py           # 問題・ケースの列挙
└── problems/
    └── <problem_id>/         # 例: lca, factorize, abc362d
        ├── solution.cpp      # gwen を使った解答（必須）
        ├── meta.yaml         # タイトル・タグ・TL 等（任意）
        └── cases/
            ├── 01.in
            ├── 01.out
            └── ...
```

### 問題 ID の付け方

- 手書き問題: 機能名や内容がわかる snake_case（`factorize`, `lca`, `dijkstra_nonneg`）
- 既存コンテスト問題: 問題 ID（`abc362_d`）— 公式の非公開テストは使わず、**サンプル + 自作小ケース**のみ同梱する

### meta.yaml（任意）

```yaml
title: "根付き木の LCA"
tags: [graph, tree_lca]
timeout_sec: 2.0
```

- `tags`: フィルタ用（`pytest -k lca` や将来 `--tag graph`）
- `timeout_sec`: ケースごとの実行時間上限（デフォルト 2 秒）

## 問題の追加手順

1. `test/problems/<id>/` を作成
2. `solution.cpp` に gwen を使ったソルバを書く（`verify/` からコピーしてもよい）
3. `cases/` に `.in` / `.out` をペアで追加
   - サンプルケース（問題文より）
   - 境界・小規模ランダム（手計算 or 別解法で expected を生成）
   - エッジケース（N=1, 全点同値, など）
4. `./scripts/run_tests.sh --problem <id>` で確認
5. 問題が増えるほどカバレッジが上がる — **テスト項目は多いほどよい**

`solution.cpp` は `verify/` と同じ書き方（`using namespace std; using namespace gwen;`）に揃える。
`-DLOCAL` は runner が付与するので、ソース側で `#define LOCAL` は不要。

## pytest の動き

1. `test/problems/*/solution.cpp` を列挙
2. 各 `solution.cpp` を **1 回だけ** コンパイル（`debug/test_build/` にキャッシュ、ソースより exe が新しければ再コンパイルしない）
3. `cases/*.in` ごとに実行し、標準出力を `.out` と比較
4. 失敗時: コンパイルエラー / TLE / WA を pytest の失敗として報告

### 出力比較

- 行末空白は無視
- ファイル末尾の空行の有無は許容
- それ以外は完全一致

## 実行方法

```bash
# 依存（初回のみ）
python3 -m venv .venv
.venv/bin/pip install -r requirements-dev.txt

# 全問題
./scripts/run_tests.sh

# 特定問題のみ
./scripts/run_tests.sh --problem lca
./scripts/run_tests.sh --problem factorize

# pytest を直接叩く場合
python -m pytest test/ -v
python -m pytest test/ -k lca
```

## コミット前フック

```bash
./scripts/install_pre_commit_hook.sh   # 初回のみ
```

以降、`git commit` のたびに `./scripts/run_tests.sh` が走る。スキップする場合:

```bash
git commit --no-verify
```

## コンパイルフラグ

verify 用スクリプトと揃える:

```
-std=c++20 -I src -O2 -DLOCAL
```

環境変数 `CXX`（デフォルト `g++`）でコンパイラを変更可能。

## verify からの移行

| 旧（verify ルート） | 新（test/problems） |
|---------------------|---------------------|
| `verify/factorize.cpp` | `test/problems/factorize/` |
| `verify/mdarray.cpp` | 将来 `test/problems/mdarray/` 等 |
| `verify/yosupo/lca.cpp` | `test/problems/lca/`（ケース同梱） |
| `verify/atcoder/*.cpp` | 必要に応じて `test/problems/abc*_*/` |

`verify/` ルートの assert テストは段階的に `test/problems/` へ移し、`run_verify.sh` は提出コード専用に整理する。

## 将来

- GitHub Actions で `run_tests.sh` を CI 化（verify-helper より軽量）
- `meta.yaml` の `tags` による部分実行
- チェッジャー付きケース（`.out` の代わりに `.chk.py`）— 浮動小数点など
