#!/usr/bin/env bash
#
# release-body.sh - generate the GitHub Release description for a tag.
#
# Usage:
#   ./scripts/release-body.sh <version> <output-file>
#
# Reads the matching "## [<version>]" section from CHANGELOG.md and wraps it
# in the release title + install instructions. Falls back to a short notice
# when no changelog entry exists yet.
#

set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"

TAG="${1:?usage: release-body.sh <version> <output-file>}"
OUT="${2:?usage: release-body.sh <version> <output-file>}"

{
    echo "# sget $TAG"
    echo
    echo "The CLI data-tracer: a high-performance web data extractor built on top of \`curl\`."
    echo
    echo "## Install"
    echo
    echo '```bash'
    echo "# Debian / Ubuntu"
    echo "sudo apt install ./sget_${TAG}_amd64.deb"
    echo
    echo "# Fedora / RHEL"
    echo "sudo dnf install ./sget-${TAG}-1.x86_64.rpm"
    echo '```'
    echo
    echo "## Changelog"
    echo
} > "$OUT"

if awk -v v="[$TAG]" '
    $0 == "## " v { on = 1; next }
    /^## / { on = 0 }
    on { print }
' "$ROOT/CHANGELOG.md" >> "$OUT"; then
    :
fi

if ! grep -q "## \[$TAG\]" "$ROOT/CHANGELOG.md"; then
    echo "_No changelog entry for this version yet._" >> "$OUT"
fi