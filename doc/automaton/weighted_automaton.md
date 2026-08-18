---
title: WeightedAutomaton
documentation_of: //include/gwen/automaton/weighted_automaton.hpp
---

# WeightedAutomaton

重み付きオートマトンと、その直積などの演算を定義するモジュールです。複数の制約を持つオートマトンを一つの `u64` bitmask (condition) に集約することができます。

## WeightedAutomaton

```cpp
template <gwen::i32 base, gwen::monoid WeightMonoid>
struct gwen::WeightedAutomaton
```

オートマトンの状態と重み付き遷移を管理する構造体です。

- `n`: オートマトンの状態数
- `condition_count`: このオートマトンが持つ条件の数 (デフォルト1)
- `init`: 初期状態と初期重みのペアのリスト
- `condition`: 各状態が満たしている条件を示す `u64` bitmask のリスト
- `edges`: 遷移リスト
- `edge_weights`: 遷移の重みリスト

### コンストラクタ

```cpp
WeightedAutomaton(i32 n, i32 condition_count = 1)
```

状態数 `n` と、管理する条件の数 `condition_count` を指定してオートマトンを作成します。

**制約**

- `0 <= condition_count <= 64`

**計算量**

- $O(N \times \mathrm{base})$

### add_init

```cpp
void add_init(i32 i, Weight w)
```

初期状態 `i` とその初期重み `w` を追加します。

**計算量**

- $O(1)$

### set_condition

```cpp
void set_condition(i32 i, u64 mask)
```

状態 `i` における条件の達成状況を `mask` に設定します。

**計算量**

- $O(1)$

### set_edge

```cpp
void set_edge(i32 from, i32 label, i32 to, Weight w)
```

状態 `from` から文字 `label` による遷移先を `to` に、その遷移重みを `w` に設定します。

**計算量**

- $O(1)$

### edge

```cpp
std::pair<i32, Weight> edge(i32 from, i32 label) const
```

状態 `from` から文字 `label` による遷移先と重みのペアを取得します。遷移できない場合は `to` に `-1` が返ります。

**計算量**

- $O(1)$

### trim

```cpp
void trim()
```

初期状態から到達不可能な状態を削除し、オートマトンを縮約します。状態のインデックスは詰め直されます。

**計算量**

- $O(N \times \mathrm{base})$

## weighted_automaton_monoid

```cpp
template <gwen::i32 base, gwen::monoid WeightMonoid>
struct gwen::weighted_automaton_monoid
```

重み付きオートマトンの演算（直積など）を扱う代数構造（モノイド）です。

### op

```cpp
static WeightedAutomaton<base, WeightMonoid> op(
    const WeightedAutomaton<base, WeightMonoid>& a, 
    const WeightedAutomaton<base, WeightMonoid>& b
)
```

2つのオートマトン `a` と `b` の直積オートマトンを計算します。
直積オートマトンの状態 $(i, j)$ は `i * b.n + j` にマッピングされます。
`condition` の合成は `(b.condition[j] << a.condition_count) | a.condition[i]` のように行われ、下位ビットに `a` の条件、上位ビットに `b` の条件が割り当てられます。
遷移の重みは `WeightMonoid::op` によって合成されます。

**制約**

- `a.condition_count + b.condition_count <= 64`

**計算量**

- $O(\mathrm{base} \times a.\mathrm{n} \times b.\mathrm{n})$

### e

```cpp
static WeightedAutomaton<base, WeightMonoid> e()
```

直積の単位元となるオートマトンを返します。
状態数が1、`condition_count` が0で、すべての文字に対する遷移先がその状態自身（状態 `0`）となり、重みは `WeightMonoid::e()` となります。

**計算量**

- $O(\mathrm{base})$

## build_weighted_automaton

```cpp
template <gwen::i32 base, gwen::monoid WeightMonoid>
WeightedAutomaton<base, WeightMonoid> build_weighted_automaton(
    const std::vector<WeightedAutomaton<base, WeightMonoid>>& automata
)
```

複数のオートマトンを順番に直積合成し、一つのオートマトンを構築します。
最初の要素が最下位のビットに、最後の要素が最上位のビットに割り当てられます。

**制約**

- `automata` に含まれるオートマトンの `condition_count` の合計が $64$ 以下

**計算量**

- オートマトンを順に直積合成するため、それぞれの計算量の和となります。
