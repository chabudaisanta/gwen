# データ構造 (Treap) 仕様策定および設計見直し

## 1. 概要
Treap系のデータ構造の移植およびリファクタリング。ユーザーからの要望「treap系はできることも種類も多いので、前のをただリファクタリングして移植するだけでなく、よく検討してから作りたい」に基づき、全体設計の再検討と統合・整理を行う。

## 2. 現状の課題と分析 (master ブランチ)
現在、以下の4つのクラスが存在している：
1. `treap`: インデックスベース (暗黙キー)。遅延伝搬なし。
2. `implicit_treap`: インデックスベース。遅延伝搬あり（区間作用）、区間反転あり。
3. `sorted_treap`: キーベースの順序付き多重集合。モノイドなし。
4. `associative_treap`: キーベースのマップ。遅延伝搬あり（区間作用）。

**【課題】**
- 名前が機能を表しきれていない（`treap` が暗黙キーであることを名前から推測しづらい。`implicit_treap` と名前が被る）。
- 競技プログラミングにおいて、定数倍のオーバーヘッド（遅延伝搬の有無）は重要であるため、4つに分かれていること自体は妥当。しかし、コンセプトによる制約が緩い。
- 反転操作 (`reverse`) における非可換モノイドの扱い (`rev_prod`) の要求仕様が暗黙的。

## 3. 新しい設計案

### 3.1 命名規則の整理 (PascalCaseへ統一)
用途と機能（遅延伝搬の有無）が明確になるようにリネームします。
- `treap` $\to$ `ImplicitTreap` (インデックスベース、遅延評価なし)
- `implicit_treap` $\to$ `LazyImplicitTreap` (インデックスベース、遅延評価・反転あり)
- `sorted_treap` $\to$ `SortedTreap` (キーベース、モノイドなしの Ordered MultiSet)
- `associative_treap` $\to$ `LazyAssociativeTreap` (キーベース、遅延評価ありの Ordered Map)

### 3.2 テンプレート引数とコンセプトの厳格化
- **遅延評価なし (`ImplicitTreap`)**
  - 要求: `monoid` コンセプト（`S`, `op`, `e` のみ）
- **遅延評価あり (`LazyImplicitTreap`, `LazyAssociativeTreap`)**
  - 新規コンセプト `action_monoid` を定義し、`S`, `F`, `op`, `e`, `mapping`, `composition`, `id` を厳格に要求。
  - さらに `LazyImplicitTreap` は区間反転をサポートするため、モノイド側が反転に対応しているか（`rev_prod` などの有無）を `requires` 節で判定し、非可換時の反転をコンパイル時分岐で安全に処理する仕組みを導入。

### 3.3 共通化について
- 全てのTreapでノードの定義や `merge`/`split` のロジックが似ていますが、インデックスで分割するかキーで分割するか、また `push` (遅延伝搬) の有無で内部実装が異なるため、**無理に継承などで共通化せず、独立したクラスとして実装**します（定数倍高速化のため）。

## 4. クラスの具体的なインターフェース案

### `ImplicitTreap<monoid M>`
動的配列（`std::vector` の上位互換として）。
- `insert(pos, val)`, `erase(pos)`
- `get(pos)`, `set(pos, val)`
- `prod(l, r)`

### `LazyImplicitTreap<action_monoid M>`
区間更新・反転可能な動的配列。
- 上記に加え `apply(l, r, f)`, `reverse(l, r)`

### `SortedTreap<class K, class Compare>`
順序付き多重集合。
- `insert(key)`, `erase(key)`
- `count(key)`, `count(l_key, r_key)`
- `kth(k)` (k番目の要素取得)
- `lower_bound`, `upper_bound`

### `LazyAssociativeTreap<class K, action_monoid M, class Compare>`
区間更新可能なマップ。
- `set(key, val)`, `get(key)`, `erase(key)`
- `prod(l_key, r_key)`, `apply(l_key, r_key, f)`
- `kth(k)`

## 5. 作業手順
1. この仕様案のユーザーレビュー
2. `action_monoid` などの新コンセプトの定義 (`include/gwen/alge/action_monoid.hpp` 等)
3. 各Treapの実装とテスト作成
4. ドキュメント化
