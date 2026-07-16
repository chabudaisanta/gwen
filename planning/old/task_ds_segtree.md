# セグメント木 (Segment Tree) 実装方針

## 概要
競技プログラミングにおいて必須となるセグメント木の各種バリアントを `gwen` ライブラリとしてスクラッチ実装します。
過去のバージョンはACL（AtCoder Library）のアダプタでしたが、本プロジェクトのコンセプト（`monoid`, `acted_monoid` などの C++23 Concept を利用する設計）に合わせて1から再実装します。

## 対象モジュール
作成するデータ構造は以下の4種類とし、すべて `include/gwen/ds/` ディレクトリ配下に実装します。

1. **`SegmentTree`** (`segment_tree.hpp`)
   - 静的な区間積取得と一点更新をサポート
   - 要件: `gwen::monoid`

2. **`LazySegmentTree`** (`lazy_segment_tree.hpp`)
   - 遅延評価による区間作用と区間積取得をサポート
   - 要件: `gwen::acted_monoid`

3. **`DynamicSegmentTree`** (`dynamic_segment_tree.hpp`)
   - 座圧なしで広大な区間（例: $0 \le x < 10^9$）を扱える動的生成セグメント木
   - ポインタまたは `NodePool` によるメモリ管理を利用
   - 要件: `gwen::monoid`

4. **`LazyDynamicSegmentTree`** (`lazy_dynamic_segment_tree.hpp`)
   - 広大な区間に対する区間作用と区間積取得をサポートする動的生成セグメント木
   - 要件: `gwen::acted_monoid`

## 設計の詳細

### メモリ管理
- **通常のセグメント木**: `std::vector` によるフラットな配列として実装（2冪のサイズに拡張）。ACLや通常の競プロ実装と同様の $2N$ サイズの配列で高速化を図ります。
- **動的セグメント木**: `gwen::NodePool` を利用してノードを管理し、定数倍のオーバーヘッドを極限まで減らします。`new/delete` は使用しません。

### Concept の利用
- 各クラスのテンプレート引数 `M` は、`gwen::monoid` または `gwen::acted_monoid` コンセプトを満たす必要があります。
- C++23 `concept` を用いて要件を明示します。

### メソッドインターフェース
基本的にACLに準拠しつつ、より直感的な命名を採用します。
- `set(p, x)`: 一点更新（`Lazy` の場合は遅延評価を解決してから更新）
- `get(p)`: 一点取得
- `prod(l, r)`: 区間 $[l, r)$ の積を取得
- `all_prod()`: 全体の積を取得
- `apply(p, f)`: 一点作用 (Lazyのみ)
- `apply(l, r, f)`: 区間作用 (Lazyのみ)
- `max_right(l, g)`, `min_left(r, g)`: セグメント木上の二分探索（条件を満たす最大/最小のインデックスを返す）

## テストと検証
- **単体テスト**: `test/unit/segment_tree_test.cpp` などをそれぞれ作成し、Google Testでカバー率100%を目指します。
- **ドキュメント**: `doc/ds/` 配下に作成し `.verify-helper/docs/index.md` へリンクを追加します。
- **Verify**: テストコード追加後、verify-helper に対応するテストケース（Aizu Online Judge や Library Checker の問題）を追加するかは別途判断。まずは単体テスト完備を目標とします。
