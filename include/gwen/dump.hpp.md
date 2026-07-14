---
data:
  attributes:
    '*NOT_SPECIAL_COMMENTS*': ''
    links: []
  dependencies:
  - files:
    - filename: types.hpp
      icon: LIBRARY_NO_TESTS
      path: include/gwen/types.hpp
      title: types
    type: Depends on
  - files: []
    type: Required by
  - files: []
    type: Verified with
  dependsOn:
  - include/gwen/types.hpp
  documentPath: doc/dump.md
  embedded:
  - code: "#pragma once\n\n#include <iostream>\n#include <concepts>\n#include <format>\n\
      #include <string>\n#include <string_view>\n\n#include \"gwen/types.hpp\"\n\n\
      namespace gwen {\n\nnamespace internal {\n\ntemplate<typename... Args>\nconstexpr\
      \ bool is_empty_args(Args&&... args) {\n    return sizeof...(args) == 0;\n}\
      \ \n\ntemplate<typename T>\nconcept dumpable = requires(const T& t) {\n    {\
      \ t.dump() } -> std::convertible_to<std::string>;\n};\n\ntemplate<typename T>\n\
      concept value_formattable = requires(const T& t) {\n    { t.val() } -> std::formattable<char>;\n\
      };\n\nusize length_of_string_view(std::string_view sv) {\n    return sv.size();\n\
      }\n\n} // namespace internal\n\ntemplate<typename... Args>\nvoid dump(Args&&...\
      \ args) {\n    auto f = [](auto&& arg) {\n        using T = std::remove_cvref_t<decltype(arg)>;\n\
      \        if constexpr (internal::dumpable<T>) {\n            return arg.dump();\n\
      \        }\n        else if constexpr (internal::value_formattable<T>) {\n \
      \           return std::format(\"{}\", arg.val());\n        }\n        else\
      \ if constexpr (std::formattable<T,char>) {\n            return std::format(\"\
      {}\", arg);\n        }\n        else {\n            return \"[unformattable\
      \ token]\";\n        }\n    };\n    usize cnt = 0;\n    auto sz = sizeof...(args);\n\
      \    ((std::cerr << f(args) << (++cnt < sz ? \", \" : \"\\n\")), ...);\n}\n\n\
      #ifdef LOCAL\n#define DUMP(...) \\\n    do { \\\n        if constexpr (::gwen::internal::is_empty_args(__VA_ARGS__))\
      \ {\\\n            std::cerr << \"empty dump called\\n\";\\\n        } else\
      \ {\\\n            std::cerr << #__VA_ARGS__ << \": \";\\\n            if (15\
      \ <= ::gwen::internal::length_of_string_view(#__VA_ARGS__)) std::cerr << \"\\\
      n    \";\\\n            ::gwen::dump(__VA_ARGS__);\\\n        }\\\n    } while(0)\n\
      #else\n#define DUMP(...) do{}while(0)\n#endif\n} // namespace gwen"
    name: default
  - code: "Traceback (most recent call last):\n  File \"/opt/hostedtoolcache/Python/3.11.15/x64/lib/python3.11/site-packages/competitive_verifier/oj/resolver.py\"\
      , line 290, in resolve\n    bundled_code = language.bundle(path, basedir=basedir)\n\
      \                   ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n  File \"/opt/hostedtoolcache/Python/3.11.15/x64/lib/python3.11/site-packages/competitive_verifier/oj/languages/cplusplus.py\"\
      , line 243, in bundle\n    bundler.update(path)\n  File \"/opt/hostedtoolcache/Python/3.11.15/x64/lib/python3.11/site-packages/competitive_verifier/oj/languages/cplusplus_bundle.py\"\
      , line 479, in update\n    self._resolve(pathlib.Path(included), included_from=path)\n\
      \  File \"/opt/hostedtoolcache/Python/3.11.15/x64/lib/python3.11/site-packages/competitive_verifier/oj/languages/cplusplus_bundle.py\"\
      , line 286, in _resolve\n    raise BundleErrorAt(path, -1, \"no such header\"\
      )\ncompetitive_verifier.oj.languages.cplusplus_bundle.BundleErrorAt: gwen/types.hpp:\
      \ line -1: no such header\n"
    name: bundle error
  isFailed: false
  isVerificationFile: false
  path: include/gwen/dump.hpp
  pathExtension: hpp
  requiredBy: []
  timestamp: '2026-07-14 19:46:59+09:00'
  title: DUMP
  verificationStatus: LIBRARY_NO_TESTS
  verifiedWith: []
documentation_of: include/gwen/dump.hpp
layout: document
title: DUMP
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