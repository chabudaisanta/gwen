---
title: Deque
documentation_of: //include/gwen/ds/deque.hpp
---

# Deque

リングバッファベースの高速な両端キューです。
`std::deque` よりもメモリ確保のオーバーヘッドが小さく、インデックス計算にビット演算を用いることで高速に動作します。
要素へのアクセス、先頭・末尾の追加・削除などの基本的な操作を $O(1)$ でサポートします。
また、ランダムアクセス可能なイテレータも提供します。

## コンストラクタ

```cpp
Deque()
explicit Deque(usize count, const T& value = T(), const Allocator& alloc = Allocator())
```
- 引数なしの場合は空の `Deque` を構築します。
- `count` を指定した場合は、`count` 個の要素を `value` で初期化して構築します。
- **計算量**: $O(1)$ または $O(\mathrm{count})$

## メソッド

### operator[]
```cpp
T& operator[](usize index)
const T& operator[](usize index) const
```
`index` 番目の要素への参照を返します。
- **制約**: `0 <= index < size()`
- **計算量**: $O(1)$

### front / back
```cpp
T& front()
const T& front() const
T& back()
const T& back() const
```
先頭・末尾の要素への参照を返します。
- **制約**: `!empty()`
- **計算量**: $O(1)$

### push_back / push_front
```cpp
void push_back(const T& value)
void push_front(const T& value)
```
末尾・先頭に要素を追加します。ムーブセマンティクスを用いた追加(`T&&`)や直接構築(`emplace_back`, `emplace_front`)も利用可能です。
- **計算量**: ならし $O(1)$

### pop_back / pop_front
```cpp
void pop_back()
void pop_front()
```
末尾・先頭の要素を削除します。
- **制約**: `!empty()`
- **計算量**: $O(1)$

### clear
```cpp
void clear()
```
すべての要素を削除し、空にします。内部のメモリ領域の解放は行われません。
- **計算量**: $O(\mathrm{size()})$ （要素型が自明なデストラクタを持つ場合は実質 $O(1)$）

### reserve
```cpp
void reserve(usize new_cap)
```
最低でも `new_cap` 個の要素を格納できるように容量を確保します。実際の容量は `new_cap` 以上の最小の2の冪乗になります。
- **計算量**: $O(\mathrm{size()})$

### size / empty / capacity
```cpp
usize size() const
bool empty() const
usize capacity() const
```
要素数、空かどうか、現在確保されている容量を返します。
- **計算量**: $O(1)$

### イテレータ
```cpp
iterator begin()
iterator end()
reverse_iterator rbegin()
reverse_iterator rend()
```
（定数イテレータ用の `cbegin` 等もサポートされています）
ランダムアクセス可能なイテレータを返します。
- **計算量**: $O(1)$
