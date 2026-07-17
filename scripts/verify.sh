#!/usr/bin/env bash
set -eo pipefail

# カレントディレクトリをプロジェクトルートに移動
cd "$(dirname "$0")/.."

# 仮想環境が存在する場合はアクティベート
if [ -d ".venv" ]; then
    source .venv/bin/activate
fi

# 引数で指定されたターゲットがあればそれを使用し、なければ verify/** を指定する
TARGET="verify/**"
if [ "$#" -gt 0 ]; then
    TARGET="$1"
fi

echo "==> Running competitive-verifier <=="
export CPLUSINCLUDEPATH="$(pwd)/include"
export PATH="$HOME/.local/bin:$PATH"
echo "1. Resolving dependencies for ${TARGET}..."
competitive-verifier oj-resolve --config config.toml --include "${TARGET}" > verify_files.json

echo "2. Running verification..."
set +e
competitive-verifier verify --verify-json verify_files.json --output result.json --check-error --tle 10.0 2>&1 | tee >(sed -E 's/\x1b\[[0-9;]*[a-zA-Z]//g' > verify/verify.log)
EXIT_CODE=${PIPESTATUS[0]}
set -e

echo ""
echo "=== Verification Summary ==="
if [ -f result.json ]; then
    python3 scripts/summary.py result.json
fi

exit $EXIT_CODE
