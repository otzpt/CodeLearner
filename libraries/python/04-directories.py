"""
TITLE: Listing a directory
GROUP: Basics
USES: os.scandir, pathlib.Path
SUMMARY: os.scandir() and Path.iterdir() list the entries of a directory.
  Counting entries is how a package count or a list of network devices is made.
NOTES:
  - Entries come in no particular order: sorted() them if order matters.
  - Path("/sys/class/net").iterdir() yields Path objects; entry.name is the
    file name, entry.is_dir() and entry.is_file() say what it is.
  - Path.glob("card*") matches names with a pattern, Path.rglob() recurses.
  - A missing directory raises FileNotFoundError; an unreadable one
    PermissionError. Catch OSError to cover both.
  - Symbolic links are common under /sys: entry.is_symlink(), os.readlink().
SEE: pydoc pathlib.Path.iterdir, pydoc os.scandir
"""

from pathlib import Path


def count_entries(path):
    try:
        return sum(1 for _ in Path(path).iterdir())
    except OSError:
        return None


for path in ("/sys/class/net", "/proc/self/fd", "/no/such/directory"):
    count = count_entries(path)
    print(f"{path:<22}", "cannot open" if count is None else f"{count} entries")

print("net devices:", sorted(entry.name for entry in Path("/sys/class/net").iterdir()))
print("drm cards  :", sorted(p.name for p in Path("/sys/class/drm").glob("card[0-9]*") if "-" not in p.name))
