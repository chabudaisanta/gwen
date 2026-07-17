---
title: LazySortedTreap
documentation_of: //include/gwen/bbst/lazy_sorted_treap.hpp
---

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

### begin / end
```cpp
iterator begin() const;
iterator end() const;
```
- **制約**: なし
- 最小要素を指すイテレータ、および番兵（終端）イテレータを返します。
- **計算量**: $O(\log N)$

### lower_bound
```cpp
iterator lower_bound(const K& x) const;
```
- **制約**: なし
- 指定したキー `x` 以上の最初の要素を指すイテレータを返します。
- **計算量**: $O(\log N)$

### upper_bound
```cpp
iterator upper_bound(const K& x) const;
```
- **制約**: なし
- 指定したキー `x` より真に大きい最初の要素を指すイテレータを返します。
- **計算量**: $O(\log N)$

### 最大・最小要素の取得について
`largest` や `smallest` のような専用のメソッドの代わりに、イテレータ演算を利用して前後の要素にアクセスできます。
- `x` 以下の最大の要素: `auto it = t.upper_bound(x); if (it != t.begin()) --it;`
- `x` 以上の最小の要素: `auto it = t.lower_bound(x);`

## イテレータ (`iterator`) について

`std::set` のように、`operator++` や `operator--` を使って要素を前後に移動できる双方向イテレータを提供します。

### イテレータの無効化について (重要)
Treap はランダムな優先度に基づいて平衡を保つため、`insert` や `erase` などの**木構造を変更する操作を行うと、以前取得したイテレータは直ちに無効になります**。
イテレータを保持したまま要素の追加・削除を行わないようにしてください。操作後は必ずイテレータを再取得する必要があります。
- **計算量**: `++`, `--` の移動は $O(\log N)$（ならし $O(1)$）
