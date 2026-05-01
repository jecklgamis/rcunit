#!/usr/bin/env bash
set -euo pipefail

DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$DIR"

SPEC_FILE="$DIR/rpm/rcunit.spec"
PKG_NAME="$(rpmspec -q --qf '%{name}\n' "$SPEC_FILE" | head -1)"
PKG_VERSION="$(rpmspec -q --qf '%{version}\n' "$SPEC_FILE" | head -1)"
PKG_RELEASE="$(rpmspec -q --qf '%{release}\n' "$SPEC_FILE" | head -1)"
TARBALL="${PKG_NAME}-${PKG_VERSION}.tar.gz"
RPMBUILD_DIR="$DIR/build/rpm"
OUTPUT_DIR="$DIR/build/rpm/RPMS"

echo "Building RPM package: ${PKG_NAME} ${PKG_VERSION}-${PKG_RELEASE}"

command -v rpmbuild >/dev/null 2>&1 || { echo "rpmbuild not found. Install: sudo dnf install rpm-build"; exit 1; }
command -v rpmspec  >/dev/null 2>&1 || { echo "rpmspec not found. Install: sudo dnf install rpm-build"; exit 1; }
command -v autoreconf >/dev/null 2>&1 || { echo "autoreconf not found. Install: sudo dnf install autoconf automake"; exit 1; }

mkdir -p "$RPMBUILD_DIR"/{BUILD,BUILDROOT,RPMS,SOURCES,SPECS,SRPMS}

cp "$SPEC_FILE" "$RPMBUILD_DIR/SPECS/"

echo "Creating source tarball: $TARBALL"
git archive --prefix="${PKG_NAME}-${PKG_VERSION}/" HEAD \
    | gzip > "$RPMBUILD_DIR/SOURCES/$TARBALL"

rpmbuild -ba \
    --define "_topdir $RPMBUILD_DIR" \
    "$RPMBUILD_DIR/SPECS/rcunit.spec"

echo "Package(s) written to $OUTPUT_DIR:"
find "$OUTPUT_DIR" -name "*.rpm"
