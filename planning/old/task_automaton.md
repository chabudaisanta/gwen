# Automaton モジュール 実装設計書

## 1. 概要
`shojin.cpp` のプロトタイプを元に、オートマトンを表すデータ構造 `Automaton` と、オートマトンの直積などを扱う代数構造 `automaton_monoid` を実装する。

## 2. 命名規則・配置
- **ヘッダファイル**: `include/gwen/automaton/automaton.hpp`
- **データ構造**: `Automaton` (PascalCase)
- **代数構造 (モノイド)**: `automaton_monoid` (snake_case、実体のない代数構造であるため)
- **メソッド名/変数名**: `size`, `init`, `accept`, `edges`, `op`, `e` などすべて snake_case

## 3. クラス設計 (C++23)

### Automaton 構造体
```cpp
template <typename T, i32 base>
struct Automaton {
    i32 size;
    std::vector<i32> init;
    std::vector<i32> accept;
    std::vector<std::pair<i32, T>> edges; // edges[i * base + x] = {next_state, label}
};
```

### automaton_monoid 構造体
```cpp
template <typename T, i32 base>
struct automaton_monoid {
    using S = Automaton<T, base>;
    
    static S op(const S& a, const S& b);
    static S e();
};
```
※ `automaton_monoid` 自体は `gwen::monoid<automaton_monoid<T, base>>` を満たす。

## 4. ドキュメント (AC Library スタイル)
- **配置パス**: `doc/automaton.md`
- **内容**: `## Automaton`, `## automaton_monoid`, 制約 (Constraints)、計算量 (Complexity) などを太字で記述する。
- **目次追加**: `.verify-helper/docs/index.md` へリンクを追加する。

## 5. テスト計画
- **Google Test**: `test/unit/automaton/automaton_test.cpp` にて、初期化、直積 (`op`)、単位元 (`e`) の動作確認を行う。
- **Verifyコード**: `verify/dummy/automaton_dummy.test.cpp` 等で動作確認を行う。

## 6. その他要件
- プリミティブ型は `i32` など `gwen/types.hpp` の型エイリアスを使用。
- 全ての public API に対し Doxygen コメントを付与。
