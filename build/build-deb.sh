#!/usr/bin/env bash
# Build a .deb package for librcunit-dev locally.
# Produces the .deb in dist/ relative to the repo root.
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd "${SCRIPT_DIR}/.." && pwd)"

cd "${REPO_ROOT}"

check_deps() {
    local missing=()
    for cmd in dpkg-buildpackage debhelper dh autoreconf pkg-config lintian; do
        command -v "${cmd}" &>/dev/null || missing+=("${cmd}")
    done
    if [[ ${#missing[@]} -gt 0 ]]; then
        echo "Missing build dependencies. Install with:"
        echo "  sudo apt-get install -y autoconf automake libtool build-essential debhelper devscripts lintian pkg-config dh-autoreconf"
        exit 1
    fi
}

check_deps

echo "Building .deb for librcunit-dev ..."

# Clean previous build artifacts one level up (dpkg-buildpackage writes there)
rm -f ../librcunit-dev_*.deb ../librcunit-dev_*.changes ../librcunit-dev_*.buildinfo

dpkg-buildpackage -us -uc -b

mkdir -p dist
mv ../librcunit-dev_*.deb dist/

echo ""
echo "Package built:"
ls -lh dist/*.deb

echo ""
echo "Linting ..."
lintian dist/*.deb || true

echo ""
echo "Package contents:"
dpkg -c dist/*.deb
