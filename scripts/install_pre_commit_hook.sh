#!/usr/bin/env bash
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
HOOK="$ROOT/.git/hooks/pre-commit"

cat > "$HOOK" << EOF
#!/usr/bin/env bash
set -euo pipefail
exec "$ROOT/scripts/run_tests.sh"
EOF

chmod +x "$HOOK"
echo "Installed pre-commit hook: $HOOK"
echo "Skip once with: git commit --no-verify"
