#!/usr/bin/env bash
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT"

PYTHON="${ROOT}/.venv/bin/python"
if [[ ! -x "$PYTHON" ]]; then
    PYTHON="python3"
fi

if ! "$PYTHON" -c "import pytest" 2>/dev/null; then
    echo "pytest is not installed."
    echo "Run:"
    echo "  python3 -m venv .venv"
    echo "  .venv/bin/pip install -r requirements-dev.txt"
    exit 1
fi

exec "$PYTHON" -m pytest test/ -v "$@"
