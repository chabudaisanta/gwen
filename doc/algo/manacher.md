---
title: Manacher (manacher.hpp)
documentation_of: //include/gwen/algo/manacher.hpp
---

# Manacher

列中の各中心について最長回文の半径を求めます。

## manacher

```cpp
template <typename Range>
std::pair<std::vector<i32>, std::vector<i32>> gwen::manacher(const Range& s);
```

戻り値を `{odd, even}` とすると、`odd[i]` は `s[i]` を中心とする奇数長回文の半径で、回文長は `2 * odd[i] - 1` です。`even[i]` は `s[i-1]` と `s[i]` の間を中心とする偶数長回文の半径で、回文長は `2 * even[i]` です。`even[0]` は `0` です。空の列では、両方とも空の配列を返します。

**制約**

- `const Range` はサイズを取得できるランダムアクセス範囲で、要素参照は等値比較可能であること。
- `s.size() <= INT32_MAX`。

**計算量**

- $N = |s|$ とすると、時間・使用領域ともに $O(N)$。
