# コーディングおよびドキュメント規約

## 1. ヘッダーファイル (`.hpp`) のフォーマット
すべてのヘッダーファイルは以下のフォーマット・ルールに従って記述します。

- **インクルードガード**: `#pragma once` を使用します。
- **標準ライブラリのインクルード**: 必要なものは `// std` などのコメントブロック以下にまとめて記述します。
- **内部依存のインクルード**: `gwen/` 以下のヘッダーを読み込みます。
- **ネームスペース**: 全て `namespace gwen { ... }` の中に記述し、閉じカッコには `} // namespace gwen` のようにコメントを添えます。
- **Doxygenスタイルコメント**: クラス、関数、コンセプトには `/** ... */` スタイルでコメントを記述します。
- **空行**: 各関数や構造体の間には原則1行の空行を入れ、可読性を確保します。

```cpp
#pragma once

#include <vector>
#include <concepts>

#include "gwen/types.hpp"

namespace gwen {

/**
 * @brief 簡潔な説明
 * @tparam T テンプレート引数の説明
 */
template <typename T>
concept Monoid = requires(T a, T b) {
    { a + b } -> std::same_as<T>;
    { T::id() } -> std::same_as<T>;
};

// 空行をあける

/**
 * @brief モノイドを扱うクラスの例
 */
template <Monoid T>
struct MonoidWrapper {
    T val;
    // ...
};

} // namespace gwen
```

## 2. ドキュメント (`.md`) のフォーマット
`competitive-verifier` 向けに、GitHub Pages でレンダリングされる markdown ドキュメントは以下のフォーマットで統一します。AC Library スタイルをできる限り踏襲します。

- **Frontmatter**: `title` と `documentation_of` を必ず設定します。
- **セクション構成**:
  1. `## 概要` (Overview)
  2. `## 制約` (Requirements / Concepts)
  3. `## 各メソッド` (Methods / API)
  4. `## 使い方` (Usage)
  5. `## 注意点` (Notes / Warning - あれば)
  6. `## 計算量` (Complexity)

```markdown
---
title: モジュール名 (例: Basic Monoid)
documentation_of: //include/gwen/algebra/basic_monoid.hpp
---

## 概要
ここにアルゴリズムやデータ構造の概要を書きます。

## 制約
- `T` は `gwen::Monoid` concept を満たすこと。

## 各メソッド
### `T::id()`
- 単位元を返します。

### `T operator+(T a, T b)`
- 2つの元の演算結果を返します。

## 使い方
```cpp
#include "gwen/algebra/basic_monoid.hpp"

// サンプルコード
```

## 注意点
- 特殊なコーナーケースや未定義動作の条件があればここに記述します。

## 計算量
- `id()`: $O(1)$
- `operator+`: $O(1)$
```

## 3. テストコード (`.cpp`) の規約
単体テストのファイルでは、隠れた依存関係のミス（include忘れ）を検知し、ヘッダーの自己完結性を保証するために、**必ずテスト対象のヘッダーファイルをファイルの先頭でインクルード**します。

```cpp
// 1. テスト対象のヘッダーを最初にインクルードする
#include "gwen/misc/timer.hpp"

// 2. その他の標準ライブラリや外部ライブラリをインクルードする
#include <gtest/gtest.h>
#include <vector>

// ...
```
