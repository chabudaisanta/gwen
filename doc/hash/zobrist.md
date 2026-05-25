# Zobrist Hash

集合（または多重集合）のハッシュを、要素の追加・削除ごとに $O(1)$ で更新するデータ構造です。状態の同一性判定やハッシュテーブルのキーに使います。

---

## テンプレート引数

- `T`: 要素の型。`std::hash<T>` でハッシュ可能であること。
- `isMultiset`: `false` のときは集合（XOR）、`true` のときは多重集合（加算）でハッシュを更新します。

---

## コンストラクタ

```
zobrist<T, isMultiset> z;
```

空の集合で構築します。

---

## size

```
i32 z.size() const;
```

現在の要素数を返します。

---

## empty

```
bool z.empty() const;
```

要素が 0 個なら `true`、そうでなければ `false` を返します。

---

## get

```
u64 z.get() const;
```

現在の集合（または多重集合）のハッシュ値を返します。

---

## insert

```
void z.insert(const T& x);
```

要素 `x` を 1 つ追加し、ハッシュを更新します。

- 集合モード: ハッシュに `hash(x)` を XOR します。
- 多重集合モード: ハッシュに `hash(x)` を加算します。

**計算量**

- $O(1)$

---

## erase

```
void z.erase(const T& x);
```

要素 `x` を 1 つ削除し、ハッシュを更新します。多重集合モードでは対応する加算の逆を行います。`size` が 0 未満にならないことを呼び出し側で保証する必要があります。

**計算量**

- $O(1)$

---

## operator== / operator!=

```
bool z == other;
bool z != other;
```

サイズとハッシュ値が等しいかどうかを比較します。

---

## 使用例

```cpp
#include "gwen/hash/zobrist.hpp"

using namespace gwen::hash;

zobrist<i32> z;  // 集合
z.insert(1);
z.insert(2);
z.get();
z.erase(1);

zobrist<i32, true> zm;  // 多重集合
zm.insert(1);
zm.insert(1);
zm.erase(1);
```
