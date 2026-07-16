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
