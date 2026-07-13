#!/usr/bin/env bash
set -e

# カレントディレクトリをプロジェクトルートに移動
cd "$(dirname "$0")/.."

echo "==> Building tests (via scripts/build.sh) <=="
./scripts/build.sh

echo "==> Running unit tests <=="
cd .build
ctest --output-on-failure
