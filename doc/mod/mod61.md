---
title: ModInt61 (ローリングハッシュ用)
documentation_of: //include/gwen/mod/mod61.hpp
---

# ModInt61

$2^{61}-1$ を法とする高速なモジュラ演算を提供する構造体です。主にローリングハッシュなどのアルゴリズムで利用されます。
`gwen::modint` コンセプトを満たすため、他のモジュラ演算と同様に四則演算を透過的に行うことができます。

## コンストラクタ

```cpp
constexpr ModInt61()
template <std::integral T> constexpr ModInt61(T x)
```

- デフォルトコンストラクタは $0$ で初期化します。
- 整数 `x` から初期化する際、自動的に $0 \le \text{val} < 2^{61}-1$ の範囲に正規化されます。負の数からも初期化可能です。
- **制約**: `T` は 64-bit 以下の整数型であること (`static_assert` で検証されます)。

## メソッド

### val

```cpp
constexpr u64 val() const
```

現在の値を $0 \le x < 2^{61}-1$ の範囲で返します。

### mod

```cpp
static constexpr u64 mod()
```

法である $2^{61}-1$ を返します。

### inv

```cpp
constexpr ModInt61 inv() const
```

自身の逆元を返します。
内部ではフェルマーの小定理を用いて `pow(mod - 2)` を計算しています。

- **制約**: 自身の値が $0$ でないこと（実行時アサートで検査されます）。
- **計算量**: $O(\log(\text{mod}))$

### pow

```cpp
template <std::integral T> constexpr ModInt61 pow(T x) const
```

自身の `x` 乗を返します。

- **制約**: `x` は $0$ 以上であること。
- **計算量**: $O(\log x)$

## 演算子

通常の数値型と同様に、以下の演算子が使用可能です（いずれも `constexpr` に対応しています）。
- 算術演算子: `+`, `-`, `*`, `/`, `+=`, `-=`, `*=`, `/=`, 単項 `-`
- 比較演算子: `==`, `!=`
- 左辺が通常の整数型である場合の演算（例: `1 + m`）もサポートされています。
