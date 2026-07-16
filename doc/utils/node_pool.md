---
title: ノードプール (node_pool.hpp)
documentation_of: //include/gwen/utils/node_pool.hpp
---

# ノードプール (node_pool.hpp)

競技プログラミングなどにおいて、動的メモリ確保（`new` / `delete`）のオーバーヘッドを避けるため、静的配列（`std::vector`）を用いてノードのメモリを管理するためのプールです。
解放されたノードの再利用機能（`free_node`）も提供します。

## NodePool

```cpp
template <typename Node>
class NodePool
```

ノードを管理するメモリプールクラスです。

## コンストラクタ

```cpp
explicit NodePool()
```

プールの最初の要素（ID=0）にダミーノードを配置して初期化します。これにより、ID=0をNULL扱いとしたり、1-indexedで管理することが可能です。

**制約**
- なし

**計算量**
- $O(1)$

## new_node

```cpp
i32 new_node(Node&& node)
i32 new_node(const Node& node)
```

新しいノードをプールに確保し、そのIDを返します。
内部の `free_list` が空でない場合は、過去に解放されたIDを再利用します。

**制約**
- なし

**計算量**
- 償却 $O(1)$

## free_node

```cpp
void free_node(i32 id)
```

指定したIDのノードを解放し、再利用可能な状態にします。
参照やリソースのリークを防ぐため、解放時にそのノードはデフォルト構築状態にリセットされます。

**制約**
- `id` はプール内の有効なノードIDであり、**すでに解放済みでないこと**（二重解放しないこと）。
- `id` は `0` より大きい有効なノードIDであること（ID=0 のダミーノードは解放不可）。

**計算量**
- $O(1)$

## size

```cpp
i32 size() const
```

プール内の有効な総ノード数（ダミーノードを含む）を返します。

**制約**
- なし

**計算量**
- $O(1)$

## operator[]

```cpp
Node& operator[](i32 id)
const Node& operator[](i32 id) const
```

指定したIDのノードにアクセスします。

**制約**
- `id >= 0` かつ `id < pool.size()`

**計算量**
- $O(1)$

## 使用例

```cpp
#include <iostream>
#include "gwen/utils/node_pool.hpp"

using namespace gwen;

struct MyNode {
    int val = 0;
};

int main() {
    NodePool<MyNode> pool;
    
    i32 id1 = pool.new_node(MyNode{10});
    i32 id2 = pool.new_node(MyNode{20});
    
    std::cout << pool[id1].val << "\n"; // 10
    std::cout << pool[id2].val << "\n"; // 20
    
    // ノードを解放
    pool.free_node(id1);
    
    // 再利用される
    i32 id3 = pool.new_node(MyNode{30});
    std::cout << (id1 == id3) << "\n";  // 1 (true)
    
    return 0;
}
```
