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
            sed -n '2,18p' "$0" | sed 's/^# \{0,1\}//'
            exit 0
            ;;
        *) echo "install.sh: unknown option: $1" >&2; exit 1 ;;
    esac
    shift
done

SHARE_DIR="${PREFIX}/share/codelearner"
BIN_DIR="${PREFIX}/bin"
WRAPPER="${BIN_DIR}/codelearner"

if [ "$ACTION" = "uninstall" ]; then
    rm -rf "$SHARE_DIR"
    rm -f "$WRAPPER"
    echo "Removed $SHARE_DIR and $WRAPPER"
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
for exe in launcher/launcher c/c-course cpp/cpp-course gui/gui-course \
           assembly/asm-course csharp/csharp csharp/run java/run; do
    [ -f "$SHARE_DIR/$exe" ] && chmod +x "$SHARE_DIR/$exe"
done
rm -f "$SHARE_DIR/codelearner.sh"     # the wrapper below replaces it

# The launcher finds every course by a path relative to its own directory
# (../c, ../python, ...), so it has to be started from inside that layout --
# the same reason the .deb and Arch packages ship a wrapper instead of
# symlinking the binary onto PATH.
cat > "$WRAPPER" <<EOF
#!/bin/sh
cd "$SHARE_DIR/launcher" && exec ./launcher
EOF
chmod +x "$WRAPPER"

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
