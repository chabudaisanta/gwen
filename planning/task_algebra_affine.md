# 代数構造 (range_affine_range_sum_monoid) 移植タスク

## 1. 概要
遅延評価セグメント木などで使用される「区間アフィン変換・区間和」のための作用付きモノイド（または代数構造）の定義を移植する。

## 2. 移植対象のファイル
- `include/gwen/algebra/range_affine_range_sum_monoid.hpp` (masterからのパス。移植後は `include/gwen/alge/range_affine_range_sum_monoid.hpp` に配置する。`alge` ディレクトリの命名に合わせる)

## 3. 仕様・設計方針
- **名前空間とパス**: `gwen` ネームスペース内に配置。ディレクトリは `include/gwen/alge/` とする（既存の `monoid.hpp` がここにあるため）。
- **構造体名**: `range_affine_range_sum_monoid`。`RULES.md` の PascalCase 規則から外れますが、既存の `sum_monoid` などに合わせたスネークケースを維持します（値を持つオブジェクトではなく、型トレイト的な利用をされるため）。この例外の可否はユーザーに確認します。
- **内容**:
  - `struct S { T val, len; };` : 要素（区間和とその区間の長さ）。
  - `op(S a, S b)` : `{a.val + b.val, a.len + b.len}`。
  - `e()` : `{T(0), T(0)}`。
  - `unit(T x)` : `{x, T(1)}` (配列の初期化などで便利)。
  - `struct F { T a, b; };` : 作用（アフィン変換 $ax + b$）。
  - `mapping(F f, S x)` : `{f.a * x.val + f.b * x.len, x.len}`。
  - `composition(F f, F g)` : $f(g(x)) = f.a (g.a x + g.b) + f.b = (f.a \times g.a) x + (f.a \times g.b + f.b)$。したがって `{f.a * g.a, f.a * g.b + f.b}`。
  - `id()` : `{T(1), T(0)}`。
- **C++23化**: 
  - 定数式で評価できるよう、全ての静的関数に `constexpr` を付与する。
  - すべての公開 API に対して Doxygen 形式のコメントを付与する。

## 4. 依存関係
- `gwen::ring<T>` (`include/gwen/alge/ring.hpp`) を用いて、テンプレート引数 `T` が加法および乗法をサポートしていることを制約する（`template <ring T> struct range_affine_range_sum_monoid`）。

## 5. 作業手順
1. 本ファイルの作成（現在完了）
2. サブエージェントによる事前レビュー依頼
3. ユーザーへの仕様レビュー依頼
4. 実装の実施 (`range_affine_range_sum_monoid.hpp`)
5. サブエージェントによる実装レビュー
6. テスト (`range_affine_range_sum_monoid_test.cpp`)。
   （必要に応じて `ModInt` と組み合わせて挙動を確認）
7. ドキュメント作成 (`doc/alge/range_affine_range_sum_monoid.md`) と `index.md` への追加
8. 最終レビューとコミット
