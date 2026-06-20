# mdarray

多次元配列クラス `gwen::mdarray<T, Rank>` を提供します。内部的には1次元の `std::vector<T>` でデータを保持し、C++23 の `std::mdspan` (独自実装へのフォールバックを含む `gwen::detail::mdspan_type`) を用いて多次元インデックスからのアクセスを高速化します。

## ヘッダファイル

```cpp
#include "gwen/container/mdarray.hpp"
```

## テンプレート引数

- `T`: 要素の型
- `Rank`: 配列の次元数 (`Rank >= 1`)

## コンストラクタ

```cpp
// デフォルトコンストラクタ（空の配列）
mdarray();

// std::array を使って形状と初期値を指定
explicit mdarray(std::array<i32, Rank> shape, T fill = T{});

// 可変長引数で次元サイズを直接指定（Rank と同数の引数が必要）
// 例: mdarray<int, 3> arr(10, 20, 30);
template <typename... Is>
explicit mdarray(Is... dims);
```

## メンバ関数

### 要素へのアクセス

```cpp
// 多次元インデックスによる要素への参照（Rank 個の引数が必要）
T& operator()(Is... indices);
const T& operator()(Is... indices) const;
```

### その他

- `i64 size() const`: 保持している総要素数を返します。
- `T* raw_data()` / `const T* raw_data() const`: 内部で保持している1次元配列（vector）の生ポインタを返します。
- `detail::mdspan_type<T, Rank>& view()`: アクセス用の `mdspan` ビューを取得します。
- `void fill(const T& x)`: すべての要素を `x` で埋め直します（内部の `std::fill` を呼び出します）。

## 演算子オーバーロード

- `std::istream& operator>>(std::istream& is, mdarray<T, Rank>& a)`: 標準入力（ストリーム）から `a.size()` 個の要素を順番に読み込みます。
