---
title: XorShift 乱数 (xorshift.hpp)
documentation_of: //include/gwen/utils/xorshift.hpp
---

# XorShift 乱数 (xorshift.hpp)

軽量かつ高速な擬似乱数生成器 (XorShift) を用いた乱数生成関数を提供します。
内部でグローバルな static 変数を使用してシードと状態を管理しているため、**スレッドセーフではありません**。

## rand64

```cpp
u64 rand64()
```

64ビットの乱数を生成します。

**制約**
- なし

**計算量**
- $O(1)$

## rand32

```cpp
u32 rand32()
u32 rand32(u32 r)
u32 rand32(u32 l, u32 r)
```

32ビットの乱数を生成します。
引数なしの場合は 32ビット全域の乱数を返します。
1引数 `r` の場合は $[0, r)$ の範囲の乱数を返します。
2引数 `l`, `r` の場合は $[l, r)$ の範囲の乱数を返します。

**制約**
- 2引数の場合、`l <= r` であること。

**計算量**
- 全て $O(1)$

## 使用例

```cpp
#include <iostream>
#include "gwen/utils/xorshift.hpp"

using namespace gwen;

int main() {
    // 64ビット乱数
    std::cout << rand64() << "\n";
    
    // 0 から 9 までの乱数
    std::cout << rand32(10) << "\n";
    
    // 10 から 20 までの乱数
    std::cout << rand32(10, 21) << "\n";
    
    return 0;
}
```
