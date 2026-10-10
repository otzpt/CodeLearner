"""
TITLE: Number of installed packages
GROUP: System information
USES: pathlib.Path; subprocess for rpm
SUMMARY: There is no single API. Each package manager keeps its own database,
  and "how many packages" means reading the right one. Count only the managers
  that exist on the machine.
NOTES:
  - Debian, Ubuntu, Mint: /var/lib/dpkg/status is a text file of blocks. Count
    the lines "Status: install ok installed".
  - Arch: one directory per package in /var/lib/pacman/local. Count the
    sub-directories (a file called ALPM_DB_VERSION sits there too).
  - Flatpak: one directory per app in /var/lib/flatpak/app and in
    ~/.local/share/flatpak/app.
  - Snap: one .snap file per installed snap in /var/lib/snapd/snaps. That
    count includes runtimes such as core22 and snapd itself.
  - Fedora, openSUSE (rpm): the database is SQLite or Berkeley DB. Run
    `rpm -qa` and count its lines, or use the sqlite3 module on the database.
  - The result is approximate: managers disagree on what counts as one.
SEE: man 5 deb-status, man 8 dpkg
"""

import shutil
import subprocess
from pathlib import Path


def count_dirs(path):
    try:
        return sum(1 for entry in Path(path).iterdir() if entry.is_dir())
    except OSError:
        return None


def count_dpkg():
    try:
        with open("/var/lib/dpkg/status", encoding="utf-8", errors="replace") as file:
            return sum(1 for line in file if line == "Status: install ok installed\n")
    except OSError:
        return None


def count_snaps():
    try:
        return sum(1 for entry in Path("/var/lib/snapd/snaps").iterdir() if entry.suffix == ".snap")
    except OSError:
        return None


def count_rpm():
    if shutil.which("rpm") is None:
        return None
    result = subprocess.run(["rpm", "-qa"], capture_output=True, text=True, timeout=30)
    return len(result.stdout.splitlines()) if result.returncode == 0 else None


counts = {
    "dpkg": count_dpkg(),
    "pacman": count_dirs("/var/lib/pacman/local"),
    "flatpak": count_dirs("/var/lib/flatpak/app"),
    "snap": count_snaps(),
    "rpm": count_rpm(),
}
shown = {name: count for name, count in counts.items() if count is not None}
for name, count in shown.items():
    print(f"{count} ({name})")
if not shown:
    print("no known package manager database found")
