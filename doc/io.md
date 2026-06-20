# io

競技プログラミング向けの高速な入出力 (Fast I/O) を提供します。標準の `std::cin` / `std::cout` を完全に置き換えることができます。

## ヘッダファイル

```cpp
#include "gwen/io.hpp"
```

## 概要

`gwen::input` および `gwen::output` というグローバルインスタンスが定義されており、これらに対して `>>` や `<<` 演算子を用いて入出力を行います。

内部で大きなバッファ (1MB) を確保し、`fread` / `fwrite` を用いて一括で読み書きを行うため非常に高速です。

## 入力 (`gwen::input`)

`>>` 演算子を用いて様々な型を読み込むことができます。空白文字 (スペースや改行) は自動的にスキップされます。

```cpp
int n;
long long m;
std::string s;

// 基本的な型の読み込み
gwen::input >> n >> m >> s;
```

### 対応している型
- 符号付き/符号なしの整数型 (`i32`, `u32`, `i64`, `u64`, `i128`, `u128` などの組み込み整数)
- `char`, `std::string`
- `std::vector<T>` (要素型 `T` が読み込み対応している場合、サイズ分だけ連続で読み込みます)
- `gwen::mdarray<T, Rank>` (配列全体を連続で読み込みます)

## 出力 (`gwen::output`)

`<<` 演算子を用いて値を出力します。

```cpp
int a = 10;
std::string msg = "Hello";

// 基本的な出力
gwen::output << a << " " << msg << '\n';
```

### 対応している型
- 符号付き/符号なしの整数型 (`i32`, `u32`, `i64`, `u64` など)
- `char`, `const char*`, `std::string`
- `std::vector<T>` (要素同士を空白区切りで出力します)

### バッファのフラッシュ

出力はプログラム終了時に自動的にフラッシュされますが、インタラクティブ問題などで強制的にフラッシュしたい場合は `write_to_file(true)` を呼び出します。

```cpp
// 出力バッファを stdout に書き出し、fflush する
gwen::output.write_to_file(true);
```
