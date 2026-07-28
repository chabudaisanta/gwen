# 今後の追加機能案

以下の機能は、旧 master ブランチに存在していたものの移植や、Library Checker (Yosupo judge) などの頻出テーマに基づく新規実装の余地があるモジュールです。
必要に応じて実装（`port_module`）の対象としてご検討ください。

## 1. 幾何全般 (Geometry)
*   `geo/point.hpp`, `geo/graham_scan.hpp` など、点・ベクトルの基本から凸包、偏角ソート、線分の交差判定、多角形の面積などの幾何ライブラリ全般。

## 2. 高度なデータ構造 (Advanced Data Structures)
*   **永続系全般**: `container/persistent_stack.hpp` の他、永続セグメント木や永続Union-Findなど。
*   **Static RMQ / Disjoint Sparse Table**: セグメント木より定数倍が早く、構築 $O(N \log N)$ 取得 $O(1)$ などで動く Sparse Table や、モノイドに対する静的区間クエリを $O(1)$ で処理する Disjoint Sparse Table。
*   **SegTree Beats**: 区間に対する chmin/chmax 更新や、区間和などを同時に処理できる高度なセグメント木。
*   **Double-ended Priority Queue**: 最大値と最小値の両方を高速に取り出せる優先度付きキュー。
*   **Binary Trie**: 数のXOR演算等と相性の良いTrie木（`ds/trie.hpp` の整数バイナリ特化としての充実）。
*   **Splay Tree**: 均らして $O(\log N)$ で動作し、列の分割・結合が柔軟な平衡二分探索木。Link-Cut Tree の基盤。
*   **FastSet (64分木 / vEB木)**: 整数集合に対する検索や値の追加・削除をビット演算を用いて非常に高速（$O(\log_{64} N)$ 等）に行うデータ構造。
*   **区間を set で管理するテク**: 連続する区間を `std::set` などで管理し、区間の追加・削除・結合をならし $O(\log N)$ で行う手法のクラス化。
*   `container/associative_treap.hpp`: 連想配列のように振る舞う Treap。
*   `container/sq_div.hpp`: 平方分割 (Square Root Decomposition) の一般的なデータ構造ベース。

## 3. グラフ・木構造 (Graph & Tree)
*   **Tree全般**: 木の直径、重心、オイラーツアーなど、木に関する基本・頻出アルゴリズムを「全部入り」にした利便性の高いモジュール。
*   **Rerooting (全方位木DP)**: 木上の動的計画法をすべての根に対して $O(N)$ で計算する抽象化ライブラリ。
*   **SCC (強連結成分分解) と DAG 縮約**: 有向グラフを SCC で分解し、各成分を1つの頂点に縮約した DAG (有向非巡回グラフ) を自動で構築してくれる便利なモジュール。
*   `graph/range_edge_graph.hpp`: 区間に辺を張る（あるいは区間から辺を張る）操作をセグメント木を用いて効率的に行うグラフ構築ヘルパー。

## 4. 文字列系 (Strings)
*   **String系全般**: Suffix Array, LCP Array, Z-algorithm, Manacher などの文字列アルゴリズム全般。
*   **Aho-Corasick**: 複数パターンの文字列検索をオートマトンを用いて高速に行うアルゴリズム。

## 5. 数学・多項式 (Math & Convolution)
*   **Convolution (畳み込み)**: NTT (Number Theoretic Transform) や FFT を用いた高速な数列の畳み込み。
*   **FPS (形式的冪級数)**: 逆元、log、exp、pow などの多項式に対する高度な演算全般。
*   **Stern-Brocot Tree**: 有理数の列挙や二分探索、特定の分数の近似に用いる探索木。
*   **modint (Barrett reduction)**: Barrett reduction を用いて、任意の剰余に対する剰余演算を高速化した動的 modint。
*   **任意 mod 二項係数**: 任意の法における二項係数の計算。(参考: [arbitrary-mod-binomial](https://nyaannyaan.github.io/library/modulo/arbitrary-mod-binomial.hpp))
*   `math/binom.hpp`: `combination.hpp` の拡張（法が素数でない場合やLucasの定理など特殊ケース向け）。

## 6. ハッシュ・乱数・Zobrist Hash
*   **高速なハッシュマップ**: `std::unordered_map` より定数倍が圧倒的に高速なハッシュマップの実装 (カスタムハッシュ関数と Open Addressing など)。
*   `hash/zobrist.hpp`: Zobrist Hash の実装。集合の一致判定などに。
*   `hash/to_hash.hpp`: 様々な型を `std::hash` ライクに一意な値に落とし込むハッシュ拡張ヘルパー。

## 7. その他ユーティリティ・特殊クエリ
*   **FastIO**: `std::cin` / `std::cout` を使わず、`fread` / `fwrite` などを用いて極限まで入出力を高速化するライブラリ。
*   `container/mdarray.hpp`: C++23の `std::mdspan` の代替、あるいは使いやすい多次元配列ラッパー。
*   `query/functional_query.hpp` / `query/polynomial_difference_array.hpp`: 特殊な関数クエリや多項式階差などを扱うモジュール。
