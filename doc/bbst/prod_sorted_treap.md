---
title: ProdSortedTreap
documentation_of: //include/gwen/bbst/prod_sorted_treap.hpp
---

# ProdSortedTreap

キーベースで区間積が取得可能な順序付き多重集合・辞書（Treap）です。各キーに値を結びつけ、キーの範囲に対する区間積を計算します。

## テンプレート引数

```cpp
template <typename K, monoid M, typename Compare = std::less<K>> class ProdSortedTreap;
```
- `K`: キーの型
- `M`: モノイド（`gwen::monoid` を満たす必要があります）
- `Compare`: キーの比較関数オブジェクト型（デフォルトは `std::less<K>`）

## コンストラクタ

```cpp
ProdSortedTreap<K, M, Compare>();
```
- 空のTreapを構築します。

## メソッド

### size, empty, contains, count
- `SortedTreap` と同様です。

### insert
```cpp
void insert(const K& key, const S& val = M::e());
```
- キーと値のペアを挿入します（キーの重複を許容します）。
- **計算量**: $O(\log N)$

### erase
```cpp
void erase(const K& key);
```
- 指定したキーに一致する要素を1つ削除します。
- **計算量**: $O(\log N)$

### erase_all
```cpp
void erase_all(const K& key);
```
- 指定したキーに一致する要素を全て削除します。
- **計算量**: $O(\log N)$

### set
```cpp
void set(const K& key, const S& val);
```
- 指定したキーに一致する要素を全て削除し、新しいキーと値のペアを1つ挿入します（Map的な挙動）。
- **計算量**: $O(\log N)$

### prod
```cpp
S prod(const K& lower, const K& upper);
```
- 区間 `[lower, upper)` に含まれるキーを持つ要素の積を取得します。
- **計算量**: $O(\log N)$

### all_prod
```cpp
S all_prod() const;
```
- 全要素の積を取得します。
- **計算量**: $O(1)$

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
