# Trie (trie_base)

文字列（または一般の列）の集合を管理するトライ木です。ノードは `node_pool` で管理され、`tree` 型（実体は `i32`）がノード ID を表します。

- 文字の型は `i32(x) - Offset` が $[0, \mathrm{Width})$ に収まる必要があります（例: `'a'` なら `Offset = 'a'`, `Width = 26`）。
- 同じ文字列を複数回 insert 可能（多重集合として扱う場合、`count` がその回数になります）。

---

## テンプレート引数

- `Node`: ノードの型（例: `trie_node_minimum<Width>`）。少なくとも `par`, `ch[Width]`, `cnt`, `subtree_cnt`, `idx` を持つこと。
- `Width`: 文字（子の枝）の種類数。
- `Offset`: 文字をインデックスに変換するときのオフセット（`i = i32(x) - Offset` が $[0, \mathrm{Width})$ に入るようにする）。

---

## コンストラクタ

```
trie_base<Node, Width, Offset> t;
```

空のトライで構築します。

**計算量**

- $O(1)$

---

## find

```
tree t.find(const Container& str) const;
```

文字列 `str` に対応するノード ID を返します。見つからない場合は `NIL`（0）を返します。

**計算量**

- $O(|\mathrm{str}|)$

---

## count

```
i32 t.count(const Container& str) const;
```

`str` を**完全一致**で挿入した回数（そのノードの `cnt`）を返します。

**計算量**

- $O(|\mathrm{str}|)$

---

## contains

```
bool t.contains(const Container& str) const;
```

`str` が 1 回以上挿入されていれば `true`、そうでなければ `false` を返します。`count(str) != 0` と同じです。

**計算量**

- $O(|\mathrm{str}|)$

---

## match_count

```
i32 t.match_count(const Container& str) const;
```

`str` を**接頭辞**として持つ文字列の総数（そのノードの `subtree_cnt`）を返します。

**計算量**

- $O(|\mathrm{str}|)$

---

## get_node

```
Node& t.get_node(const Container& str);
Node& t.get_node(tree id);
```

- `get_node(str)`: 文字列 `str` に対応するノードへの参照を返します。`str` が存在しない場合は未定義動作です。
- `get_node(id)`: ノード ID `id` のノードへの参照を返します。

**制約**

- `get_node(str)`: `find(str) != NIL` であること。

---

## insert

```
tree t.insert(const Container& str);
```

文字列 `str` を 1 回挿入し、対応するノード ID を返します。

**計算量**

- $O(|\mathrm{str}|)$

---

## erase

```
void t.erase(const Container& str, i32 n = 1);
```

- `n > 0`: `str` の挿入数を `n` 回分減らします（最大で現在の `count(str)` まで）。
- `n <= 0`: `str` の挿入をすべて削除します。

**計算量**

- $O(|\mathrm{str}|)$

---

## path

```
vector<tree> t.path(const Container& str) const;
```

根から `str` を辿ったときのノード ID の列を返します。

- `path(str)[0]` = 根
- `path(str)[i]` = `str[0..i)` を辿った先のノード
- 長さは `min(len(str), マッチした長さ) + 1` です。途中で NIL に達したらそれ以降は NIL で打ち切られます。

**計算量**

- $O(|\mathrm{str}|)$

---

## 使用例

```cpp
#include "gwen/container/trie.hpp"

using namespace gwen;

using trie_t = trie_base<trie_node_minimum<26>, 26, 'a'>;
trie_t t;
t.insert("abc");
t.insert("abc");
t.insert("ab");
t.count("abc");      // 2
t.match_count("ab"); // 3
t.contains("a");     // false（"a" は挿入していない）
t.path("ab");        // [root, node_a, node_ab]
```
