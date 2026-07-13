#!/usr/bin/env bash
set -e

# カレントディレクトリをプロジェクトルートに移動
cd "$(dirname "$0")/.."

echo "==> Running competitive-verifier <=="
export CPLUSINCLUDEPATH="$(pwd)/include"
competitive-verifier verify
