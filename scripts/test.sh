#!/usr/bin/env bash
set -e

# カレントディレクトリをプロジェクトルートに移動
cd "$(dirname "$0")/.."

echo "==> Building tests (via scripts/build.sh) <=="
./scripts/build.sh

echo "==> Running unit tests <=="
cd .build

set +e
ctest --output-on-failure
EXIT_CODE=$?
set -e

# CTestが自動生成する詳細ログ（標準出力・標準エラー出力を含む）をコピー
cp Testing/Temporary/LastTest.log ../test/test.log
echo "==> Test log saved to ./test/test.log <=="

exit $EXIT_CODE
