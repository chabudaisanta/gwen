# LazyImplicitTreap

遅延評価付きインデックスベース動的配列（Lazy Implicit Treap）です。区間反転、区間作用、区間取得をサポートします。

## テンプレート引数

```cpp
template <acted_monoid M> class LazyImplicitTreap;
```
- `M`: 作用付きモノイド（`gwen::acted_monoid` を満たす必要があります）

## コンストラクタ

```cpp
LazyImplicitTreap<M>();
LazyImplicitTreap<M>(const std::vector<S>& vec);
```
- 空のTreap、または配列からTreapを構築します。
- **計算量**: $O(N)$

## メソッド

### size, empty, insert, erase, reverse, get, set, concat, to_vec
- `ImplicitTreap` と同様です（モノイド要素 `S` を扱います）。

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
- **計算量**: $O(1)$
