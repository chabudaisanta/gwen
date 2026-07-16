# Mo's Algorithm (Mo法) 実装仕様書

## 概要
オフラインの区間クエリを処理する Mo's Algorithm (Mo法) を実装する。
配列長 $N$、クエリ数 $Q$ としたとき、$O((N+Q)\sqrt{N})$ の計算量で各クエリの解を求めるための汎用クラスを提供する。

## 配置
- ヘッダファイル: `include/gwen/algo/mo.hpp`
- テストファイル: `test/unit/algo/mo_test.cpp`
- Verifyコード: `verify/yosupo/data_structure/static_range_inversions_query.test.cpp` (Mo法で解ける典型問題)
- ドキュメント: `doc/algo/mo.md`

## クラス・メソッド設計

### クラス名
`Mo` (PascalCase)

### メンバ関数
- `Mo(int n)` : 初期化 (要素数 $N$)
- `void add_query(int l, int r)` : クエリ $[l, r)$ を追加する (0-indexed、半開区間)
- `template <typename Add, typename Del, typename Calc>`
  `void build(Add add, Del del, Calc calc)` : 
  すべてのクエリを処理する。
  - `add(int i)`: 添字 `i` の要素を区間に追加する
  - `del(int i)`: 添字 `i` の要素を区間から削除する
  - `calc(int q_idx)`: 現在の区間状態を用いて、`q_idx` 番目のクエリの答えを記録/計算する
- `template <typename AddLeft, typename AddRight, typename DelLeft, typename DelRight, typename Calc>`
  `void build(AddLeft add_left, AddRight add_right, DelLeft del_left, DelRight del_right, Calc calc)` : 
  左右で追加・削除の処理が異なる場合に使用する汎用版。

### Concept適用
コールバック関数 `Add`, `Del`, `Calc` に対し、`std::invocable<Add, int>` などの標準コンセプトを用いて型制約を課す (C++20/23)。

## テスト計画
1. **Google Test (`test/unit/algo/mo_test.cpp`)**
   - 単純な区間和 (Range Sum) を Mo法 で計算し、愚直な解法と一致するか検証。
   - クエリの追加と、コールバック関数の呼び出し回数が $O(N\sqrt{Q})$ となっているかの動作確認。
2. **Verify (`verify/yosupo/data_structure/static_range_inversions_query.test.cpp`)**
   - Yosupo Judge の `Static Range Inversions Query` (区間転倒数) などの Mo法 が想定解・適用可能な問題で Verify を行う。
   ※ Yosupo Judge に Mo法単体のシンプルな問題はないが、区間転倒数などが有名。もし FenwickTree (BIT) などが既存であれば組み合わせて解く。

## ドキュメント
- `doc/algo/mo.md` に AC Library スタイルで記載する。
- `.verify-helper/docs/index.md` の目次に `algo/mo.md` へのリンクを追加する。
