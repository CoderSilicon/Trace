#!/usr/bin/env bash
#
# package-deb.sh - convert a fresh build into an installable Debian package.
#
# Requires dpkg-dev (dpkg-shlibdeps) on the build host:
#   Debian/Ubuntu:  apt install dpkg-dev
#
# Usage:
#   OUT_DIR=/tmp/pkg ./scripts/package-deb.sh
#

set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
BUILD_DIR="${BUILD_DIR:-$ROOT/build}"
PKG_DIR="${PKG_DIR:-$ROOT/pkg}"

if ! command -v dpkg-shlibdeps >/dev/null 2>&1; then
    echo "error: dpkg-dev is required to build the .deb package" >&2
    echo "       install it with:  sudo apt install dpkg-dev" >&2
    exit 1
fi

"$ROOT/scripts/build.sh"

mkdir -p "$PKG_DIR"
cpack -B "$PKG_DIR" --config "$BUILD_DIR/CPackConfig.cmake" -G DEB

echo "Debian package built:"
echo "  $PKG_DIR/$(ls "$PKG_DIR" | grep -m1 '\.deb$')"