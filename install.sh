#!/bin/sh
# CodeLearner installer -- any Linux distribution, x86-64.
#
#   curl -fsSL https://raw.githubusercontent.com/otzpt/CodeLearner/main/install.sh | sh
#   wget -qO-  https://raw.githubusercontent.com/otzpt/CodeLearner/main/install.sh | sh
#
# Installs the latest release into your home directory and puts a
# `codelearner` command on your PATH. No sudo, no package manager, so it
# does not care which distribution this is -- the .deb and Arch packages on
# the Releases page are the native-packaging path, and this is the one for
# everything else.
#
#   --uninstall        remove it again
#   --prefix DIR       install somewhere else (default: ~/.local)
#
# POSIX sh on purpose: this runs before we know anything about the machine,
# including whether bash exists.

set -eu

REPO="otzpt/CodeLearner"
ASSET="codelearner-linux-x86_64.tar.gz"
BUNDLE_DIR="codelearner-linux-x86_64"

PREFIX="${HOME}/.local"
ACTION="install"

while [ $# -gt 0 ]; do
    case "$1" in
        --uninstall) ACTION="uninstall" ;;
        --prefix)
            [ $# -ge 2 ] || { echo "install.sh: --prefix needs a directory" >&2; exit 1; }
            PREFIX="$2"
            shift
            ;;
        -h|--help)
            cat <<'USAGE'
CodeLearner installer -- any Linux distribution, x86-64.

  --uninstall        remove it again
  --prefix DIR       install somewhere else (default: ~/.local)
USAGE
            exit 0
            ;;
        *) echo "install.sh: unknown option: $1" >&2; exit 1 ;;
    esac
    shift
done

