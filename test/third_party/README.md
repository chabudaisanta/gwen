---
layout: default
title: testlib.h の使い方
---

# testlib.h の使い方
`testlib.h` は競技プログラミング（Codeforcesなど）で広く使われている、テストジェネレータ・チェッカー用のC++ライブラリです。
本プロジェクトでは、アルゴリズムのランダムテストを実装する際の乱数生成器として利用します。

## インクルード方法

`test/unit/CMakeLists.txt` でインクルードパスを設定しているため、各テストファイルからは直接ファイル名でインクルードできます。

```cpp
#include <gtest/gtest.h>
#include "testlib.h"
#include "utils/random_seed.hpp"
```

## 初期化（Google Test内で使う場合）

```cpp
TEST(MyAlgoTest, RandomTest) {
    // std::random_device 経由のランダムシードで初期化しつつ、コンソールにシードを出力する
    test::setup_random_seed();
    
    // バグ等で特定のシードを再現したい場合は固定値を渡す
    // test::setup_random_seed(12345);
    
    // ランダムテストの実行...
}
```

※ または、ダミーの引数を作って `registerGen` を呼ぶことも可能です。

```cpp
TEST(MyAlgoTest, RandomTestWithRegister) {
    char* argv[] = {(char*)"test", (char*)"12345"};
    registerGen(2, argv, 1);
}
```

## よく使う乱数生成メソッド

`testlib.h` の `rnd` オブジェクトには、テストケース生成に便利なメソッドが多数用意されています。

### 1. 整数・小数の生成
- `rnd.next(n)` : `[0, n-1]` の範囲の整数をランダムに生成
- `rnd.next(l, r)` : `[l, r]` の範囲の整数をランダムに生成
- `rnd.next(0.0, 1.0)` : `[0.0, 1.0)` の範囲の小数をランダムに生成

### 2. 偏りのある乱数生成 (wnext)
引数 `w` を指定することで、複数回乱数を振ったときの「最大値」または「最小値」を採用するような偏った乱数を作れます。
- `rnd.wnext(n, w)` : `w > 0` の場合、`w` 回振った最大値。`w < 0` の場合は `|w|` 回振った最小値。
- `rnd.wnext(l, r, w)` : `[l, r]` の範囲での偏り付き乱数。

※ コーナーケース（非常に小さい配列、非常に大きい値など）を引き当てやすくするのに便利です。

### 3. 文字列の生成
正規表現風のパターンを指定して、それにマッチする文字列をランダムに生成できます。
- `rnd.next("[a-z]{5,10}")` : 小文字のアルファベットからなる、長さ5〜10の文字列

### 4. 順列・配列からの選択
- `rnd.perm(n)` : `0` から `n-1` までの数値がランダムに並んだ `std::vector<int>` を生成（例：木の頂点のシャッフルなどに使用）
- `rnd.any(vec)` : `std::vector` `vec` の中からランダムに1要素を選択

---
公式リポジトリおよび詳細なドキュメント：
https://github.com/MikeMirzayanov/testlib
