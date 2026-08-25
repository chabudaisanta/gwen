---
title: Automaton
documentation_of: //include/gwen/automaton/automaton.hpp
---

# Automaton

オートマトンと、その直積などの演算を定義するモジュールです。

## Automaton

```cpp
template <gwen::i32 base>
struct gwen::Automaton
```

オートマトンの状態と遷移を管理する構造体です。

- `n`: オートマトンの状態数
- `init`: 初期状態のリスト
- `accept`: 受理状態のリスト
- `edges`: 遷移リスト。状態 $i$ から文字 $x$ ($0 \le x < \mathrm{base}$) による遷移は `edges[i * base + x]` に格納され、遷移先の状態を表します。無効な遷移（遷移不可）の場合は `-1` となります。

`init` と `accept` の重複は許可されます。桁 DP では同じ状態が現れる回数を多重度として扱います。

### valid

```cpp
bool valid() const
```

状態数、初期・受理状態、遷移先、遷移配列のサイズが内部表現の制約を満たすか判定します。重複する初期・受理状態は不正とみなしません。

**計算量**

- $O(N \times \mathrm{base} + |\mathrm{init}| + |\mathrm{accept}|)$

### trim

```cpp
void trim()
```

初期状態から到達できない状態を削除し、状態番号を詰め直します。直積演算から自動的に呼ばれることはありません。

**計算量**

- $O(N \times \mathrm{base})$

## automaton_monoid

```cpp
template <gwen::i32 base>
struct gwen::automaton_monoid
```

オートマトンの演算（直積など）を扱う代数構造（モノイド）です。

### op

```cpp
static Automaton<base> op(const Automaton<base>& a, const Automaton<base>& b)
```

2つのオートマトン `a` と `b` の直積オートマトンを計算します。
直積オートマトンの状態 $(i, j)$ は `i * b.n + j` にマッピングされます。いずれか一方のオートマトンで遷移が無効（`-1`）である場合、直積オートマトンの遷移も無効（`-1`）となります。
到達可能性にかかわらず $a.n \times b.n$ 個の状態を構築し、`trim()` は自動実行しません。

**制約**

- `a.edges.size() == a.n * base`
- `b.edges.size() == b.n * base`
- `base >= 1`
- `a.valid()`および`b.valid()`が`true`
- `a.n * b.n <= INT32_MAX`

**計算量**

- $O(\mathrm{base} \times a.\mathrm{n} \times b.\mathrm{n})$

### e

```cpp
static Automaton<base> e()
```

直積の単位元となるオートマトンを返します。
状態数が1で、すべての文字に対する遷移先がその状態自身（状態 `0`）となります。

**制約**

- `base >= 1`

**計算量**

- $O(\mathrm{base})$

## 演算子オーバーロード

### operator*

```cpp
template <gwen::i32 base>
Automaton<base> operator*(const Automaton<base>& a, const Automaton<base>& b)
```

2つのオートマトンの直積を計算するためのシンタックスシュガーです。
内部的に `automaton_monoid::op(a, b)` を呼び出します。
左結合であるため、`a * b * c` のように3つ以上のオートマトンを容易に結合させることができます。

**計算量**

- `automaton_monoid::op` に準じます。
