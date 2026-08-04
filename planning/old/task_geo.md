# Geo (Geometry) モジュールの実装仕様

## 1. 概要
計算幾何学（Geometry）の基本的なコンポーネントを提供するモジュールを作成する。
初回の実装として、点とベクトルの基本操作を扱う `Point` クラスと、それを用いた凸包（Convex Hull）の構築関数を提供する。

## 2. 対象ファイル
- **本体**:
  - `include/gwen/geo/point.hpp`
  - `include/gwen/geo/convex_polygon.hpp`
- **テスト**:
  - `test/unit/geo_test.cpp`
- **Verifyコード**:
  - `verify/yosupo/geometry/static_convex_hull.test.cpp` (※ Yosupo Judge の Static Convex Hull などに相当)
- **ドキュメント**:
  - `doc/geo/point.md`
  - `doc/geo/convex_polygon.md`

## 3. クラス設計・命名規則
gwen のコーディング規約に従う。

### 3.1 Point
- **構造体名**: `Point<T>` (PascalCase)
  - `template <typename T>` とし、座標型を汎用化（`long long`, `double` 等）する。
- **メンバ変数**: `T x, y;`
- **メソッド・関数 (snake_case)**:
  - `dot(const Point& p) const` : 内積
  - `cross(const Point& p) const` : 外積
  - `norm() const` : 原点からの距離の2乗 ($x^2 + y^2$)
  - 各種演算子 (`+`, `-`, `*`, `==`, `!=` 等。**※ `<` などの大小比較演算子はオーバーロードしない**)
  - 用途に応じた比較関数として以下を提供する:
    1. `compare_x` : x座標基準
    2. `compare_y` : y座標基準
    3. `compare_xy` : x座標 -> y座標の辞書式順
    4. `compare_arg` : 偏角基準
    5. `compare_arg_norm` : 偏角 -> ノルム（原点からの距離）基準

### 3.2 CCW (Counter-Clockwise) 判定
- `ccw(const Point<T>& a, const Point<T>& b, const Point<T>& c)`
  - 点 `a`, `b`, `c` の位置関係（反時計回り、時計回り、同一直線上など）を判定する定数または関数。

### 3.3 凸包 (Convex Polygon)
- **関数名**: `convex_polygon(std::vector<Point<T>> pts)`
  - Andrew's Monotone Chain アルゴリズム等を用いて、凸多角形（凸包）を構成する頂点の列（反時計回りなど）を返す。将来の Convex Hull Trick との命名衝突を避けるため、関数名やファイル名には `convex_polygon` を用いる。
  - 制約・計算量: 頂点数 $N$ に対して $O(N \log N)$

## 4. Doxygen と Concept
- 全ての公開クラス、メソッド（`Point`, `convex_polygon` 等）に Doxygen 形式のコメント (`@brief`, `@tparam`, `@param`, `@return`) を付与する。
- 座標型に対する制約として、C++23 の標準コンセプト (`std::integral` や `std::floating_point` 等) を用いて制約をかける。独自に Concept を定義する場合は、サフィックス `_concept` を付けないこと（例：`number` とする）。

## 5. テスト・Verifyコード
- **Google Test**: `Point` クラスの各メソッド（`dot`, `cross`, CCW判定など）の正確性、および小さいケースにおける `convex_polygon` の結果を検証する。
- **Verify**: Competitive-Verifier を用い、`yosupo` の Static Convex Hull 問題等の入出力を通して正確性を保証する。

## 6. ドキュメント
- AC Library スタイルに従い、`## メソッド名` と見出しにし、`**制約**`、`**計算量**` を太字で記述する。
- `.verify-helper/docs/index.md` へのリンクを追加する。
