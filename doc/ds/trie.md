---
title: Trie木 (Prefix Tree)
documentation_of: //include/gwen/ds/trie.hpp
---

## 概要

文字列の集合を効率的に管理・検索できる Trie木（プレフィックス木）の実装です。
各ノードに「そのノードで終了する単語数」や「そのノードをプレフィックスとして持つ単語数」を保持しており、高速な追加・削除・検索をサポートします。

## 構造体と型エイリアス

### TrieNodeMinimum

```cpp
template <i32 Width> struct TrieNodeMinimum
```

Trie木のノードを表す構造体です。文字の種類数を `Width` で指定します。

### TrieBase

```cpp
template <typename Node, i32 Width, i32 Offset> class TrieBase
```

Trie木本体のベースクラスです。

### 型エイリアス

一般的な用途向けに以下のエイリアスが用意されています。

```cpp
using LowerAlphaTrie = TrieBase<TrieNodeMinimum<26>, 26, 'a'>;
using UpperAlphaTrie = TrieBase<TrieNodeMinimum<26>, 26, 'A'>;
using AlphaTrie      = TrieBase<TrieNodeMinimum<52>, 52, 'A'>;
```

## メソッド

### insert

```cpp
template <std::ranges::range Container> tree insert(const Container& str)
```

文字列 `str` をTrie木に登録します。

**制約**

- `str` の各要素の文字コードから `Offset` を引いた値 `i` が $0 \le i < Width$ を満たすこと。

**計算量**

- $O(|S|)$ （$|S|$ は文字列の長さ）

### erase

```cpp
template <std::ranges::range Container> void erase(const Container& str, i32 n = 1)
```

文字列 `str` をTrie木から削除します。
$n \le 0$ の場合は該当する文字列をすべて削除し、$n > 0$ の場合は最大 $n$ 個まで削除します。

**制約**

- `str` の各要素の文字コードから `Offset` を引いた値 `i` が $0 \le i < Width$ を満たすこと。

**計算量**

- $O(|S|)$

### count

```cpp
template <std::ranges::range Container> i32 count(const Container& str) const
```

文字列 `str` がTrie木に何個登録されているかを返します。

**制約**

- `str` の各要素の文字コードから `Offset` を引いた値 `i` が $0 \le i < Width$ を満たすこと。

**計算量**

- $O(|S|)$

### contains

```cpp
template <std::ranges::range Container> bool contains(const Container& str) const
```

文字列 `str` がTrie木に1つ以上登録されているかを判定します。

**制約**

- `str` の各要素の文字コードから `Offset` を引いた値 `i` が $0 \le i < Width$ を満たすこと。

**計算量**

- $O(|S|)$

### match_count

```cpp
template <std::ranges::range Container> i32 match_count(const Container& str) const
```

文字列 `str` をプレフィックスとして持つ単語が何個登録されているかを返します。

**制約**

- `str` の各要素の文字コードから `Offset` を引いた値 `i` が $0 \le i < Width$ を満たすこと。

**計算量**

- $O(|S|)$

### path

```cpp
template <std::ranges::range Container> std::vector<tree> path(const Container& str) const
```

文字列 `str` を根から辿ったときのノードIDの列を返します。途中で木が途切れた場合は終端として `NIL(0)` が末尾に追加されます。

**制約**

- `str` の各要素の文字コードから `Offset` を引いた値 `i` が $0 \le i < Width$ を満たすこと。

**計算量**

- $O(|S|)$
