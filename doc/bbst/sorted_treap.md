---
title: SortedTreap
documentation_of: //include/gwen/bbst/sorted_treap.hpp
---

# SortedTreap

キーベースの順序付き多重集合（Treap）です。モノイドを持たず、キーの大小で二分探索ツリーを構築します。`std::multiset` に近い振る舞いをしますが、インデックス（$k$番目）でのアクセスが高速です。

## テンプレート引数

```cpp
template <typename K, typename Compare = std::less<K>> class SortedTreap;
```
- `K`: キーの型
- `Compare`: キーの比較関数オブジェクト型（デフォルトは `std::less<K>`）

## コンストラクタ

```cpp
SortedTreap<K, Compare>();
```
- 空のTreapを構築します。

## メソッド

### size, empty, to_vec
- `ImplicitTreap` と同様です。

### insert
```cpp
void insert(const K& key);
```
- キーを挿入します（重複を許容します）。
- **計算量**: $O(\log N)$

### erase
```cpp
void erase(const K& key);
```
- 指定したキーに一致する要素を1つ削除します。存在しない場合は何もしません。
- **計算量**: $O(\log N)$

### contains
```cpp
bool contains(const K& x);
```
- 指定したキーが存在するかを返します。
- **計算量**: $O(\log N)$

### count
```cpp
i32 count(const K& x);
i32 count(const K& lower, const K& upper);
```
- 指定したキー `x` と一致する要素の数、または区間 `[lower, upper)` に含まれるキーの数を返します。
- **計算量**: $O(\log N)$

### count_lower
```cpp
i32 count_lower(const K& key);
```
- 指定したキーより真に小さい要素の数を返します。
- **計算量**: $O(\log N)$

### kth
```cpp
K kth(i32 k);
```
- 0-indexedで昇順 $k$ 番目のキーを返します。
- **制約**: $0 \le k < \text{size}()$
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
