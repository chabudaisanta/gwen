# modint

実行時に法 (mod) を設定できる自動 Mod 演算構造体 `gwen::dynamic_modint64` を提供します。Montgomery乗算を用いており、高速な演算が可能です。

## ヘッダファイル

```cpp
#include "gwen/mod/modint.hpp"
```

## 概要

`dynamic_modint64` は 64-bit 整数までの法を設定して、加算・減算・乗算を自動的に mod 上で行います。

> [!WARNING]
> 現状の実装では法 `n` は `static` 変数として 1 つだけ保持されます。同時に異なる法を用いることはできず、スレッドセーフでもありません。

## 使い方

### 法の設定
演算を行う前に必ず法を設定してください。法は奇数である必要があります。

```cpp
gwen::dynamic_modint64::set_mod(MOD);
```

### インスタンス化と演算
```cpp
using mint = gwen::dynamic_modint64;

mint a = 10;
mint b = -5; // 負の値も自動的に正の mod に変換されます

mint c = a + b;
mint d = a * b;
mint e = a - 100;

a += 5;
b *= 2;
```

### 値の取得
`val()` メソッドを用いて生の `u64` 整数値を取得できます。

```cpp
u64 ans = c.val();
```

### 累乗 (べき乗)
`pow(x)` は $O(\log x)$ で $a^x \pmod{\text{MOD}}$ を計算します。

```cpp
mint res = a.pow(1000000000);
```
