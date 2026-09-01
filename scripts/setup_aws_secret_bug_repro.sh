#!/usr/bin/env bash
set -euo pipefail

root=${1:?usage: setup_aws_secret_bug_repro.sh OUTPUT_DIRECTORY}
mkdir -p "$root"
umask 077

cat >"$root/config" <<EOF
[profile role-repro]
role_arn = arn:aws:iam::123456789012:role/repro
source_profile = source-repro

EOF

cat >"$root/credentials" <<'EOF'
[source-repro]
aws_access_key_id = SOURCE_KEY
aws_secret_access_key = SOURCE_SECRET

[wrong-repro]
aws_access_key_id = WRONG_KEY
aws_secret_access_key = WRONG_SECRET

EOF

