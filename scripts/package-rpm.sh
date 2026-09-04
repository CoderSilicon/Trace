#!/usr/bin/env bash
#
# package-rpm.sh - convert a fresh build into an installable RPM package.
#
# Requires rpmbuild on the build host:
#   Fedora/RHEL:   dnf install rpm-build
#   Debian/Ubuntu: apt install rpm
#
# Usage:
#   OUT_DIR=/tmp/pkg ./scripts/package-rpm.sh
#

set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
BUILD_DIR="${BUILD_DIR:-$ROOT/build}"
PKG_DIR="${PKG_DIR:-$ROOT/pkg}"

if ! command -v rpmbuild >/dev/null 2>&1; then
    echo "error: rpmbuild is required to build the .rpm package" >&2
    echo "       install it with:  dnf install rpm-build   (or apt install rpm)" >&2
    exit 1
fi

"$ROOT/scripts/build.sh"

mkdir -p "$PKG_DIR"
cpack -B "$PKG_DIR" --config "$BUILD_DIR/CPackConfig.cmake" -G RPM

echo "RPM package built:"
echo "  $PKG_DIR/$(ls "$PKG_DIR" | grep -m1 '\.rpm$')"