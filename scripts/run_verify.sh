#!/bin/bash

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
VERIFY_DIR="$ROOT/verify"
INCLUDE_DIR="$ROOT/include"
BUILD_DIR="$ROOT/.build"
CXX="${CXX:-g++}"
CXXFLAGS="-std=c++20 -I $INCLUDE_DIR -O2"

run_test() {
    local name="$1"
    local cpp_path="$2"
    local exe="$BUILD_DIR/verify_${name}"

    mkdir -p "$BUILD_DIR"
    local err
    err=$("$CXX" $CXXFLAGS "$cpp_path" -o "$exe" 2>&1)
    if [[ $? -ne 0 ]]; then
        echo "  [FAIL] $name: compile error"
        [[ -n "$err" ]] && echo "$err" | sed 's/^/        /'
        return 1
    fi

    # Root files are self-contained tests (compile and run)
    # Subdirectory files are online judge solvers (compile only)
    local parent_dir
    parent_dir=$(dirname "$cpp_path")
    if [[ "$parent_dir" == "$VERIFY_DIR" ]]; then
        if "$exe"; then
            echo "  [OK]   $name"
            return 0
        else
            echo "  [FAIL] $name: exit code $?"
            return 1
        fi
    else
        echo "  [OK]   $name (compile only)"
        return 0
    fi
}

main() {
    cd "$ROOT"
    echo "gwen verify"
    echo "------------"

    if [[ -n "$1" ]]; then
        # Find specified test by name recursively
        local target_name="$1"
        local cpp_path=""
        while IFS= read -r -d '' file; do
            if [[ "$(basename "$file" .cpp)" == "$target_name" ]]; then
                cpp_path="$file"
                break
            fi
        done < <(find "$VERIFY_DIR" -type f -name "*.cpp" -print0)

        if [[ -n "$cpp_path" ]]; then
            run_test "$target_name" "$cpp_path"
        else
            echo "  [SKIP] $target_name: test file not found"
            exit 1
        fi
    else
        local failed=0
        while IFS= read -r -d '' cpp; do
            name=$(basename "$cpp" .cpp)
            run_test "$name" "$cpp" || ((failed++))
        done < <(find "$VERIFY_DIR" -type f -name "*.cpp" -print0 | sort -z)

        if [[ $failed -gt 0 ]]; then
            echo "------------"
            echo "$failed test(s) failed"
            exit 1
        fi
    fi
}

main "$@"
