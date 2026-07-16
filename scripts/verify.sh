#!/usr/bin/env bash
set -e

# カレントディレクトリをプロジェクトルートに移動
cd "$(dirname "$0")/.."

# 仮想環境が存在する場合はアクティベート
if [ -d ".venv" ]; then
    source .venv/bin/activate
fi

echo "==> Running competitive-verifier <=="
export CPLUSINCLUDEPATH="$(pwd)/include"
export PATH="$HOME/.local/bin:$PATH"
echo "1. Resolving dependencies..."
competitive-verifier oj-resolve --config config.toml --include "verify/**" > verify_files.json

echo "2. Running verification..."
competitive-verifier verify --verify-json verify_files.json
