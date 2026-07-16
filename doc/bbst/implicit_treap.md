---
title: ImplicitTreap
documentation_of: //include/gwen/bbst/implicit_treap.hpp
---

# ImplicitTreap

インデックスベースの動的配列（Treap）です。値の保持と反転のみをサポートする最速のTreapです。

## コンストラクタ

```cpp
ImplicitTreap<T>();
ImplicitTreap<T>(const std::vector<T>& vec);
```

- 空のTreap、または配列からTreapを構築します。
- **計算量**: $O(N)$

## メソッド

### size
```cpp
i32 size() const;
```
- 要素数を返します。
- **計算量**: $O(1)$

### empty
```cpp
bool empty() const;
```
- 空かどうかを返します。
- **計算量**: $O(1)$

### insert
```cpp
void insert(i32 pos, const T& x);
```
- 指定した位置に要素を挿入します。
- **制約**: $0 \le \text{pos} \le \text{size}()$
- **計算量**: $O(\log N)$

### erase
```cpp
void erase(i32 pos);
```
- 指定した位置の要素を削除します。
- **制約**: $0 \le \text{pos} < \text{size}()$
- **計算量**: $O(\log N)$

### reverse
```cpp
void reverse(i32 l, i32 r);
```
- 区間 $[l, r)$ を反転します。
- **制約**: $0 \le l \le r \le \text{size}()$
- **計算量**: $O(\log N)$

### get
```cpp
T get(i32 pos);
```
- 指定した位置の要素を取得します。
- **制約**: $0 \le \text{pos} < \text{size}()$
- **計算量**: $O(\log N)$

### set
```cpp
void set(i32 pos, const T& x);
```
- 指定した位置の要素を更新します。
- **制約**: $0 \le \text{pos} < \text{size}()$
- **計算量**: $O(\log N)$

### concat
```cpp
void concat(ImplicitTreap& other);
static ImplicitTreap concat(ImplicitTreap& t0, ImplicitTreap& t1);
```
- 2つのTreapを結合します。結合元のTreapは空になります。
- **計算量**: $O(\log N)$

### to_vec
```cpp
std::vector<T> to_vec();
```
- Treapの全要素を配列として返します。
- **計算量**: $O(N)$
