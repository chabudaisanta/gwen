# Task: Implement LazyImplicitTreap

## 概要
ドキュメント (`doc/bbst/lazy_implicit_treap.md`) が既に存在しているが実装がない `LazyImplicitTreap` を実装する。

## 設計
- **クラス名**: `LazyImplicitTreap`
- **ファイルパス**: `include/gwen/bbst/lazy_implicit_treap.hpp`
- **Concept**: C++23のConcept `acted_monoid` をテンプレート引数に適用する。
  ```cpp
  template <acted_monoid M> class LazyImplicitTreap;
  ```
- **メソッド名** (snake_caseを使用):
  - `size`, `empty`
  - `insert`, `erase`, `reverse`
  - `get`, `set`
  - `apply`, `all_apply`
  - `prod`, `all_prod`
  - `concat`, `to_vec`
- **ドキュメント (Doxygen)**: 
  - 全ての公開API (`class`, `insert`, `prod` など) に対して `@brief`, `@tparam` などのDoxygenコメントを付与する。

## テスト計画
- **Google Test**: `test/unit/bbst/lazy_implicit_treap_test.cpp`
  - `insert`, `erase`, `reverse`, `apply`, `prod` などの基本機能が正しく動作することを検証する。
- **Verifyコード**: `verify/yosupo/dynamic_array_with_lazy_eval.test.cpp` などを想定 (Yosupo Library Checker などに該当問題があればそれを使用、なければ自作のVerifyコードを作成)。今回は遅延評価付き動的配列のverifyを行う。

## ドキュメント更新
- `doc/bbst/lazy_implicit_treap.md` は既にAC Libraryスタイルで書かれているため、内容に不足があれば補足し、`.verify-helper/docs/index.md` へのリンク追加を確認する。
