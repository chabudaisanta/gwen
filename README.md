# gwen

競技プログラミングのための C++ ヘッダーオンリーライブラリ。

## 特徴
- **ヘッダーオンリー**: `include/` にパスを通すだけで利用可能です。
- **AIによる開発支援**: このライブラリは、Google AntiGravitiyを使用して開発されています。
- **C++23 Concept**: 汎用的なデータ構造やアルゴリズムにおいて C++23 concept を活用し、より安全で意図の明確なインターフェースを提供します。
- **堅牢な検証**: Google Test による単体テストと `competitive-verifier` によるオンラインジャッジテストの両面から品質を担保しています。

## 要件
- C++23 以上
- CMake 3.14+ (テスト実行用)
- Python 3.8+ (competitive-verifier 等のスクリプト実行用)

## ディレクトリ構成
- `include/gwen/`: ライブラリ本体 (ヘッダーオンリー)
- `test/unit/`: Google Test を用いた単体テスト
- `verify/`: `competitive-verifier` を用いたオンラインジャッジでの検証用コード
- `doc/`: ドキュメント
- `scripts/`: ビルド・テスト等のターミナル実行用ユーティリティスクリプト
- `planning/`: `TODO.md` などの計画や設計ドキュメント
- `references/`: 参考実装等

## ビルドとテスト (Google Test)
当ライブラリはヘッダーオンリーのためライブラリ自体のビルドは不要ですが、ユニットテストを実行するには CMake を使用します。

```bash
cmake -S . -B .build
cmake --build .build
cd .build && ctest
```

## 検証 (competitive-verifier)
`competitive-verifier` を用いて、実際の競技プログラミングプラットフォーム（Yosupo Judge 等）の問題で検証を行います。

```bash
# TODO: verify コマンドの準備が整い次第更新
```
