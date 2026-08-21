---
layout: default
title: 作業手順
---

# 作業手順

この文書は、`gwen` を初めて触る作業者が、リポジトリのルール確認から変更の引き渡しまでを安全に進めるための手順です。

## 0. 作業を始める前に

まず、作業ディレクトリがリポジトリのルートであることを確認します。

```bash
pwd
git status --short
```

すでに変更がある場合は、その変更を自分の作業対象とみなさないでください。内容と所有者が不明な変更は、上書き・削除・整形を行わず、作業範囲を分けます。

## 1. ルールを読む

次の順序でルールを確認します。

1. リポジトリ直下の `AGENTS.md` があれば読む。
2. 直下にない場合は、現在のルールファイルである `.agents/AGENTS.md` を読む。
3. 詳細な規約は [RULES.md](RULES.md) を読む。
4. 新規モジュールや大きな変更では、[DEVELOPMENT.md](DEVELOPMENT.md) と本書の「今後の候補」を読む。

特に、ヘッダー・単体テスト・オンラインジャッジ検証・仕様書・公開ドキュメント目次の対応関係を確認してください。

## 2. リポジトリを把握する

主要な配置は次のとおりです。

| 場所 | 役割 |
| --- | --- |
| `include/gwen/` | 自作のヘッダーオンリーライブラリ |
| `include/atcoder/` | 競プロワークスペースとの互換性のために保持する AC Library ヘッダー |
| `test/unit/` | Google Test による単体テスト |
| `verify/` | `competitive-verifier` によるオンラインジャッジ検証 |
| `doc/` | 公開 API の仕様書、開発資料 |
| `.verify-helper/docs/index.md` | GitHub Pages のモジュール目次 |
| `scripts/` | ビルド、テスト、検査の補助スクリプト |

対象のヘッダー、既存テスト、仕様書、verify を先に検索します。

```bash
rg -n "対象となる型名または関数名" include test verify doc
```

似たモジュールがあれば、その実装・テスト・文書を基準にします。

## 3. 変更を計画する

変更するファイルと、必要な検証を小さく列挙します。

- ライブラリ変更: ヘッダー、単体テスト、仕様書、必要なら verify
- 公開ヘッダー追加: `.verify-helper/docs/index.md` への掲載
- 仕様書変更: `documentation_of` とヘッダーパスの対応確認
- verify 追加: `verify/<judge>/.../<problem>[_<implementation>].test.cpp` の命名と `#define PROBLEM` の確認

新規モジュールの実装や移植は、`.agents/AGENTS.md` に定められた次の手順を必ず守ります。

1. 依頼内容、変更対象、検証計画を作業記録にまとめ、`code_reviewer` の事前レビューを受ける。
2. 事前レビューの指摘を解消し、実装前にユーザーの承認を得る。
3. 承認後に実装、単体テスト、verify、仕様書、公開目次を更新する。
4. 検証後に `code_reviewer` の事後レビューを受け、指摘を解消する。
5. 作業内容と検証結果を報告し、確認後に Conventional Commits 形式でコミットする。

上記の事前レビューとユーザー承認が終わるまで、次の実装工程へ進んではいけません。

## 4. 実装する

- 自作ライブラリは `include/gwen/` に追加・変更します。
- `include/atcoder/` はワークスペース互換性のため、移動やディレクトリ再編をしません。
- 公開 API には Doxygen コメントを付けます。
- ヘッダーを追加した場合、対応する仕様書を `doc/` の同じ相対パスに置きます。
  - 例: `include/gwen/ds/example.hpp` → `doc/ds/example.md`
- 作業中に無関係なファイルを整形・改名しません。

## 5. 検証する

変更内容に応じて、次を実行します。

```bash
# 公開ドキュメントの目次・仕様書対応を確認
python3 scripts/check_docs.py

# 単体テストをビルドして実行
./scripts/test.sh

# verify を実行する。対象を絞る場合はパスを第1引数に渡す
./scripts/verify.sh
./scripts/verify.sh 'verify/yosupo/graph/*.test.cpp'
```

ドキュメントのみの変更でも、少なくとも `python3 scripts/check_docs.py` と `git diff --check` を実行します。ライブラリの振る舞いを変えた場合は、該当する単体テストと verify を必ず実行します。

## 6. 引き渡し前に確認する

```bash
git status --short
git diff --check
git diff
```

次を確認してから引き渡します。

- 変更が依頼範囲だけに収まっている。
- 意図しない生成物やローカル設定を含めていない。
- 実行した検証と結果を説明できる。
- 追加・変更した公開ヘッダーが、仕様書と公開目次に反映されている。

## 7. 報告する

報告には、少なくとも「変更内容」「変更した主なファイル」「実行した検証と結果」「残る注意点」を含めます。検証できなかった項目がある場合は、成功したように扱わず、理由を明記します。

## 8. 今後の候補

優先度を固定しない改善候補です。実装に着手するときは、この一覧ではなく当該変更の作業記録で設計と検証計画を明確にします。

- 検証基盤: 既存モジュールの competitive-verifier 向け verify とランダム化単体テストの拡充。
- dump: 各データ構造の dump 対応を進める。
- ハッシュ: `hash/zobrist.hpp` の設計改善・実装。
- 幾何: 凸包、偏角ソート、線分交差、多角形操作。
- データ構造: 永続データ構造、Static RMQ、SegTree Beats、両端優先度付きキュー、Binary Trie、Splay Tree、区間 set、連想 Treap、平方分割。
- グラフ・木: 木の基本アルゴリズム、Rerooting、SCC と DAG 縮約、Range Edge Graph。
- 文字列: Suffix Array、LCP、Z-algorithm、Manacher、Aho-Corasick。
- 数学: NTT/FFT、形式的冪級数、Stern-Brocot Tree、Barrett reduction、任意法の二項係数。
- ユーティリティ: 高速ハッシュマップ、Zobrist Hash、型ハッシュ、FastIO、多次元配列、特殊クエリ。
