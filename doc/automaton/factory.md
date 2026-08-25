---
title: Automaton Factory
documentation_of: //include/gwen/automaton/factory.hpp
---

## 概要

`gwen::automaton` 名前空間には、桁 DP (`gwen::run_digit_dp`) で頻出する制約のオートマトンを簡単に生成するためのファクトリ関数群が定義されています。
これらの関数を用いて生成したオートマトンは、そのまま `run_digit_dp` に渡すか、あるいは `gwen::automaton_monoid::op` を用いて複数の制約を直積で結合することができます。

## 関数一覧

すべてのファクトリ関数は `template <i32 base = 10>` を受け取ります。

### digit_sum_exact

```cpp
Automaton<base> digit_sum_exact(i32 M);
```
各位の数字の和がちょうど `M` になる制約を表すオートマトンを生成します。
- 状態数: $M + 1$ (和が $M$ を超えた場合は遷移が無効になります)

**制約**

- `0 <= M < INT32_MAX`

### digit_sum_leq

```cpp
Automaton<base> digit_sum_leq(i32 M);
```
各位の数字の和が `M` 以下になる制約を表すオートマトンを生成します。
- 状態数: $M + 1$

**制約**

- `0 <= M < INT32_MAX`

### digit_sum_mod

```cpp
Automaton<base> digit_sum_mod(i32 M, i32 K);
```
各位の数字の和を `M` で割った余りが `K` になる制約を表すオートマトンを生成します。
- 状態数: $M$

**制約**

- `M > 0`
- `0 <= K < M`

### exclude_digits

```cpp
Automaton<base> exclude_digits(std::span<const i32> S);
```
配列 `S` に含まれる数字を一切使用しない制約を表すオートマトンを生成します。
- 状態数: $1$
- 備考: 禁止文字を踏んだ場合の遷移を無効（`-1`）にすることで経路を枝刈りしています。そのため、他のオートマトンと `op` で結合しても状態数が増加しません。

### value_mod

```cpp
Automaton<base> value_mod(i32 M, i32 K);
```
作られる数値そのものを `M` で割った余りが `K` になる制約を表すオートマトンを生成します (`base` 進数)。
- 状態数: $M$

**制約**

- `M > 0`
- `0 <= K < M`

### non_zero_count_exact

```cpp
Automaton<base> non_zero_count_exact(i32 M);
```
`0` 以外の数字がちょうど `M` 個現れる制約を表すオートマトンを生成します。
- 状態数: $M + 1$ (カウントが $M$ を超えた場合は遷移が無効になります)

**制約**

- `0 <= M < INT32_MAX`

### non_zero_count_leq

```cpp
Automaton<base> non_zero_count_leq(i32 M);
```
`0` 以外の数字が `M` 個以下現れる制約を表すオートマトンを生成します。
- 状態数: $M + 1$

**制約**

- `0 <= M < INT32_MAX`


### include_all_digits

```cpp
Automaton<base> include_all_digits(std::span<const i32> S);
```
配列 `S` に含まれる数字をすべて少なくとも1回以上使用する制約を表すオートマトンを生成します。
- 状態数: $2^{K}$ ($K$ は `S` の中に含まれるユニークで有効な文字の数)
- 備考: 内部で `S` に含まれる $0 \le c < \mathrm{base}$ を満たすユニークな文字を抽出し、それらをすべて踏んだ状態を受理状態とします。

**制約**

- 有効でユニークな数字の個数は31未満

### used_digits_count_exact

```cpp
Automaton<base> used_digits_count_exact(i32 k);
```

先頭ゼロを除いて、ちょうど `k` 種類の数字が使われるオートマトンを生成します。数値0は使用数字0種類として扱います。`k < 0`または`k > base`の場合は空言語を返します。

**制約**

- `1 <= base < 31`

**計算量**

- $O(2^{\mathrm{base}} \times \mathrm{base})$

### used_digits_count_leq

```cpp
Automaton<base> used_digits_count_leq(i32 k);
```

先頭ゼロを除いて、使用される数字が `k` 種類以下となるオートマトンを生成します。`k < 0`の場合は空言語、`k >= base`の場合はすべての数を受理します。

**制約**

- `1 <= base < 31`

**計算量**

- $O(2^{\mathrm{base}} \times \mathrm{base})$

### contains_pattern

```cpp
Automaton<base> contains_pattern(std::span<const i32> pattern);
```

固定長数字列に`pattern`が含まれるオートマトンを生成します。桁DPで補われた先頭ゼロも照合対象です。

**制約**

- `pattern`の各要素は`0 <= digit < base`
- `pattern.size() <= INT32_MAX - 1`

**計算量**

- $O(|pattern| \times \mathrm{base})$

## leading zeroの扱い

- `used_digits_count_*`は先頭ゼロを無視します。
- `contains_pattern`、`include_all_digits`、`exclude_digits`は先頭ゼロを通常の数字0として扱います。
- 桁和、剰余、非ゼロ桁数は先頭ゼロによって結果が変化しません。
