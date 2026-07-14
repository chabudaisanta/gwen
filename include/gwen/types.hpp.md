---
data:
  attributes:
    '*NOT_SPECIAL_COMMENTS*': ''
    links: []
  dependencies:
  - files: []
    type: Depends on
  - files: []
    type: Required by
  - files: []
    type: Verified with
  dependsOn: []
  documentPath: doc/types.md
  embedded:
  - code: "#pragma once\n\n#include <cstddef>\n#include <cstdint>\n\nnamespace gwen\
      \ {\n\n// \u7B26\u53F7\u3042\u308A\u6574\u6570\u578B\nusing i32 = std::int32_t;\n\
      using i64 = std::int64_t;\nusing i128 = __int128_t;\n\n// \u7B26\u53F7\u306A\
      \u3057\u6574\u6570\u578B\nusing u32 = std::uint32_t;\nusing u64 = std::uint64_t;\n\
      using u128 = __uint128_t;\n\n// \u30DD\u30A4\u30F3\u30BF\u30B5\u30A4\u30BA\u30FB\
      \u30B3\u30F3\u30C6\u30CA\u30B5\u30A4\u30BA\u578B\nusing usize = std::size_t;\n\
      using isize = std::ptrdiff_t;\n\n// \u6D6E\u52D5\u5C0F\u6570\u70B9\u578B\nusing\
      \ f32 = float;\nusing f64 = double;\nusing f80 = long double;\n\n// \u30E6\u30FC\
      \u30B6\u30FC\u5B9A\u7FA9\u30EA\u30C6\u30E9\u30EB\nnamespace literals {\n\nconstexpr\
      \ i64 operator\"\"_i64(unsigned long long n) { return static_cast<i64>(n); }\n\
      constexpr u64 operator\"\"_u64(unsigned long long n) { return static_cast<u64>(n);\
      \ }\nconstexpr usize operator\"\"_zu(unsigned long long n) { return static_cast<usize>(n);\
      \ }\n\n}  // namespace literals\n\n}  // namespace gwen\n"
    name: default
  - code: "#line 2 \"include/gwen/types.hpp\"\n\n#include <cstddef>\n#include <cstdint>\n\
      \nnamespace gwen {\n\n// \u7B26\u53F7\u3042\u308A\u6574\u6570\u578B\nusing i32\
      \ = std::int32_t;\nusing i64 = std::int64_t;\nusing i128 = __int128_t;\n\n//\
      \ \u7B26\u53F7\u306A\u3057\u6574\u6570\u578B\nusing u32 = std::uint32_t;\nusing\
      \ u64 = std::uint64_t;\nusing u128 = __uint128_t;\n\n// \u30DD\u30A4\u30F3\u30BF\
      \u30B5\u30A4\u30BA\u30FB\u30B3\u30F3\u30C6\u30CA\u30B5\u30A4\u30BA\u578B\nusing\
      \ usize = std::size_t;\nusing isize = std::ptrdiff_t;\n\n// \u6D6E\u52D5\u5C0F\
      \u6570\u70B9\u578B\nusing f32 = float;\nusing f64 = double;\nusing f80 = long\
      \ double;\n\n// \u30E6\u30FC\u30B6\u30FC\u5B9A\u7FA9\u30EA\u30C6\u30E9\u30EB\
      \nnamespace literals {\n\nconstexpr i64 operator\"\"_i64(unsigned long long\
      \ n) { return static_cast<i64>(n); }\nconstexpr u64 operator\"\"_u64(unsigned\
      \ long long n) { return static_cast<u64>(n); }\nconstexpr usize operator\"\"\
      _zu(unsigned long long n) { return static_cast<usize>(n); }\n\n}  // namespace\
      \ literals\n\n}  // namespace gwen\n"
    name: bundled
  isFailed: false
  isVerificationFile: false
  path: include/gwen/types.hpp
  pathExtension: hpp
  requiredBy: []
  timestamp: '2026-07-14 20:58:01+09:00'
  title: types
  verificationStatus: LIBRARY_NO_TESTS
  verifiedWith: []
documentation_of: include/gwen/types.hpp
layout: document
title: types
---
# types.hpp

C++の標準型を競技プログラミング向けに短く書けるようにしたエイリアスと、型推論を容易にするユーザー定義リテラルを提供するヘッダーです。

## インクルード

```cpp
#include "gwen/types.hpp"
```

## 型エイリアス

### 符号あり整数
- `i32`: `std::int32_t`
- `i64`: `std::int64_t`
- `i128`: `__int128_t` (環境依存)

### 符号なし整数
- `u32`: `std::uint32_t`
- `u64`: `std::uint64_t`
- `u128`: `__uint128_t` (環境依存)

### サイズ・ポインタ差分
- `usize`: `std::size_t`
- `isize`: `std::ptrdiff_t`

### 浮動小数点
- `f32`: `float`
- `f64`: `double`
- `f80`: `long double`

## ユーザー定義リテラル

`gwen::literals` 名前空間を展開することで、リテラルにサフィックスをつけて特定の型として評価できます。`auto` を用いた型推論時に便利です。

```cpp
using namespace gwen::literals;

auto a = 1_i64; // i64 型の 1
auto b = 42_u64; // u64 型の 42
auto c = 100_zu; // usize 型の 100
```