#!/usr/bin/env bash
set -e

# カレントディレクトリをプロジェクトルートに移動
cd "$(dirname "$0")/.."

echo "==> Configuring CMake (C++23) <=="
cmake -S . -B .build

echo "==> Building tests <=="
cmake --build .build
