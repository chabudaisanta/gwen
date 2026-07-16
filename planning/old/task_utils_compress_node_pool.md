# `utils/compress.hpp` および `utils/node_pool.hpp` 移植タスク仕様案

## 1. 概要
配列の座標圧縮を行う `compress.hpp` と、データ構造のノードを管理する `node_pool.hpp` を移植します。
元の実装は非常にシンプルですが、型の安全性やC++23への対応、および名前空間の配置を整えます。
（元の `master` では `misc/` に配置されていましたが、今回の移植タスクに則り `utils/` へ配置します。）

## 2. モジュールの設計方針

### 2.1. `utils/compress.hpp` (座標圧縮)
- 元の実装:
  ```cpp
  template <typename T> std::vector<int> compress(const std::vector<T>& vec);
  ```
- 改善案:
  - 戻り値のインデックス型を `int` ではなく `i32` に統一します。
  - C++20以降の `std::ranges` アルゴリズム（`std::ranges::sort`, `std::ranges::unique`, `std::ranges::lower_bound`）を利用して、より安全かつ簡潔に実装します。

### 2.2. `utils/node_pool.hpp` (ノードプール)
- 元の実装:
  ```cpp
  template <typename Node> struct node_pool { ... };
  ```
- 改善案:
  - 競プロにおいて、動的確保の代わりに配列でノードを管理するための構造体です。
  - **新規追加 (ユーザー要望)**: 不要になったノードの ID を再利用するための `free_node(i32 id)` メソッドを追加します。内部で `std::vector<i32> free_list;` を持ち、`new_node` が呼ばれた際は `free_list` が空でなければそこからIDを再利用し、空であれば `pool` の末尾に追加するように拡張します。
  - メソッド名や振る舞いは既存のまま（`new_node`, `operator[]` など）とし、IDは `i32` で管理します。
  - 内部の `free_list` と `pool` の整合性（不変条件）を保つため、`RULES.md` に則り `class` としてカプセル化（`private` 変数化）します。

## 3. レビュー依頼事項（Subagent向け）
- `compress` に `std::ranges` を使用する際の懸念点がないか。
- `node_pool` を `class` 化してカプセル化する（または `struct` のままにする）ことの是非。
