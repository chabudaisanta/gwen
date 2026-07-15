---
title: 統合テンプレート (template.hpp)
documentation_of: //include/gwen/template.hpp
---

# 統合テンプレート

競技プログラミング向けの統合インクルード用ヘッダーです。これ一つを include するだけで、各種ユーティリティ、マクロ、入出力補助、アルゴリズム関数などが使えるようになります。

## インクルード内容
- `<bits/stdc++.h>`
- `gwen::types` (i32, i64, u32, u64 など)
- `gwen::literals`
- 各モジュール (`core`, `math`, `io`, `algo`, `ds`)

## 使用例
```cpp
#include "gwen/template.hpp"

int main() {
    std::vector<int> a = {1, 2, 3};
    std::cout << a << EL;
    return 0;
}
```
