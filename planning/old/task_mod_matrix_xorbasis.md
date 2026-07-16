# Matrix および XorBasis (Noshi基底) の実装仕様書

## 概要
`master` ブランチの `include/gwen/math/matrix.hpp` を現行プロジェクトの規約に沿って移植するとともに、`include/gwen/math/xor_basis.hpp` についてはユーザーの要望に基づき「Noshi基底」のアルゴリズムを採用した設計で再実装します。

## 配置
- `matrix` ヘッダファイル: `include/gwen/math/matrix.hpp`
- `xor_basis` ヘッダファイル: `include/gwen/math/xor_basis.hpp`
- テストファイル: `test/unit/math_matrix_test.cpp`, `test/unit/math_xor_basis_test.cpp`
- Verifyコード: 
  - Matrix: `verify/yosupo/math/matrix_det.test.cpp`, `verify/yosupo/math/matrix_product.test.cpp`, `verify/yosupo/math/inverse_matrix.test.cpp` 等
  - XorBasis: (Yosupo Judgeの適切な問題など) -> 例: `verify/yosupo/data_structure/set_xor_min.test.cpp` など (問題がなければ省略か単純なユニットテストのみ)
- ドキュメント: `doc/math/matrix.md`, `doc/math/xor_basis.md`

## クラス・メソッド設計

### Matrix (math/matrix.hpp)
- **クラス名**: `Matrix` (PascalCase)
- **Concept制限**: `matrix_field` を `MatrixField` (PascalCase化せず、`gwen::matrix_field` などを採用するか、Conceptは snake_case の方針なので `matrix_field` のままとする)
- メンバ変数の大文字小文字修正: `N`, `M` を `n`, `m` 等の `snake_case` に変更。
- メソッド名: `rows()`, `cols()`, `is_square()`, `det()`, `rank()`, `trace()`, `inverse()` 等を維持。
- Doxygenコメント付与。

### XorBasis (math/xor_basis.hpp)
- **クラス名**: `XorBasis` (PascalCase)
- **型パラメタ**: `template <std::unsigned_integral T>` または `template <class T>`
  - 旧仕様の `BitWidth` 依存 `std::bitset` から、`u32` や `u64` などの符号なし整数型へ変更（Noshi基底は整数の大小比較 `std::min` を活用するため）。
- **Noshi基底のアルゴリズム**:
  - 挿入処理 `insert(T x)`: `for (T b : basis) x = std::min(x, x ^ b);` を行い、残った `x` があれば `basis` に追加して降順にソートする。
  - `can_form(T x)`: 同様の掃き出しを行い `0` になるか判定する。
  - 結合律 `op(XorBasis a, XorBasis b)`: マージ演算をサポートする。
- 命名規則・Doxygenコメントを徹底する。

## ワークフローの進行
この仕様案について `code_reviewer` エージェントに事前レビューを依頼し、その後にユーザーへ承認を求めます。
