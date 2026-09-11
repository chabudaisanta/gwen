---
title: 過半数要素 (majority_vote.hpp)
documentation_of: //include/gwen/algo/majority_vote.hpp
---

# 過半数要素

配列中に出現回数が半数を真に超える要素があるかを判定する関数を提供します。

## majority_vote

```cpp
template <std::copy_constructible T>
    requires std::assignable_from<T&, const T&> && std::equality_comparable<T>
std::pair<bool, T> majority_vote(const std::vector<T>& values)
```

`values` に過半数要素が存在するとき、`{true, その要素}` を返します。存在しないときは `{false, 候補}` を返し、このとき候補の値は意味を持ちません。

**制約**

- `values` は空でないこと。
- `T` はコピー構築、`const T&` からの代入、等値比較が可能であること。

**計算量**

- $O(N)$（$N$ は `values` のサイズ）
