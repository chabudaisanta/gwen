# LazySortedTreap

キーベースで区間作用・区間積が取得可能な順序付き多重集合・辞書（Treap）です。各キーに値を結びつけ、キーの範囲に対する区間積と遅延評価による区間作用をサポートします。

## テンプレート引数

```cpp
template <typename K, acted_monoid M, typename Compare = std::less<K>> class LazySortedTreap;
```
- `K`: キーの型
- `M`: 作用付きモノイド（`gwen::acted_monoid` を満たす必要があります）
- `Compare`: キーの比較関数オブジェクト型（デフォルトは `std::less<K>`）

## コンストラクタ

```cpp
LazySortedTreap<K, M, Compare>();
```
- 空のTreapを構築します。

## メソッド

### size, empty, contains, count, insert, erase, erase_all, set, prod, all_prod
- `ProdSortedTreap` と同様の操作を提供します。

### apply
```cpp
void apply(const K& lower, const K& upper, const F& f);
```
- 区間 `[lower, upper)` に含まれるキーを持つ要素に対して、作用素 `f` を適用します。
- **計算量**: $O(\log N)$
