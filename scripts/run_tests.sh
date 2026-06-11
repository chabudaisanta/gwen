#!/usr/bin/env bash
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
BUILD_DIR="$ROOT/.build"

echo "=== Auto-generating test cases and CMake targets ==="
python3 "$ROOT/scripts/manage_problems.py" generate-all

echo "=== Building tests with CMake ==="
mkdir -p "$BUILD_DIR"
cmake -S "$ROOT" -B "$BUILD_DIR" -DCMAKE_BUILD_TYPE=Release
cmake --build "$BUILD_DIR" --parallel

echo ""
echo "=== Running Google Test ==="
exec "$BUILD_DIR/test/gwen_tests" "$@"
