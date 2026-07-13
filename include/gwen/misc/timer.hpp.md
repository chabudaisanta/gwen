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
  documentPath: doc/misc/timer.md
  embedded:
  - code: "#pragma once\n\n#include <cassert>\n#include <chrono>\n#include <format>\n\
      #include <string>\n\nnamespace gwen {\n\n/**\n * @brief \u30B7\u30F3\u30D7\u30EB\
      \u306A\u30BF\u30A4\u30DE\u30FC\u30E6\u30FC\u30C6\u30A3\u30EA\u30C6\u30A3\n */\n\
      struct Timer {\npublic:\n    using clock = std::chrono::steady_clock;\n    using\
      \ duration = clock::duration;\n    using timepoint = clock::time_point;\n  \
      \  using s = std::chrono::seconds;\n    using ms = std::chrono::milliseconds;\n\
      \    using us = std::chrono::microseconds;\n\nprivate:\n    const timepoint\
      \ start = clock::now();\n    timepoint latest = start;\n    duration limit =\
      \ duration::max();\n\npublic:\n    Timer() = default;\n\n    template <typename\
      \ Unit> Unit elapsed() const { return std::chrono::duration_cast<Unit>(elapsed_raw());\
      \ }\n    template <typename Unit> Unit delta() const { return std::chrono::duration_cast<Unit>(delta_raw());\
      \ }\n    template <typename Unit> Unit lap() {\n        auto res = delta<Unit>();\n\
      \        reset();\n        return res;\n    }\n\n    void reset() { latest =\
      \ clock::now(); }\n    void set_limit(duration target) { limit = target; }\n\
      \    void restart(duration target) {\n        reset();\n        set_limit(target);\n\
      \    }\n    bool expired() const {\n        assert(limit < duration::max());\n\
      \        return limit <= delta_raw();\n    }\n    bool expired_elapsed() const\
      \ {\n        assert(limit < duration::max());\n        return limit <= elapsed_raw();\n\
      \    }\n\n    std::string dump() const {\n        return std::format(\"elapsed:\
      \ {}ms\\ndelta: {}ms\\nlimit: {}ms\\n\", elapsed<ms>().count(), delta<ms>().count(),\n\
      \                           std::chrono::duration_cast<ms>(limit).count());\n\
      \    }\n\nprivate:\n    duration elapsed_raw() const { return clock::now() -\
      \ start; }\n    duration delta_raw() const { return clock::now() - latest; }\n\
      };\ninline Timer timer;\n\n}  // namespace gwen"
    name: default
  - code: "#line 2 \"include/gwen/misc/timer.hpp\"\n\n#include <cassert>\n#include\
      \ <chrono>\n#include <format>\n#include <string>\n\nnamespace gwen {\n\n/**\n\
      \ * @brief \u30B7\u30F3\u30D7\u30EB\u306A\u30BF\u30A4\u30DE\u30FC\u30E6\u30FC\
      \u30C6\u30A3\u30EA\u30C6\u30A3\n */\nstruct Timer {\npublic:\n    using clock\
      \ = std::chrono::steady_clock;\n    using duration = clock::duration;\n    using\
      \ timepoint = clock::time_point;\n    using s = std::chrono::seconds;\n    using\
      \ ms = std::chrono::milliseconds;\n    using us = std::chrono::microseconds;\n\
      \nprivate:\n    const timepoint start = clock::now();\n    timepoint latest\
      \ = start;\n    duration limit = duration::max();\n\npublic:\n    Timer() =\
      \ default;\n\n    template <typename Unit> Unit elapsed() const { return std::chrono::duration_cast<Unit>(elapsed_raw());\
      \ }\n    template <typename Unit> Unit delta() const { return std::chrono::duration_cast<Unit>(delta_raw());\
      \ }\n    template <typename Unit> Unit lap() {\n        auto res = delta<Unit>();\n\
      \        reset();\n        return res;\n    }\n\n    void reset() { latest =\
      \ clock::now(); }\n    void set_limit(duration target) { limit = target; }\n\
      \    void restart(duration target) {\n        reset();\n        set_limit(target);\n\
      \    }\n    bool expired() const {\n        assert(limit < duration::max());\n\
      \        return limit <= delta_raw();\n    }\n    bool expired_elapsed() const\
      \ {\n        assert(limit < duration::max());\n        return limit <= elapsed_raw();\n\
      \    }\n\n    std::string dump() const {\n        return std::format(\"elapsed:\
      \ {}ms\\ndelta: {}ms\\nlimit: {}ms\\n\", elapsed<ms>().count(), delta<ms>().count(),\n\
      \                           std::chrono::duration_cast<ms>(limit).count());\n\
      \    }\n\nprivate:\n    duration elapsed_raw() const { return clock::now() -\
      \ start; }\n    duration delta_raw() const { return clock::now() - latest; }\n\
      };\ninline Timer timer;\n\n}  // namespace gwen\n"
    name: bundled
  isFailed: false
  isVerificationFile: false
  path: include/gwen/misc/timer.hpp
  pathExtension: hpp
  requiredBy: []
  timestamp: '2026-07-13 23:30:07+09:00'
  title: Timer
  verificationStatus: LIBRARY_NO_TESTS
  verifiedWith: []
documentation_of: include/gwen/misc/timer.hpp
layout: document
title: Timer
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

**@{keyword.complexity}**

- $O(1)$

## elapsed

```cpp
template <typename Unit> Unit timer.elapsed() const
```

タイマーが構築されてからの**全体の経過時間**を指定した時間単位で返します。

**@{keyword.complexity}**

- $O(1)$

## delta

```cpp
template <typename Unit> Unit timer.delta() const
```

直近で `reset()` または `lap()` を呼び出した時刻（構築直後は構築時刻）からの**経過時間（ラップタイム）**を指定した時間単位で返します。

**@{keyword.complexity}**

- $O(1)$

## lap

```cpp
template <typename Unit> Unit timer.lap()
```

現在の `delta()` を返しつつ、ラップタイム測定用の基準時刻をリセット（`reset()`）します。

**@{keyword.complexity}**

- $O(1)$

## reset

```cpp
void timer.reset()
```

ラップタイム測定用の基準時刻を「現在時刻」にリセットします。全体の経過時間（`elapsed`）の基準時刻には影響しません。

**@{keyword.complexity}**

- $O(1)$

## set_limit

```cpp
void timer.set_limit(duration target)
```

タイマーの制限時間を設定します。

**@{keyword.complexity}**

- $O(1)$

## restart

```cpp
void timer.restart(duration target)
```

ラップタイムをリセット（`reset()`）し、同時に新たな制限時間を設定します。

**@{keyword.complexity}**

- $O(1)$

## expired

```cpp
bool timer.expired() const
```

前回の `reset()` や `lap()` などのラップタイム基準時刻からの経過時間が、設定された制限時間を超過している場合に `true` を返します。

**@{keyword.complexity}**

- $O(1)$

## expired_elapsed

```cpp
bool timer.expired_elapsed() const
```

タイマーが構築されてからの全体経過時間が、設定された制限時間を超過している場合に `true` を返します。

**@{keyword.complexity}**

- $O(1)$

## dump

```cpp
std::string timer.dump() const
```

現在の全体の経過時間、ラップタイム、および制限時間を改行区切りの文字列として出力します。

**@{keyword.complexity}**

- $O(1)$

## @{keyword.examples}

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