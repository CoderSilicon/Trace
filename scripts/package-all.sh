#!/usr/bin/env bash
#
# package-all.sh - build both the .deb and .rpm packages.
#
# Usage:
#   ./scripts/package-all.sh
#

set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"

"$ROOT/scripts/package-deb.sh"
"$ROOT/scripts/package-rpm.sh"

echo "All packages built into: $ROOT/pkg"