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

**使用例**
```cpp
int a = 1;
std::vector<int> v = {2, 3, 4};
DUMP(a, v);
// 出力: a, v: 1, [2, 3, 4]
```

## メソッド

### dump

```cpp
template <typename... Args>
void dump(Args&&... args)
```

渡された引数の値をカンマ区切りで標準エラー出力 (`std::cerr`) に出力します。通常は `DUMP` マクロを介して使用します。

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
#include <gwen/dump.hpp>
// clang-format on

#include <vector>
#include <set>
#include <utility>
#include <tuple>

int main() {
    int x = 10;
    std::string s = "test";
    DUMP(x, s);
    // x, s: 10, test
    
    std::vector<int> v = {1, 2, 3};
    std::set<std::string> st = {"a", "b"};
    std::pair<int, double> p = {1, 2.5};
    std::tuple<int, std::string, double> t = {1, "test", 3.14};
    
    DUMP(v, st, p, t);
    // v, st, p, t: [1, 2, 3], {"a", "b"}, (1, 2.5), (1, "test", 3.14)
    
    return 0;
}
```
