---
title: LazyImplicitTreap
---

# LazyImplicitTreap

遅延評価付きインデックスベース動的配列（Lazy Implicit Treap）です。区間反転、区間作用、区間取得をサポートします。

## テンプレート引数

```cpp
template <acted_monoid M, bool Commutative = false> class LazyImplicitTreap;
```
- `M`: 作用付きモノイド（`gwen::acted_monoid` を満たす必要があります）
- `Commutative`: 区間積の演算（`M::op`）が可換であるかどうか。`true` を指定すると、区間反転 (`reverse`) 時の非可換用の計算（`rev_prod`）を省略するため、定数倍が高速になります。デフォルトは `false` です。

## コンストラクタ

```cpp
LazyImplicitTreap<M>();
```
- 空のTreapを構築します。
- **制約**: なし
- **計算量**: $O(1)$

```cpp
LazyImplicitTreap<M>(const std::vector<S>& vec);
```
- 配列からTreapを構築します。
- **制約**: なし
- **計算量**: $O(N)$

## メソッド

### size

```cpp
i32 size() const;
```
- 要素数を取得します。
- **制約**: なし
- **計算量**: $O(1)$

### empty

```cpp
bool empty() const;
```
- 空かどうかを判定します。
- **制約**: なし
- **計算量**: $O(1)$

### insert

```cpp
void insert(i32 pos, const S& x);
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
- 区間 $[l, r)$ の要素を反転させます。
- **制約**: $0 \le l \le r \le \text{size}()$
- **計算量**: $O(\log N)$

### apply

```cpp
void apply(i32 l, i32 r, const F& f);
```
- 区間 $[l, r)$ の要素に対して作用素 `f` を適用します。
- **制約**: $0 \le l \le r \le \text{size}()$
- **計算量**: $O(\log N)$

### all_apply

```cpp
void all_apply(const F& f);
```
- 全ての要素に対して作用素 `f` を適用します。
- **制約**: なし
- **計算量**: $O(1)$

### prod

```cpp
S prod(i32 l, i32 r);
```
- 区間 $[l, r)$ の要素の積を取得します。
- **制約**: $0 \le l \le r \le \text{size}()$
- **計算量**: $O(\log N)$

### all_prod

```cpp
S all_prod();
```
- 全要素の積を取得します。
- **制約**: なし
- **計算量**: $O(1)$

### get

```cpp
S get(i32 pos);
```
- 指定した位置の要素を取得します。
- **制約**: $0 \le \text{pos} < \text{size}()$
- **計算量**: $O(\log N)$

### set

```cpp
void set(i32 pos, const S& x);
```
- 指定した位置の要素を書き換えます。
- **制約**: $0 \le \text{pos} < \text{size}()$
- **計算量**: $O(\log N)$

### concat

```cpp
void concat(LazyImplicitTreap& other);
static LazyImplicitTreap concat(LazyImplicitTreap& t0, LazyImplicitTreap& t1);
```
- 別のTreapを連結します。
- **制約**: なし
- **計算量**: $O(\log N)$

### push_back

```cpp
void push_back(const S& x);
```
- 末尾に要素を追加します。
- **制約**: なし
- **計算量**: $O(\log N)$

### push_front

```cpp
void push_front(const S& x);
```
- 先頭に要素を追加します。
- **制約**: なし
- **計算量**: $O(\log N)$

### to_vec

```cpp
std::vector<S> to_vec();
```
- 現在のTreapの要素をstd::vectorとして返します。
- **制約**: なし
- **計算量**: $O(N)$
