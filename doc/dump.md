---
title: DUMP
documentation_of: //include/gwen/dump.hpp
---

# Dump

競技プログラミング向けのデバッグ出力用モジュールです。
C++23 の `std::format` の Range Formatting 機能を活用しており、標準コンテナや `std::pair`、`std::tuple` 等を追加コードなしで綺麗にダンプできます。

## マクロ

### DUMP

```cpp
#define DUMP(...)
```

指定された変数の名前とその値を標準エラー出力 (`std::cerr`) に出力します。
マクロ `LOCAL` が定義されている場合のみ動作します。提出時には `LOCAL` マクロを外すことで無効化されるため、デバッグ出力を消し忘れても TLE 等の原因になりません。

出力は `[vars]` (変数名) と `[dump]` (値) のセクションに分かれ、複数の変数を渡した場合は改行区切りで出力されます。ANSIエスケープシーケンスによる色付けが行われ、さらに `std::source_location` を用いて、DUMPを呼び出したファイル名と行番号も自動的に付与されるため視認性が非常に高くなっています。

### DUMP_SIZE_LIMIT

```cpp
#define DUMP_SIZE_LIMIT 10
```

`#include "gwen/dump.hpp"` よりも前に `DUMP_SIZE_LIMIT` マクロを定義することで、配列などの Range やデータ構造（FenwickTreeなど）を出力する際に、表示される要素数を制限することができます。要素数が制限を超えた場合は、中間の要素が `...` で省略され、末尾の要素が表示されます（例: `[1, 2, 3, ... , 10]`）。これにより、巨大なデータ構造をダンプする際のログの肥大化を防ぐことができます。

**制約**

- `LOCAL` マクロが定義されている場合のみ有効（未定義時はノーオペレーション）
- 引数は `std::formattable<T, char>`、`dumpable`、または `value_formattable` のいずれかを満たすこと（満たさない場合は `[unformattable token]` として出力される）

**計算量**

- $O(N)$（$N$: 渡した引数の数）

**使用例**
```cpp
int a = 1;
std::vector<int> v = {2, 3, 4};
DUMP(a, v);
/* 出力 (実際のコンソールでは色付き):
[gwen::DUMP @ main.cpp:26]
[vars]
a, v
[dump]
1,
[2, 3, 4]
*/
```

## メソッド

### dump

```cpp
template <typename... Args>
void dump(Args&&... args)
```

渡された引数の値をカンマ区切りで標準エラー出力 (`std::cerr`) に出力します。通常は `DUMP` マクロを介して使用します。

**制約**

- 各引数は `std::formattable<T, char>`、`dumpable`、または `value_formattable` のいずれかを満たすこと（満たさない場合は `[unformattable token]` として出力される）

**計算量**

- $O(N)$（$N$: 渡した引数の数）

## カスタム型のダンプ

標準で `std::formattable<T, char>` を満たす型（基本型やコンテナ、タプルなど）は自動的にフォーマットされます。
自作の構造体やクラスを出力したい場合は、以下のいずれかの方法で対応できます。

### 1. `dumpable` コンセプトを満たす
型に `std::string dump() const` メソッドを実装します。

```cpp
struct MyStruct {
    int x;
    std::string dump() const {
        return std::format("MyStruct(x={})", x);
    }
};
```

### 2. `value_formattable` コンセプトを満たす
型に `std::formattable` な値を返す `val() const` メソッドを実装します。`modint` のようなラッパークラスを想定しています。

```cpp
struct ModInt {
    int v;
    int val() const { return v; }
};
```

フォーマットに対応していない型を渡した場合はコンパイルエラーにはならず、安全に `[unformattable token]` として出力されます。

## 使用例

```cpp
// clang-format off
#define LOCAL // ローカル環境の想定
#include "gwen/dump.hpp"
// clang-format on

#include <vector>
#include <set>
#include <utility>
#include <tuple>

int main() {
    int x = 10;
    std::string s = "test";
    DUMP(x, s);
    /* 
    [gwen::DUMP @ main.cpp:14]
    [vars]
    x, s
    [dump]
    10,
    test
    */
    
    std::vector<int> v = {1, 2, 3};
    std::set<std::string> st = {"a", "b"};
    std::pair<int, double> p = {1, 2.5};
    std::tuple<int, std::string, double> t = {1, "test", 3.14};
    
    DUMP(v, st, p, t);
    /*
    [gwen::DUMP @ main.cpp:22]
    [vars]
    v, st, p, t
    [dump]
    [1, 2, 3],
    {"a", "b"},
    (1, 2.5),
    (1, "test", 3.14)
    */
    
    return 0;
}
```
