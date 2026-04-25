#!/usr/bin/env bash
# Run this once locally to generate the apt repo signing key.
# Then add GPG_PRIVATE_KEY as a GitHub Actions secret.
set -euo pipefail

KEY_NAME="rcunit apt signing key"
KEY_EMAIL="jecklgamis@gmail.com"
KEY_FILE="rcunit-signing-key.asc"

echo "Generating GPG key (no passphrase) ..."
gpg --batch --gen-key <<EOF
%no-protection
Key-Type: RSA
Key-Length: 4096
Name-Real: ${KEY_NAME}
Name-Email: ${KEY_EMAIL}
Expire-Date: 0
%commit
EOF

FINGERPRINT=$(gpg --list-secret-keys --with-colons "${KEY_EMAIL}" \
  | awk -F: '/^fpr/{print $10; exit}')

echo "Key fingerprint: ${FINGERPRINT}"

gpg --armor --export-secret-keys "${FINGERPRINT}" > "${KEY_FILE}"
echo ""
echo "Done. Next steps:"
echo "  1. Go to: https://github.com/jecklgamis/rcunit/settings/secrets/actions"
echo "  2. Add secret  GPG_PRIVATE_KEY  with the contents of ${KEY_FILE}"
echo "  3. Enable GitHub Pages (Settings > Pages > Source: gh-pages branch)"
echo ""
echo "Users will install with:"
echo "  curl -fsSL https://jecklgamis.github.io/rcunit/public.key | sudo gpg --dearmor -o /etc/apt/keyrings/rcunit.gpg"
echo "  echo 'deb [signed-by=/etc/apt/keyrings/rcunit.gpg] https://jecklgamis.github.io/rcunit stable main' | sudo tee /etc/apt/sources.list.d/rcunit.list"
echo "  sudo apt-get update && sudo apt-get install librcunit-dev"
