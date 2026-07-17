---
layout: default
title: テスト用ユーティリティ
---

# test/utils
このディレクトリには、ユニットテスト（特にランダムテスト）の実装を補助するためのユーティリティが含まれています。

## 1. `naive_array.hpp`

データ構造（セグメント木やBITなど）のランダムテストを行う際、**「愚直な $O(N)$ 計算で求めた正しい答え（Expected）」** を簡単に生成するためのクラス `NaiveArray<T>` を提供します。

### 使い方

```cpp
#include "utils/naive_array.hpp"
using namespace gwen::test;

// 要素数 10、初期値 0 で初期化
NaiveArray<int> naive(10, 0);

// 1点更新
naive.add(2, 5);     // a[2] += 5
naive.set(3, 10);    // a[3] = 10

// 区間更新 [l, r)
naive.range_add(1, 5, 2);    // a[1..4] += 2
naive.range_update(1, 5, 2); // a[1..4] = 2

// 区間クエリ [l, r)
int s = naive.sum(0, 5);           // a[0..4] の総和
int m = naive.min(0, 5, 1e9);      // 最小値
int x = naive.max(0, 5, -1e9);     // 最大値

// 任意の二項演算（例：XOR）
int xor_sum = naive.fold(0, 5, [](int a, int b) { return a ^ b; }, 0);
```

## 2. `random_seed.hpp`

`testlib.h` の乱数生成器 `rnd` を安全かつ便利に初期化するためのユーティリティです。

### 提供する機能
1. **ランダムシードの自動生成とログ出力**:
   実行ごとに異なる乱数シード（`std::random_device` ベース）を生成し、標準エラー出力にロギングします。テストが落ちた場合は、ログのシード値を指定して再実行することで完全に状況を再現できます。
2. **`testlib.h` と Google Test の競合回避**:
   インクルードするだけで、Google Test がテスト一覧を取得する際（`--gtest_list_tests`）に `testlib.h` が終了コードエラーを吐いてビルドを落とす問題（`testlibFinalizeGuard`）を自動的に回避します。

### 使い方

```cpp
#include <gtest/gtest.h>
#include "testlib.h"
#include "utils/random_seed.hpp"

TEST(MyAlgoTest, RandomTest) {
    // 基本的な使い方（ランダムシードを使用しコンソールに記録）
    test::setup_random_seed();
    
    // バグ再現のためにシードを固定したい場合
    // test::setup_random_seed(12345);
    
    int n = rnd.next(5, 40); // testlib の rnd を通常通り使用可能
}
```
