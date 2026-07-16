---
title: 座標圧縮 (compress.hpp)
documentation_of: //include/gwen/utils/compress.hpp
---

# 座標圧縮 (compress.hpp)

配列内の要素の大小関係を保ったまま、値域を $0$ から始まるインデックスに変換する座標圧縮を提供します。
`std::ranges` を利用して簡潔に実装されています。

## compress

```cpp
template <typename T>
std::vector<i32> compress(const std::vector<T>& vec)
```

**制約**
- 戻り値のインデックスが `i32` に収まること（通常は元の要素数と同じ）。

**計算量**
- $O(N \log N)$ （ソートと二分探索にかかる時間）

## 使用例

```cpp
#include <iostream>
#include <vector>
#include "gwen/utils/compress.hpp"

using namespace gwen;

int main() {
    std::vector<int> v = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5};
    auto compressed = compress(v);
    
    // v = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5} の場合、
    // ソートされて重複削除された一意な値は {1, 2, 3, 4, 5, 6, 9}
    // したがって圧縮後は {2, 0, 3, 0, 4, 6, 1, 5, 4, 2, 4} となる
    for (i32 x : compressed) {
        std::cout << x << " ";
    }
    std::cout << "\n";
    return 0;
}
```
