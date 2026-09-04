#!/usr/bin/env bash
#
# build.sh - configure and build sget with CMake (Release).
#
# Usage:
#   BUILD_DIR=/path/to/build ./scripts/build.sh
#

set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
BUILD_DIR="${BUILD_DIR:-$ROOT/build}"

cmake -S "$ROOT" -B "$BUILD_DIR" -DCMAKE_BUILD_TYPE=Release
cmake --build "$BUILD_DIR" --config Release -j"$(nproc)"

echo "Built sget -> $BUILD_DIR/sget"