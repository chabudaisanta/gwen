---
title: Automaton
documentation_of: //include/gwen/automaton/automaton.hpp
---

# Automaton

オートマトンと、その直積などの演算を定義するモジュールです。
桁DPの簡略化や、Aho-Corasick法の返り値などとして利用できます。

## Automaton

```cpp
template <gwen::ring T, gwen::i32 base>
struct gwen::Automaton
```

オートマトンの状態と遷移を管理する構造体です。

- `n_states`: オートマトンの状態数
- `init`: 初期状態のリスト
- `accept`: 受理状態のリスト
- `edges`: 遷移とコストのリスト。状態 $i$ から文字 $x$ ($0 \le x < \mathrm{base}$) による遷移は `edges[i * base + x]` に格納され、`{遷移先の状態, コスト}` を表します。コストの型 `T` は環 (`gwen::ring`) である必要があります。

## automaton_monoid

```cpp
template <gwen::ring T, gwen::i32 base>
struct gwen::automaton_monoid
```

オートマトンの演算（直積など）を扱う代数構造（モノイド）です。

### op

```cpp
static Automaton<T, base> op(const Automaton<T, base>& a, const Automaton<T, base>& b)
```

2つのオートマトン `a` と `b` の直積オートマトンを計算します。
直積オートマトンの状態 $(i, j)$ は `i * b.n_states + j` にマッピングされます。遷移コストは乗算 ($p \times q$) されます。

**制約**

- `a.edges.size() == a.n_states * base`
- `b.edges.size() == b.n_states * base`
- `base >= 1`

**計算量**

- $O(\mathrm{base} \times a.\mathrm{n\_states} \times b.\mathrm{n\_states})$

### e

```cpp
static Automaton<T, base> e()
```

直積の単位元となるオートマトンを返します。
状態数が1で、すべての文字に対する遷移先がその状態自身となり、コストは乗法単位元 `T(1)` となります。

**制約**

- `base >= 1`

**計算量**

- $O(\mathrm{base})$
