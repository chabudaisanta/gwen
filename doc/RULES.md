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
- **セクション構成**: AC Library のフォーマットに合わせ、以下の構成を標準とします。
  1. `# モジュール名` と概要
  2. `## コンストラクタ` や `## メソッド名` ごとの見出し
  3. 各メソッドの直下に宣言のコードブロックを記述し、その下に説明を書く。
  4. 制約は `**制約**`、計算量は `**計算量**` という記法を用いる。
  5. サンプルコードや使い方は `## 使用例` の下に記述する。

```markdown
---
title: モジュール名 (例: Fenwick Tree)
documentation_of: //include/gwen/algebra/fenwick_tree.hpp
---

# Fenwick Tree

ここにモジュール全体の概要や、できることを簡潔に書きます。

## コンストラクタ

\`\`\`cpp
fenwick_tree<T> fw(int n)
\`\`\`

- 長さ $n$ の配列を作ります。初期値はすべて $0$ です。

**制約**

- $T$ は `gwen::Monoid` concept を満たすこと。
- $0 \leq n \leq 10^8$

**計算量**

- $O(n)$

## add

\`\`\`cpp
void fw.add(int p, T x)
\`\`\`

`a[p] += x` を行います。

**制約**

- $0 \leq p < n$

**計算量**

- $O(\log n)$

## 使用例

\`\`\`cpp
#include "gwen/algebra/fenwick_tree.hpp"

// サンプルコード
\`\`\`
```

## 3. テストコード (`.cpp`) の規約
単体テストのファイルでは、隠れた依存関係のミス（include忘れ）を検知し、ヘッダーの自己完結性を保証するために、**必ずテスト対象のヘッダーファイルをファイルの先頭でインクルード**します。

```cpp
// 1. テスト対象のヘッダーを最初にインクルードする
// clang-format off
#include "gwen/misc/timer.hpp"
// clang-format on

// 2. その他の標準ライブラリや外部ライブラリをインクルードする
#include <gtest/gtest.h>
#include <vector>

// ...
```

## 4. 命名規則 (Naming Conventions)
一般的なC++の慣習および競技プログラミングライブラリ（AC Libraryなど）のスタイルに従い、以下の命名規則を標準とします。

- **ファイル名・ディレクトリ名**: 小文字のスネークケースを使用します。（例: `fenwick_tree.hpp`, `math`）
- **型名 (クラス、構造体、エイリアス)**:
  - 基本的には標準ライブラリに倣い、**小文字のスネークケース**を使用します。（例: `fenwick_tree`, `lazy_segtree`, `modint`）
  - ただし、テンプレートパラメータや一部のコンセプトなどは **PascalCase** を許容・推奨します。（例: `Monoid`, `Graph`）
- **関数名・メソッド名**: 小文字のスネークケースを使用します。（例: `add`, `get_val`, `lower_bound`）
- **変数名・メンバ変数名**: 小文字のスネークケースを使用します。（例: `edge_count`, `node_id`）
- **定数名 (const, constexpr)**: 大文字のスネークケースを使用します。（例: `MOD`, `INF`）
- **テンプレートパラメータ**: 意味が自明な場合は1文字の大文字（`T`, `U`, `N` など）、意味を明記する場合は PascalCase（例: `ValueType`, `Operator`）を使用します。
- **マクロ名**: 可能な限り使用を避けますが、必要な場合は `GWEN_` などのプレフィックスを付けた大文字のスネークケースを使用します。（例: `GWEN_ENABLE_DEBUG`）
