# コーディングおよびドキュメント規約

## 1. ヘッダーファイル (`.hpp`) のフォーマット
すべてのヘッダーファイルは以下のフォーマット・ルールに従って記述します。

- **インクルードガード**: `#pragma once` を使用します。
- **標準ライブラリのインクルード**: 必要なものは `// std` などのコメントブロック以下にまとめて記述します。
- **ビルド成果物の除外**: ビルド成果物やデバッグ用テンポラリファイルはルートを汚さないよう `.build/` ディレクトリへ出力し、Git追跡から完全に除外（`.gitignore`）します。
- **エディタ設定の除外**: `.vscode/` 設定フォルダはローカルのみで保持し、Gitには含めません。
- **自作ヘッダのインクルード**: `gwen` 内部の自作ヘッダファイルをインクルードする際は、必ず `#include "gwen/..."` のようにダブルクォーテーション形式を使用します（`oj-bundle` 等で正しく展開されるようにするため）。
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
concept monoid = requires(T a, T b) {
    { a + b } -> std::same_as<T>;
    { T::id() } -> std::same_as<T>;
};

// 空行をあける

/**
 * @brief モノイドを扱うクラスの例
 */
template <monoid T>
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
FenwickTree<T> fw(int n)
\`\`\`

- 長さ $n$ の配列を作ります。初期値はすべて $0$ です。

**制約**

- $T$ は `gwen::monoid` concept を満たすこと。
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
一般的なC++の慣習およびC++標準委員会の提案文書（P1851R0 / P1754R1）のガイドラインに従い、以下の命名規則を標準とします。

- **ファイル名・ディレクトリ名**: 小文字のスネークケース（`snake_case`）を使用します。（例: `fenwick_tree.hpp`, `math`）
- **型名 (クラス、構造体、エイリアス)**: パスカルケース（`PascalCase`）を使用します。（例: `FenwickTree`, `LazySegtree`, `ModInt`）
- **`class` と `struct` の使い分け**:
  - **`struct`**: 数学的な概念（例: `ModInt`, `Point`）や単純なデータの集まりなど、基本型のように値ベースの演算を主目的とする型に使用します。一部のメンバが `private` でカプセル化されている場合であっても、この基準に合致するものは `struct` とします。
  - **`class`**: 複雑なデータ構造（例: `FenwickTree`, `SegmentTree`, `Graph`）や、内部状態の更新において厳密な不変条件の維持が必要なオブジェクト指向的なカプセル化を伴う型に使用します。
- **コンセプト名**:
  - `snake_case` 形式を使用し、コンセプトを示す接頭辞/接尾辞（例: `is_` や `_concept`）は使いません。
  - **特性 (Capabilities)**: 単一要件（特定の関数呼び出しが可能など）の場合は、要件を説明する**形容詞**とします。要求する特性が関数のときは、`-ible` や `-able` を付けます。（例: `swappable`, `copy_constructible`）
  - **抽象 (Abstractions)**: 複数要件から構成されるハイレベル・コンセプトの場合は、専門用語を表す**名詞**とします。コンセプトを満たす型の名前よりも総称的にします。（例: `forward_iterator`）
  - **その他の述語**:
    - 主に複数の型引数をとり型制約に利用されるときは、適切な**前置詞**で終わる名前とします。（例: `same_as<int>`, `constructible_with<args>`, `sentinel_for<iterator>`）
    - 主に別コンセプトの定義や `requires` 節で利用されるときは、前置詞は付けません。（例: `mergeable`）
- **関数名・メソッド名**: 小文字のスネークケースを使用します。（例: `add`, `get_val`, `lower_bound`）
- **変数名・メンバ変数名**: 小文字のスネークケースを使用します。（例: `edge_count`, `node_id`）
- **定数名 (const, constexpr)**: 大文字のスネークケースを使用します。（例: `MOD`, `INF`）
- **テンプレートパラメータ**: 意味が自明な場合は1文字の大文字（`T`, `U`, `N` など）、意味を明記する場合は `PascalCase`（例: `ValueType`, `Operator`）を使用します。
- **マクロ名**: 可能な限り使用を避けますが、必要な場合は `GWEN_` などのプレフィックスを付けた大文字のスネークケースを使用します。（例: `GWEN_ENABLE_DEBUG`）
