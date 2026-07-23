# Deque 実装仕様書

## 概要
`std::deque` や `std::queue` はメモリ確保のオーバーヘッドが大きく低速な場合があるため、高速に動作する両端キュー (Deque) を実装する。
内部実装は2の冪乗サイズを維持するリングバッファとし、`std::vector` などをラップせず、アロケータ (`std::allocator`) を用いて直接メモリ管理を行うことで高速化を図る。

## 配置パス
- ヘッダ: `include/gwen/container/deque.hpp`
- テスト: `test/unit/container/deque_test.cpp`
- ドキュメント: `doc/container/deque.md`
- Verifyコード: `verify/yosupo/data_structure/deque.test.cpp` (※Library Checkerに直接dequeはないが、queueや関連問題があれば代用するか、適当な問題でverifyする。例えば double_ended_priority_queue のベースにするなど。もしくは verify/yosupo/data_structure/point_add_range_sum.test.cpp などの既存の枠組みの中で deque を使ってみるなど。今回は独自の verify として何らかの問題を利用、もしくは専用のテストを手厚く行う)

## 設計 (Design)
### クラス名
- `Deque<T>`
  - (命名規則: データ構造なので PascalCase)

### コンストラクタ・デストラクタ
- `Deque()`: 初期化
- `explicit Deque(usize count, const T& value = T())`: 指定要素数で初期化
- `~Deque()`: 要素の破棄とメモリ解放
- コピー / ムーブ・コンストラクタ、代入演算子も適切に実装（または必要に応じて delete）

### メソッド・変数 (snake_case)
- **アクセス**:
  - `T& operator[](usize index)`
  - `const T& operator[](usize index) const`
  - `T& front()`
  - `const T& front() const`
  - `T& back()`
  - `const T& back() const`
- **変更**:
  - `void push_back(const T& value)`
  - `void push_back(T&& value)`
  - `template<class... Args> void emplace_back(Args&&... args)`
  - `void push_front(const T& value)`
  - `void push_front(T&& value)`
  - `template<class... Args> void emplace_front(Args&&... args)`
  - `void pop_back()`
  - `void pop_front()`
  - `void clear()`
  - `void reserve(usize new_cap)`
- **状態**:
  - `usize size() const`
  - `bool empty() const`
  - `usize capacity() const`

### 実装詳細 (Concept, Types & Comments)
- **Conceptの適用**:
  - テンプレートパラメータ `T` に対して制約を設ける場合、C++23 の `concept` を使用する。
  - 制約の命名には `_concept` のようなサフィックスは付けない（例: `std::movable`, または独自の制約など）。
- **Doxygenコメント**:
  - すべての公開 API（クラス本体、各メソッド等）に対して、`@brief`, `@tparam`, `@param`, `@return` などの Doxygen コメントを必ず付与する。
- **内部実装**:
  - サイズやインデックスには `gwen::usize` などの定義済みエイリアスを使用する。
  - 内部バッファの容量 `cap_` は常に2の冪乗（初期値は16など）とし、インデックス計算を `index & (cap_ - 1)` で高速に行う。
  - 要素数 `size_` と先頭インデックス `head_` を管理し、末尾インデックスは `(head_ + size_) & (cap_ - 1)` 等で求める。
  - 容量不足時は容量を2倍に拡張（`reserve` を内部呼び出し）して再配置する。
  - 要素の構築・破棄には `std::allocator_traits` および配置 new (`placement new`)、明示的デストラクタ呼び出しを用いる。

## Verifyとテスト
- **Google Test (`test/unit/container/deque_test.cpp`)**:
  - 追加・削除・アクセス・拡張の網羅的なテスト。
  - 非自明な型（コンストラクタ/デストラクタの呼び出し回数をカウントする独自のクラスなど）を用いて、メモリリークや二重解放がないことを確認。
- **Verify (`verify/...`)**:
  - `yosupo` などの適当なクエリ処理問題（スタック/キューとしての利用ができる問題）を用いて `Deque` の動作を verify する。
  - Library Checkerに直接の "Queue/Deque" 問題がない場合は、適当な問題(例えば `Static Range Sum` 等) で内部バッファとして使って動作確認するか、AtCoder等向けの汎用的なVerify構成とする。

## ドキュメント
- `doc/container/deque.md` に AC Library スタイルで記載し、`index.md` へリンクを追加する。
