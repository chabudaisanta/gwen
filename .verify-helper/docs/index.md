---
layout: default
title: gwen Library Documentation
---

# gwen C++ Library

Chaby(https://x.com/chbdi)の競技プログラミング用 C++23 ライブラリのドキュメントサイトです。
AtCoder(https://atcoder.jp/users/chaby)

## ドキュメント一覧

### 統合テンプレート
- [統合テンプレート (template.hpp)](doc/template.md)

### 基本機能 (Core)
- [マクロ (macro.hpp)](doc/core/macro.md)
- [定数 (constants.hpp)](doc/core/constants.md)
- [グリッド探索 (grid.hpp)](doc/core/grid.md)
- [ユーティリティ (utils.hpp)](doc/core/utils.md)
- [ビット操作 (bit.hpp)](doc/core/bit.md)
- [型エイリアス (types.hpp)](include/gwen/types.hpp)
- [エラー出力マクロ(dump.hpp)](include/gwen/dump.hpp)
- [時間計測・制限時間管理 (timer.hpp)](doc/utils/timer.md)

### 数学 (Math)
- [素数・素因数分解 (prime.hpp)](doc/math/prime.md)
- [整数演算 (integer.hpp)](doc/math/integer.md)
- [基本数学 (basic.hpp)](doc/math/basic.md)

### 剰余演算 (Mod)
- [モジュラ演算基盤 (mod.hpp)](doc/mod/mod.md)
- [Dynamic ModInt (modint.hpp)](doc/mod/modint.md)
- [ModInt61 (mod61.hpp)](doc/mod/mod61.md)

### 入出力 (IO)
- [std::vector入出力 (vector.hpp)](doc/io/vector.md)
- [Yes/No 出力 (yesno.hpp)](doc/io/yesno.md)

### アルゴリズム (Algo)
- [インデックスソート (idxsort.hpp)](doc/algo/idxsort.md)
- [ランレングス圧縮 (runlength.hpp)](doc/algo/runlength.md)

### 代数構造 (Algebra)
- [モノイド (monoid.hpp)](include/gwen/alge/monoid.hpp)
- [アーベル群 (abel.hpp)](include/gwen/alge/abel.hpp)
- [環 (ring.hpp)](doc/alge/ring.md)

### データ構造 (Data Structures)
- [累積和 (prefix_sum.hpp)](doc/ds/prefix_sum.md)
- [Fenwick Tree (fenwick_tree.hpp)](include/gwen/ds/fenwick_tree.hpp)

### 開発用資料
- [リポジトリ全体README(README.md)](README.md)
- [開発手順書 (DEVELOPMENT.md)](doc/DEVELOPMENT.md)
- [CI/CDとドキュメントの仕組み (COMPETITIVE_VERIFIER.md)](doc/COMPETITIVE_VERIFIER.md)
- [コーディング規則およびドキュメント作成規則(RULES.md)](doc/RULES.md)
- [testlib.hの使用方法(test/third_party/README.md)](test/third_party/README.md)
- [テスト用ユーティリティの使用方法(test/utils/README.md)](test/utils/README.md)
- [スクリプト使用方法(scripts/README.md)](scripts/README.md)

---
*GitHubリポジトリ: [chabudaisanta/gwen](https://github.com/chabudaisanta/gwen)*
