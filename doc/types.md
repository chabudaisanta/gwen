---
title: types
documentation_of: //include//gwen/types.hpp
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
