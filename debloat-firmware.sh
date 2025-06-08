#!/usr/bin/env bash
set -euo pipefail

# ── Variables ────────────────────────────────────────────────────────────────────
PACKAGE=sys-kernel/linux-firmware				# Valid atom for Portage
USEFILE_NAME=linux-firmware					# Clean filename (optional)
SAVEDCONFIG_DIR=/etc/portage/savedconfig/${PACKAGE%/*}		# e.g., /etc/portage/savedconfig/sys-kernel
SAVEDCONFIG_FILE=$SAVEDCONFIG_DIR/${PACKAGE##*/}		# e.g., linux-firmware
USEFILE="/etc/portage/package.use/$USEFILE_NAME"
TMPFILE=$(mktemp)

# ── Create the destination directory ─────────────────────────────────────────────
mkdir -p "$SAVEDCONFIG_DIR"

# ── Parse current dmesg ──────────────────────────────────────────────────────────
echo "-> Gathering firmware requests from dmesg…" >&2
dmesg \
	| grep -i 'Loading firmware:' \
	| sed -n 's/.*Loading firmware: \([^ ]*\).*/\1/p' \
	>> "$TMPFILE" 2>/dev/null || true

# ── Parse /var/log/dmesg (old boots) ─────────────────────────────────────────────
if [[ -f /var/log/dmesg ]]; then
	echo "-> Gathering firmware requests from /var/log/dmesg…" >&2
	grep -i 'Loading firmware:' /var/log/dmesg \
		| sed -n 's/.*Loading firmware: \([^ ]*\).*/\1/p' \
		>> "$TMPFILE" 2>/dev/null || true
fi

# ── Sort, deduplicate, and save ─────────────────────────────────────────────────
sort -u "$TMPFILE" > "$SAVEDCONFIG_FILE"
rm -f "$TMPFILE"

# ── Ensure USE=savedconfig is enabled ────────────────────────────────────────────
echo "-> Ensuring USE flag 'savedconfig' is set for $PACKAGE…" >&2

if [[ ! -f "$USEFILE" ]]; then
	echo "$PACKAGE savedconfig" > "$USEFILE"
	echo "Created $USEFILE with the flag: savedconfig"
else
	# ── Warn if '-savedconfig' is explicitly set ─────────────────────────────────────
	if grep -E -- '-savedconfig' /etc/portage/package.use/* 2>/dev/null | grep -q "$PACKAGE"; then
  		echo "	WARNING: You have explicitly disabled 'savedconfig' for $PACKAGE using '-savedconfig'."
		echo "	This will override the savedconfig and make this script ineffective."
		echo "	Please remove the '-savedconfig' flag to allow this script to work properly."
	elif ! grep -q "$PACKAGE" "$USEFILE"; then
		echo "$PACKAGE savedconfig" >> "$USEFILE"
		echo "Appended '$PACKAGE savedconfig' to $USEFILE"
	elif ! grep -q 'savedconfig' "$USEFILE"; then
		sed -i "/$PACKAGE/ s/$/ savedconfig/" "$USEFILE"
		echo "Appended 'savedconfig' to existing line in $USEFILE"
	else
		echo "'savedconfig' already enabled in $USEFILE"
	fi
fi

# ── Show the result ─────────────────────────────────────────────────────────────
cat <<EOF

🔹 Generated savedconfig at: $SAVEDCONFIG_FILE

$(sed 's/^/    /' "$SAVEDCONFIG_FILE")

Now review this file. When you’re happy, apply the configuration change with:
    
	1. emerge --config $PACKAGE
	2. emerge -C $SAVEDCONFIG_FILE
	3. emerge -a $SAVEDCONFIG_FILE

If you load new modules later (e.g. with modprobe), rerun this script
to capture their firmware requests.

EOF

echo "Script made by Flamitsu."