# The wrapper and the menu entry store this path, so it has to work from any
# directory, not just the one this script was started in.
case "$PREFIX" in
    /*) ;;
    *) PREFIX="$(pwd)/$PREFIX" ;;
esac

SHARE_DIR="${PREFIX}/share/codelearner"
BIN_DIR="${PREFIX}/bin"
WRAPPER="${BIN_DIR}/codelearner"
APP_ID="com.otzpt.codelearner"
APPS_DIR="${PREFIX}/share/applications"
ICON_DIR="${PREFIX}/share/icons/hicolor/512x512/apps"

if [ "$ACTION" = "uninstall" ]; then
    rm -rf "$SHARE_DIR"
    rm -f "$WRAPPER" "$APPS_DIR/$APP_ID.desktop" "$ICON_DIR/$APP_ID.png"
    echo "Removed $SHARE_DIR, $WRAPPER and the application menu entry"
    exit 0
fi

arch="$(uname -m)"
if [ "$arch" != "x86_64" ] && [ "$arch" != "amd64" ]; then
    echo "install.sh: the prebuilt release is x86-64 only, and this is $arch." >&2
    echo "Build from source instead: https://github.com/${REPO}#running" >&2
    exit 1
fi

if [ "$(uname -s)" != "Linux" ]; then
    echo "install.sh: this installer is for Linux. On Windows use install.ps1." >&2
    exit 1
fi

# curl on most machines, wget on the ones that ship that instead. Whichever
# is here is the one used to fetch the tarball.
if command -v curl >/dev/null 2>&1; then
    fetch() { curl -fsSL "$1" -o "$2"; }
elif command -v wget >/dev/null 2>&1; then
    fetch() { wget -qO "$2" "$1"; }
else
    echo "install.sh: needs curl or wget, and found neither." >&2
    exit 1
fi

# /releases/latest/download/ is a redirect GitHub maintains to whatever the
# newest release is, so there is no API call and no JSON to parse here.
URL="https://github.com/${REPO}/releases/latest/download/${ASSET}"

tmp="$(mktemp -d)"
# Leaving a half-extracted tree in /tmp on a failed download helps nobody.
trap 'rm -rf "$tmp"' EXIT INT TERM

echo "Downloading $ASSET..."
fetch "$URL" "$tmp/$ASSET"

# SIMPLIFICATION: a missing .sha256 only warns, because releases made before
# this check do not publish one. Make it fatal once the latest release does.
# The file comes from the same release as the tarball, so this catches a
# damaged download or a replaced tarball, not a compromised release.
if ! command -v sha256sum >/dev/null 2>&1; then
    echo "install.sh: sha256sum not found, skipping the checksum check." >&2
elif fetch "$URL.sha256" "$tmp/$ASSET.sha256" 2>/dev/null; then
    expected="$(cut -d' ' -f1 "$tmp/$ASSET.sha256")"
    actual="$(sha256sum "$tmp/$ASSET" | cut -d' ' -f1)"
    if [ "$expected" != "$actual" ]; then
        echo "install.sh: checksum mismatch for $ASSET, not installing it." >&2
        exit 1
    fi
    echo "Checksum OK."
else
    echo "install.sh: this release publishes no checksum, skipping the check." >&2
fi

echo "Extracting..."
tar -xzf "$tmp/$ASSET" -C "$tmp"
[ -d "$tmp/$BUNDLE_DIR" ] || {
    echo "install.sh: the archive did not contain $BUNDLE_DIR as expected." >&2
    exit 1
}

# Replace any previous install rather than merging into it: a release that
# drops a course would otherwise leave the old one behind, still listed by
# the launcher and no longer supported by anything around it.
rm -rf "$SHARE_DIR"
mkdir -p "$(dirname "$SHARE_DIR")" "$BIN_DIR"
mv "$tmp/$BUNDLE_DIR" "$SHARE_DIR"

# tar preserves the modes set when the release was built, but a umask or a
# filesystem without an exec bit (a FAT-formatted stick, say) can still
# arrive here without them, and every one of these is useless unless
# executable.
for exe in launcher/launcher libraries/libraries "$SHARE_DIR"/*/learn.js; do
    exe=${exe#"$SHARE_DIR/"}
    [ -f "$SHARE_DIR/$exe" ] && chmod +x "$SHARE_DIR/$exe"
done
rm -f "$SHARE_DIR/codelearner.sh"     # the wrapper below replaces it

# The launcher finds every course by a path relative to its own directory
# (../c, ../python, ...), so it has to be started from inside that layout --
# the same reason the .deb and Arch packages ship a wrapper instead of
# symlinking the binary onto PATH.
#
# Single quotes keep every character of the path literal; a ' inside it is
# closed, escaped and reopened.
quoted_dir="'$(printf '%s' "$SHARE_DIR/launcher" | sed "s/'/'\\\\''/g")'"
printf '#!/bin/sh\ncd %s && exec ./launcher\n' "$quoted_dir" > "$WRAPPER"
chmod +x "$WRAPPER"

# An application menu entry, so CodeLearner can be started without a terminal
# already open. Releases before this one do not carry the files, so it is
# optional. Exec= is rewritten to the wrapper's full path (quoted, in case the
# prefix has a space in it) because a desktop session does not always have
# ~/.local/bin on its PATH.
if [ -f "$SHARE_DIR/desktop/$APP_ID.desktop" ]; then
    mkdir -p "$APPS_DIR" "$ICON_DIR"
    # Desktop Entry spec: inside quotes \ " ` $ take a backslash, the value as
    # a whole then takes string escaping (\ becomes \\), and % is written %%.
    exec_path="$(printf '%s' "$WRAPPER" | sed -e 's/[\\"`$]/\\&/g' -e 's/\\/\\\\/g' -e 's/%/%%/g')"
    while IFS= read -r line || [ -n "$line" ]; do
        case "$line" in
            Exec=*) printf 'Exec="%s"\n' "$exec_path" ;;
            *)      printf '%s\n' "$line" ;;
        esac
    done < "$SHARE_DIR/desktop/$APP_ID.desktop" > "$APPS_DIR/$APP_ID.desktop"
    cp "$SHARE_DIR/desktop/$APP_ID.png" "$ICON_DIR/$APP_ID.png"

    # GTK trusts an existing icon-theme.cache and does not see an icon added
    # after it was written, so the menu entry shows no logo. Only a cache that
    # is already there is refreshed: without one GTK scans the directories.
    # -t because a user-level hicolor has no index.theme of its own.
    ICON_ROOT="${PREFIX}/share/icons/hicolor"
    if [ -f "$ICON_ROOT/icon-theme.cache" ] && command -v gtk-update-icon-cache >/dev/null 2>&1; then
        gtk-update-icon-cache -q -f -t "$ICON_ROOT" ||
            echo "install.sh: could not refresh the icon cache, the menu icon may be missing." >&2
    fi
fi

echo
echo "Installed to $SHARE_DIR"
echo "Command:     $WRAPPER"

case ":${PATH}:" in
    *":${BIN_DIR}:"*)
        echo
        echo "Run it with:  codelearner"
        ;;
    *)
        echo
        echo "$BIN_DIR is not on your PATH yet. Add it:"
        echo
        echo "    export PATH=\"$BIN_DIR:\$PATH\""
        echo
        echo "Put that line in ~/.bashrc, ~/.zshrc, or ~/.profile to keep it."
        echo "Until then, run it with:  $WRAPPER"
        ;;
esac

# Every course runs on Node. Installing it is the system's package manager's
# job, not this script's, so only say so.
if ! command -v node >/dev/null 2>&1; then
    echo
    echo "The courses need Node.js, which is not installed. Install it with your"
    echo "system's package manager (the package is usually called nodejs)."
fi
