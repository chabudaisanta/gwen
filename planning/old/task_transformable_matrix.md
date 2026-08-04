# TransformableMatrix 実装タスク

## 概要
行・列の交換、転置、回転などの操作を O(1) で行える2次元配列のビューアー `TransformableMatrix` を実装する。Nim の `cplib` の実装を C++ に移植する。

## 配置パス
- 実装: `include/gwen/ds/transformable_matrix.hpp`
- ユニットテスト: `test/unit/ds/transformable_matrix_test.cpp`
- Verifyコード: `verify/yosupo/` など適当な問題がない場合は、テストのみでカバーするかダミーの Verify を置くか検討。今回は `verify/dummy/transformable_matrix.test.cpp` とするか、適当なグリッド問題を探す。
- ドキュメント: `doc/ds/transformable_matrix.md` (AC Library スタイル)

## 命名規則・設計
- クラス名: `TransformableMatrix` (PascalCase)
- メソッド・変数: snake_case
- 型制約 (Concept): 特になし (任意の型 `T` について動作)。
- プリミティブ型: `gwen/types.hpp` の `i32`, `usize` などを利用する。

### メンバ構造体
```cpp
struct AxisView {
    i32 base_axis;
    bool reversed;
};
```

### メンバ変数
```cpp
std::vector<std::vector<T>> data;
std::array<i32, 2> size;
std::array<std::vector<i32>, 2> order;
std::array<AxisView, 2> view;
```

### メソッド
※すべての公開 API (クラス、構造体、メソッド) に対して、Doxygen 形式のコメント (`@brief`, `@tparam` 等) を付与すること。
- `TransformableMatrix(const std::vector<std::vector<T>>& a)`
- `i32 axis_len(i32 axis) const`
- `i32 h() const`
- `i32 w() const`
- `i32 base_index(i32 axis, i32 index) const`
- `i32 base_position(i32 axis, i32 index) const`
- `T& operator()(i32 i, i32 j)`
- `const T& operator()(i32 i, i32 j) const`
- `void swap_rows(i32 i, i32 j)`
- `void swap_columns(i32 i, i32 j)`
- `void transpose()`
- `void rotate(i32 num = 1)`
- `std::vector<std::vector<T>> to_vec() const`

## テストとVerify
- Google Test で初期化、各操作（交換、転置、回転）、要素へのアクセスが O(1) 相当（内部でのインデックス計算）で正しく動くか網羅的に検証。
- VerifyはAOJやYosupo Judgeで該当するピンポイントの問題がなければ、ローカル完結のユニットテストを主とする。
