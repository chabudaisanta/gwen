#!/bin/bash

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
VERIFY_DIR="$ROOT/verify"
SRC_DIR="$ROOT/src"
BUILD_DIR="$ROOT/debug"
CXX="${CXX:-g++}"
CXXFLAGS="-std=c++20 -I $SRC_DIR -O2"

run_test() {
    local name="$1"
    local cpp="$VERIFY_DIR/${name}.cpp"
    local exe="$BUILD_DIR/verify_${name}.exe"

    if [[ ! -f "$cpp" ]]; then
        echo "  [SKIP] $name: ${name}.cpp not found"
        return 1
    fi

    mkdir -p "$BUILD_DIR"
    local err
    err=$("$CXX" $CXXFLAGS "$cpp" -o "$exe" 2>&1)
    if [[ $? -ne 0 ]]; then
        echo "  [FAIL] $name: compile error"
        [[ -n "$err" ]] && echo "$err" | sed 's/^/        /'
        return 1
    fi

    if "$exe"; then
        echo "  [OK]   $name"
        return 0
    else
        echo "  [FAIL] $name: exit code $?"
        return 1
    fi
}

main() {
    cd "$ROOT"
    echo "gwen verify"
    echo "------------"

    if [[ -n "$1" ]]; then
        run_test "$1"
    else
        local failed=0
        for cpp in "$VERIFY_DIR"/*.cpp; do
            [[ -f "$cpp" ]] || continue
            name=$(basename "$cpp" .cpp)
            run_test "$name" || ((failed++))
        done
        if [[ $failed -gt 0 ]]; then
            echo "------------"
            echo "$failed test(s) failed"
            exit 1
        fi
    fi
}

main "$@"
