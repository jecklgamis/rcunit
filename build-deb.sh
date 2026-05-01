#!/usr/bin/env bash
set -euo pipefail

DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$DIR"

PKG_NAME="rcunit"
PKG_VERSION="$(dpkg-parsechangelog -S Version)"
ARCH="$(dpkg --print-architecture)"
OUTPUT_DIR="$DIR/build/deb"

echo "Building Debian package: ${PKG_NAME} ${PKG_VERSION} (${ARCH})"

command -v dpkg-buildpackage >/dev/null 2>&1 || { echo "dpkg-buildpackage not found. Install: sudo apt install dpkg-dev debhelper"; exit 1; }
command -v autoreconf >/dev/null 2>&1 || { echo "autoreconf not found. Install: sudo apt install autoconf automake"; exit 1; }

mkdir -p "$OUTPUT_DIR"

dpkg-buildpackage -us -uc -b

find "$DIR/.." -maxdepth 1 -name "${PKG_NAME}_*.deb" -o -name "${PKG_NAME}_*.buildinfo" -o -name "${PKG_NAME}_*.changes" | while read -r f; do
    mv "$f" "$OUTPUT_DIR/"
done

echo "Package(s) written to $OUTPUT_DIR:"
ls "$OUTPUT_DIR"
