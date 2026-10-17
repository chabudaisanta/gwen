---
title: Aho-Corasick (aho_corasick.hpp)
documentation_of: //include/gwen/automaton/aho_corasick.hpp
---

# Aho-Corasick

複数のパターンの末尾一致を判定する `Automaton<base>` を構築します。状態 `0` が初期状態です。すべての状態と文字について遷移が定義されています。

## aho_corasick

```cpp
template <i32 base>
Automaton<base> gwen::automaton::aho_corasick(std::span<const std::vector<i32>> patterns);
```

テキストを左から読み、各文字の後に現在状態が `accept` に含まれるかを調べると、その位置でいずれかのパターンが終わるかを判定できます。短いパターンが長いパターンの接尾辞である場合も検出します。以後の文字によって非受理状態に戻ることがあります。一致したパターンの番号は返しません。

空のパターン集合に対しては、状態 `0` のみを持ち、受理状態のないオートマトンを返します。

**制約**

- `base >= 1`。
- 各パターンは空でなく、各要素は `[0, base)` に属すること。
- Trie の頂点数は `i32` の範囲内であること。

**計算量**

- $S$ を Trie の頂点数、$L$ をパターンの総長とすると、構築時間は $O(L + S\,\mathrm{base})$、使用領域は $O(S\,\mathrm{base})$。
- 構築後の 1 文字の遷移は $O(1)$。
