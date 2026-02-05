# Gwen

競技プログラミング用 C++ ライブラリです。Header-only で、必要なヘッダをインクルードして使用します。

## 必要条件

- C++20 以上
- GCC / Clang（`__int128` を使用）

## ディレクトリ構成

```
src/gwen/
├── algebra/      # 代数構造（モノイド、アーベル群）
├── container/    # データ構造
├── geo/          # 幾何
├── graph/        # グラフ・木
├── hash/         # ハッシュ
├── math/         # 数学
├── misc/         # ユーティリティ
├── mod/          # 剰余演算
├── query/        # クエリ処理
├── dump.hpp      # デバッグ用出力
├── io.hpp        # 高速入出力
├── my_template.hpp  # テンプレート集
└── types.hpp     # 型定義
```

---

## 使い方

### 基本テンプレート

```cpp
#include "gwen/my_template.hpp"
// または個別に
#include "gwen/types.hpp"
#include "gwen/io.hpp"
#include "gwen/dump.hpp"
```

コンパイル時に include パスを指定：

```bash
g++ -std=c++20 -I src your_code.cpp
```

### 型・リテラル

`gwen::types.hpp` で定義：
- `i32`, `i64`, `i128` / `u32`, `u64`, `u128`
- `123_i32`, `456_u64` などのリテラル

---

## モジュール一覧

### algebra（代数構造）

| ファイル | 内容 |
|----------|------|
| `basic_monoid.hpp` | `sum_monoid`, `min_monoid`, `max_monoid`, `gcd_monoid`, `xor_monoid`, `affine_monoid` |
| `basic_abel.hpp` | `sum_abel`, `xor_abel`（逆元付き） |
| `range_affine_range_sum_monoid.hpp` | 区間 Affine・区間和の遅延セグ木用 |
| `rolling_hash_monoid.hpp` | ローリングハッシュ用モノイド |

### container（データ構造）

| ファイル | 内容 |
|----------|------|
| `fenwick_tree.hpp` | Fenwick Tree（BIT）。アーベル群を載せる |
| `mergesort_tree.hpp` | マージソートツリー |
| `persistent_stack.hpp` | 永続スタック |
| `sq_div.hpp` | 平方分割 |
| `treap.hpp` | Treap |
| `wrapper/range_add_update_sum.hpp` | 区間加算・区間和のラッパー |

**Fenwick Tree 使用例：**
```cpp
#include "gwen/container/fenwick_tree.hpp"
#include "gwen/algebra/basic_abel.hpp"

gwen::fenwick_tree<gwen::sum_abel<i64>> ft(N);
ft.add(i, x);
i64 s = ft.sum(l, r);  // [l, r) の和
```

### geo（幾何）

| ファイル | 内容 |
|----------|------|
| `point.hpp` | 2次元ベクトル・点 |
| `graham_scan.hpp` | 凸包（Graham scan） |

### graph（グラフ・木）

| ファイル | 内容 |
|----------|------|
| `edge.hpp` | `edge`, `full_edge<W>` |
| `forest.hpp` | 森の管理 |
| `xor_linked_tree.hpp` | XOR リンク木（内部用） |
| `doubling_tree.hpp` | ダブリングによる LCA・k 先祖・距離 |
| `weighted_dsu.hpp` | 重み付き Union-Find（ポテンシャル付き DSU） |

**doubling_tree 使用例：**
```cpp
#include "gwen/graph/doubling_tree.hpp"

std::vector<std::vector<int>> G(N);  // 隣接リスト
gwen::doubling_tree T(N, root, G);
T.lca(u, v);       // LCA
T.kth_anc(v, k);   // v の k 個上の祖先
T.depth(v);        // 深さ
T.len(u, v);       // u-v パスの長さ（頂点数）
```

**weighted_dsu 使用例：**
```cpp
#include "gwen/graph/weighted_dsu.hpp"
#include "gwen/algebra/basic_abel.hpp"

gwen::weighted_dsu<gwen::sum_abel<i64>> dsu(N);
dsu.merge(a, b, w);   // diff(a,b)=w となるようにマージ
i64 d = dsu.diff(a, b);
bool ok = dsu.same(a, b);
```

### hash（ハッシュ）

| ファイル | 内容 |
|----------|------|
| `rolling_hash.hpp` | 静的列の区間ハッシュ（mod61, 逆元不要） |
| `zobrist.hpp` | Zobrist ハッシュ |

**rolling_hash 使用例：**
```cpp
#include "gwen/hash/rolling_hash.hpp"

std::string s = "abcabc";
gwen::rolling_hash<> rh(s);
auto h = rh.get(0, 3);       // 区間 [0,3) のハッシュ
bool eq = rh.equal(0, 3, 3, 6);  // 部分列が一致するか
i32 len = rh.lcp(0, 3);      // suffix の最長共通接頭辞
```

### math（数学）

| ファイル | 内容 |
|----------|------|
| `binom.hpp` | 二項係数（動的拡張） |
| `combination.hpp` | 組み合わせ |
| `matrix.hpp` | 行列演算 |
| `prime.hpp` | 素因数分解、Miller-Rabin 素数判定 |

**prime 使用例：**
```cpp
#include "gwen/math/prime.hpp"

auto fac = gwen::factorize(x);     // 素因数分解
bool p = gwen::miller(n);          // 素数判定 (u64)
```

### misc（ユーティリティ）

| ファイル | 内容 |
|----------|------|
| `compress.hpp` | 座標圧縮 |
| `counting_sort.hpp` | カウンティングソート |
| `lis.hpp` | 最長増加部分列 |
| `xorshift.hpp` | 乱数 |

### mod（剰余演算）

| ファイル | 内容 |
|----------|------|
| `mod.hpp` | 剰余ユーティリティ |
| `mod61.hpp` | mod 2^61-1 |
| `modint.hpp` | `dynamic_modint64`（実行時 mod 設定） |

### query（クエリ処理）

| ファイル | 内容 |
|----------|------|
| `mo.hpp` | Mo's algorithm（Hilbert 順） |
| `swag.hpp` | Sliding Window Aggregation |
| `functional_query.hpp` | 関数的クエリ |

---

## 入出力

`gwen::io.hpp` で高速入出力を提供：

```cpp
#include "gwen/io.hpp"

gwen::input >> n >> m >> vec;
gwen::output << ans << '\n';
```

---

## デバッグ

`LOCAL` が定義されているとき、`gwen::dump.hpp` により以下が有効になります：

- `DEBUG(x, y, ...)` : 変数を標準エラーに出力
- `DUMP(x, y, ...)` : 同上
- `gwen::to_str(x)` : 各種コンテナ・ペアなどを文字列化

---

## verify・テスト

`verify/` に検証用コードがあります：

- `rolling_hash.cpp` : rolling_hash の検証

`verify/yosupo/` に Library Checker 用のコードがあります：

- `lca.cpp` : LCA（doubling_tree）
- `jump_on_tree.cpp` : 木上のジャンプ
- `primality_test.cpp` : 素数判定

テストの実行：

```bash
./scripts/run_verify.sh           # 全テスト実行
./scripts/run_verify.sh rolling_hash  # 特定のテストのみ
```

---

## ライセンス

LICENSE を参照してください。
