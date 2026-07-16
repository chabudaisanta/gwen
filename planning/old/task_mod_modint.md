# 移植タスク: mod/modint.hpp および mod/mod.hpp

## 1. 概要
`master` ブランチから `mod/mod.hpp` (モジュラ演算の基礎関数) と `mod/modint.hpp` (`dynamic_modint64`) を `rebuild-v2` ブランチへ移植します。

## 2. 仕様案

### 2.1 パスとディレクトリ構造
- `include/gwen/mod/mod.hpp`: `pow_mod`, `inv_mod`, `inv_mod_64` などのモジュラ演算基礎関数を配置します。
- `include/gwen/mod/modint.hpp`: Montgomery乗算を用いた `dynamic_modint64` を移植・配置します（後述の通りリネームを提案）。

### 2.2 Concept の利用方針
- **`modint` コンセプトの定義**: ACL互換のインターフェースを保証するため、`include/gwen/concept/modint.hpp` を新設し、以下を満たす `modint` コンセプトを定義します。
  - 四則演算が可能 (`+`, `-`, `*`, `/`, `+=`, `-=`, `*=`, `/=`)
  - `val()` メソッド（現在の値を返す）
  - `T::mod()` 静的メソッド（法を返す。`DynamicModInt64` への追加実装）
  - `inv()` メソッド（逆元）
  - `pow(n)` メソッド（累乗）
- これに伴い、`DynamicModInt64` に `static u64 mod() { return n; }` を追加し、`modint` コンセプトを満たすように実装を調整します。

### 2.3 リファクタリングおよび変更点
1. **命名規則の適用**:
   - `doc/RULES.md` では「型名 (クラス、構造体、エイリアス) はパスカルケース (PascalCase) を使用します」と規定されています。
   - そのため、クラス名 `dynamic_modint64` を **`DynamicModInt64`**（または `DynamicModInt64`）へリネームすることを提案します。
2. **インクルード**:
   - `RULES.md` の通り、依存するヘッダは `#include "gwen/mod/mod.hpp"` のように絶対パスでインクルードします。
3. **その他**:
   - 既存の `inv_mod_64` 等の関数名は、関数なのでスネークケースのまま（`inv_mod_64`）とします。

## 3. 次のステップ
ユーザーから仕様レビュー（特に型のパスカルケース化への同意）を頂いた後、実装フェーズに移行します。
