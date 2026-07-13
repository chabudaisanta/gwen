#!/bin/bash
set -euo pipefail

# スクリプトのあるディレクトリを起点に、プロジェクトルートへ移動
cd "$(dirname "$0")/.."

echo "Running clang-format..."

# 対象のディレクトリを配列で指定
TARGET_DIRS=("include" "test" "verify")

for dir in "${TARGET_DIRS[@]}"; do
    if [ -d "$dir" ]; then
        # .hpp と .cpp ファイルを再帰的に検索し、clang-format を上書きで実行
        find "$dir" -type f \( -name "*.hpp" -o -name "*.cpp" \) -print | xargs -r clang-format -i
    fi
done

echo "Format complete."
