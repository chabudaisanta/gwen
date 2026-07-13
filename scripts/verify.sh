#!/usr/bin/env bash
set -e

# カレントディレクトリをプロジェクトルートに移動
cd "$(dirname "$0")/.."

echo "==> Running competitive-verifier <=="
export CPLUSINCLUDEPATH="$(pwd)/include"
export PATH="$HOME/.local/bin:$PATH"
echo "1. Resolving dependencies..."
competitive-verifier oj-resolve --include "verify/**" > verify_files.json

echo "2. Running verification..."
competitive-verifier verify --verify-json verify_files.json
