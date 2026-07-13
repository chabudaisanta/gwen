---
title: Timer
documentation_of: //include/gwen/misc/timer.hpp
---

# Timer

実行時間の計測や、競技プログラミングにおける焼きなまし法等の「制限時間」を管理するためのユーティリティクラスです。
`std::chrono` をラップしており、任意の時間単位（ミリ秒、マイクロ秒など）で手軽に時間を取得できます。

> [!TIP]
> **グローバルインスタンスの利用**
> 本ライブラリでは、競技プログラミングのテンプレートとしてすぐに使えるよう、ヘッダ内で `inline Timer timer;` としてあらかじめグローバルインスタンスが定義されています。
> 基本的にはこの `gwen::timer` をそのまま操作することが推奨されます（用途に合わせて `Timer t;` のようにローカルに複数作成することも可能です）。

クラス内に以下の時間単位のエイリアスが用意されています。
- `Timer::s` : 秒 (`std::chrono::seconds`)
- `Timer::ms` : ミリ秒 (`std::chrono::milliseconds`)
- `Timer::us` : マイクロ秒 (`std::chrono::microseconds`)

## コンストラクタ

```cpp
Timer()
```

- タイマーを構築し、全体の計測を開始します。デフォルトの制限時間（`limit`）は未設定（最大値）となります。

**計算量**

- $O(1)$

## elapsed

```cpp
template <typename Unit> Unit timer.elapsed() const
```

タイマーが構築されてからの**全体の経過時間**を指定した時間単位で返します。

**計算量**

- $O(1)$

## delta

```cpp
template <typename Unit> Unit timer.delta() const
```

直近で `reset()` または `lap()` を呼び出した時刻（構築直後は構築時刻）からの**経過時間（ラップタイム）**を指定した時間単位で返します。

**計算量**

- $O(1)$

## lap

```cpp
template <typename Unit> Unit timer.lap()
```

現在の `delta()` を返しつつ、ラップタイム測定用の基準時刻をリセット（`reset()`）します。

**計算量**

- $O(1)$

## reset

```cpp
void timer.reset()
```

ラップタイム測定用の基準時刻を「現在時刻」にリセットします。全体の経過時間（`elapsed`）の基準時刻には影響しません。

**計算量**

- $O(1)$

## set_limit

```cpp
void timer.set_limit(duration target)
```

タイマーの制限時間を設定します。

**計算量**

- $O(1)$

## restart

```cpp
void timer.restart(duration target)
```

ラップタイムをリセット（`reset()`）し、同時に新たな制限時間を設定します。

**計算量**

- $O(1)$

## expired

```cpp
bool timer.expired() const
```

前回の `reset()` や `lap()` などのラップタイム基準時刻からの経過時間が、設定された制限時間を超過している場合に `true` を返します。

**計算量**

- $O(1)$

## expired_elapsed

```cpp
bool timer.expired_elapsed() const
```

タイマーが構築されてからの全体経過時間が、設定された制限時間を超過している場合に `true` を返します。

**計算量**

- $O(1)$

## dump

```cpp
std::string timer.dump() const
```

現在の全体の経過時間、ラップタイム、および制限時間を改行区切りの文字列として出力します。

**計算量**

- $O(1)$

## 使用例

```cpp
#include <iostream>
#include "gwen/misc/timer.hpp"

using namespace gwen;

int main() {
    timer.set_limit(Timer::ms(2000));
    
    while (!timer.expired_elapsed()) {
        // 制限時間内であればループを回す
    }

    timer.reset();
    auto time_taken = timer.delta<Timer::ms>().count();
    std::cout << time_taken << " ms" << std::endl;
}
```
