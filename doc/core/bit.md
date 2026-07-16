---
title: ビット操作 (bit.hpp)
documentation_of: //include/gwen/core/bit.hpp
---

# ビット操作

整数の各ビットにアクセスしたり変更したりする補助関数を提供します。

## **getBit**

```cpp
template <typename T1, typename T2>
T1 getBit(T1 bit, T2 i)
```

`bit` の `i` 番目のビットを取得します。

**制約**

- $0 \le i <$ (T1のビット幅)

**計算量**

- $O(1)$

## **setBit**

```cpp
template <typename T1, typename T2>
T1 setBit(T1 bit, T2 i)
```

`bit` の `i` 番目のビットを 1 にした値を返します。

**制約**

- $0 \le i <$ (T1のビット幅)

**計算量**

- $O(1)$

## **clearBit**

```cpp
template <typename T1, typename T2>
T1 clearBit(T1 bit, T2 i)
```

`bit` の `i` 番目のビットを 0 にした値を返します。

**制約**

- $0 \le i <$ (T1のビット幅)

**計算量**

- $O(1)$

## **toggleBit**

```cpp
template <typename T1, typename T2>
T1 toggleBit(T1 bit, T2 i)
```

`bit` の `i` 番目のビットを反転した値を返します。

**制約**

- $0 \le i <$ (T1のビット幅)

**計算量**

- $O(1)$
