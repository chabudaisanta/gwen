# ProdImplicitTreap

区間積取得が可能なインデックスベースの動的配列（Treap）です。反転操作（非可換モノイド対応）と区間積取得をサポートします。

## テンプレート引数

```cpp
template <monoid M> class ProdImplicitTreap;
```
- `M`: モノイド（`gwen::monoid` を満たす必要があります）

## コンストラクタ

```cpp
ProdImplicitTreap<M>();
ProdImplicitTreap<M>(const std::vector<S>& vec);
```
- 空のTreap、または配列からTreapを構築します。
- **計算量**: $O(N)$

## メソッド

### size, empty, insert, erase, reverse, get, set, concat, to_vec
- `ImplicitTreap` と同様です（モノイド要素 `S` を扱います）。

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
