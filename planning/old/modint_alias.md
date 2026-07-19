# modint エイリアス及び StaticModInt 実装計画

## 1. 背景と目的
現在 `mod/modint.hpp` には実行時に法を設定できる `DynamicModInt64` が用意されています。しかし、998244353 や 1000000007 といった競技プログラミング等で頻出する固定の法（Mod）に対して、都度 `set_mod` を呼ぶのは手間であり、コンパイル時最適化の恩恵も受けられません。
そこで、よく使う mod を簡単に扱えるようにするための実装を追加します。

## 2. 検討事項と設計
ユーザー提案の通り、いくつかのアプローチが考えられます。

### 案A: `DynamicModInt64` の継承とラップ
- `DynamicModInt64` の静的変数はクラスごとに1つであるため、単に継承するだけでは異なる Mod を同時に扱うことができません。
- テンプレート引数 `ID` などを導入して `DynamicModInt64<ID>` とすれば分離できますが、動的な法の設定 (`set_mod`) を一回だけ呼ぶ仕組み等が必要になり、ややオーバーヘッドが残ります。

### 案B: `StaticModInt64<u64 Mod>` を新設する（推奨）
- テンプレート引数として法 `Mod` を受け取る `StaticModInt64<u64 Mod>` クラスを新設します。
- `DynamicModInt64` と同様に Montgomery 乗算を用いますが、`Mod` がコンパイル時に確定しているため、Montgomery 還元に必要なパラメータ (`ns`, `r2`) を **`constexpr` 関数でコンパイル時に計算** できます。
- これにより、実行時の `set_mod` 呼び出しが完全に不要となり、パフォーマンス面で最大化されます。
- コードの重複（冗長性）は生じますが、コンパイル時定数としての最適化や実装のシンプルさを考慮すると、ACL (AtCoder Library) 等でも採用されている標準的なアプローチです。

## 3. 実装内容
1. `include/gwen/mod/modint.hpp` に以下の内容を追加します。
    - `StaticModInt64<u64 Mod>` の定義
        - `ns` や `r2` の算出を `constexpr` で行う。
    - 頻出するエイリアスの定義
        ```cpp
        using modint998244353 = StaticModInt64<998244353>;
        using modint1000000007 = StaticModInt64<1000000007>;
        ```
2. テストの追加
    - `test/unit/mod/modint_test.cpp` に `modint998244353` と `modint1000000007` の四則演算等のテストを追加。
3. Verifyコードの追加
    - Library Checker 等の適切な問題（例: Yosupo Judge の `math/pow_of_matrix` など、適当な問題が無ければ既存の `modint` 用の問題を再利用）に対して `modint998244353` を使用して Verify するコードを追加・修正する。
4. ドキュメントの追加
    - `doc/mod/modint.md` (存在しなければ作成) に AC Library スタイルで `StaticModInt64`, `modint998244353`, `modint1000000007` について追記する。

## 4. レビュワーへの確認事項
- 冗長性を減らすためにマクロやCRTP等で共通化すべきか、それとも `StaticModInt64` として独立して実装してよいか。
- エイリアス名について、`modint998244353` の形式で問題ないか。
