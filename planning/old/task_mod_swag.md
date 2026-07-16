# SWAG (Sliding Window Aggregation) および Counting Sort 実装仕様書

## 概要
SWAG (Sliding Window Aggregation) は、スライディングウィンドウ（区間幅が伸縮する）に対してモノイドの区間畳み込み演算を $O(1)$ amortized で行うデータ構造です。
`master` ブランチの `include/gwen/query/swag.hpp` と、そこで利用されている `include/gwen/misc/counting_sort.hpp` を現行プロジェクトの規約に沿って移植します。

## 配置
- `counting_sort` ヘッダファイル: `include/gwen/algo/counting_sort.hpp`
- `swag` ヘッダファイル: `include/gwen/query/swag.hpp`
- テストファイル: `test/unit/query_swag_test.cpp`, `test/unit/algo_counting_sort_test.cpp`
- Verifyコード: `verify/yosupo/data_structure/queue_operate_all_composite.test.cpp` (Foldable Queue / SWAG Deque で解ける問題)
- ドキュメント: `doc/algo/counting_sort.md`, `doc/query/swag.md`

## クラス・メソッド設計

### counting_sort (algo/counting_sort.hpp)
- 関数 `counting_sort(const std::vector<T>& a, i32 m)` (snake_case)
- 型エイリアス `i32` の使用
- Doxygenコメント付与

### SWAG (query/swag.hpp)
#### `SlidingWindowAggregation` クラス (PascalCase)
- `SlidingWindowAggregation(i32 n)`: 初期化
- `void add_query(i32 l, i32 r)`: クエリ追加
- `std::vector<S> solve(const std::vector<S>& data, bool sorted = false)`: 計算の実行

#### `SwagDeque` クラス (PascalCase)
- Foldable Deque (両端キュー) による SWAG。
- メンバ関数: `fold()`, `push_front(S x)`, `push_back(S x)`, `pop_front()`, `pop_back()`
- C++23 Concept 制約: `template <monoid Monoid>` を使用 (モノイド要求)
- すべての公開APIに Doxygen 形式のコメントを付与。
- 型制約 `monoid` には `_concept` を付けず、すでに存在する `gwen::monoid` を使用します。

## テスト計画
1. **Google Test**
   - `counting_sort_test.cpp`: ソートの安定性や結果の正しさを検証。
   - `swag_test.cpp`: 結合則のみを満たすモノイド（非可換）を用いた Range Query を愚直計算と比較検証する。両端追加・削除に対する操作の正当性をテストする。
2. **Verify (`verify/yosupo/data_structure/queue_operate_all_composite.test.cpp`)**
   - Yosupo Judge の「Queue Operate All Composite」を使用し、アフィン変換 (Affine Transformation) の結合を `SwagDeque` (または `SlidingWindowAggregation`) を用いてオンラインまたはオフラインで解く。

## ドキュメント
- `doc/algo/counting_sort.md`, `doc/query/swag.md` に AC Library スタイルで記述する。
- `.verify-helper/docs/index.md` の目次へリンクを追加する。
