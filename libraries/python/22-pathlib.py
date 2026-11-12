"""
TITLE: pathlib: object-oriented paths
GROUP: Files, paths and processes
LIBRARY: Python standard library (pathlib)
USES: import pathlib
SUMMARY: A Path is a path with methods: join with /, read and write files,
  list and search directories, ask about the file. It is the usual
  way to handle paths in new code.
ENTRY: pathlib.Path(*args, **kwargs)
   PurePath subclass that can make system calls.
ENTRY: pathlib.Path.home()
   Return a new path pointing to expanduser('~').
ENTRY: pathlib.Path.cwd()
   Return a new path pointing to the current working directory.
ENTRY: pathlib.Path.exists(self, *, follow_symlinks=True)
   Whether this path exists.
ENTRY: pathlib.Path.is_file(self, *, follow_symlinks=True)
   Whether this path is a regular file (also True for symlinks
   pointing to regular files).
ENTRY: pathlib.Path.is_dir(self, *, follow_symlinks=True)
   Whether this path is a directory.
ENTRY: pathlib.Path.read_text(self, encoding=None, errors=None, newline=None)
   Open the file in text mode, read it, and close the file.
ENTRY: pathlib.Path.write_text(self, data, encoding=None, errors=None,
    newline=None)
   Open the file in text mode, write to it, and close the file.
ENTRY: pathlib.Path.read_bytes(self)
   Open the file in bytes mode, read it, and close the file.
ENTRY: pathlib.Path.iterdir(self)
   Yield path objects of the directory contents.
ENTRY: pathlib.Path.glob(self, pattern, *, case_sensitive=None,
    recurse_symlinks=False)
   Iterate over this subtree and yield all existing files (of any
   kind, including directories) matching the given relative pattern.
ENTRY: pathlib.Path.rglob(self, pattern, *, case_sensitive=None,
    recurse_symlinks=False)
   Recursively yield all existing files (of any kind, including
   directories) matching the given relative pattern, anywhere in this
   subtree.
ENTRY: pathlib.Path.mkdir(self, mode=511, parents=False, exist_ok=False)
   Create a new directory at this given path.
ENTRY: pathlib.Path.unlink(self, missing_ok=False)
   Remove this file or link. If the path is a directory, use rmdir()
   instead.
ENTRY: pathlib.Path.rename(self, target)
   Rename this path to the target path.
ENTRY: pathlib.Path.resolve(self, strict=False)
   Make the path absolute, resolving all symlinks on the way and also
   normalizing it.
ENTRY: pathlib.Path.stat(self, *, follow_symlinks=True)
   Return the result of the stat() system call on this path, like
   os.stat() does.
ENTRY: pathlib.Path.joinpath(self, *pathsegments)
   Combine this path with one or several arguments, and return a new
   path representing either a subpath (if all arguments are relative
   paths) or a totally different path (if one of the arguments is
   anchored).
ENTRY: pathlib.Path.name
   The final path component, if any.
ENTRY: pathlib.Path.stem
   The final path component, minus its last suffix.
ENTRY: pathlib.Path.suffix
   The final component's last suffix, if any.
ENTRY: pathlib.Path.parent
   The logical parent of the path.
ENTRY: pathlib.Path.parts
   An object providing sequence-like access to the components in the
   filesystem path.
ENTRY: pathlib.Path.with_suffix(self, suffix)
   Return a new path with the file suffix changed. If the path has no
   suffix, add given suffix. If the given suffix is an empty string,
   remove the suffix from the path.
NOTES:
  - Always pass encoding='utf-8' to read_text() and write_text():
    the default depends on the locale.
  - iterdir() and glob() are lazy; the order of results is not
    sorted. Wrap them in sorted().
  - Path('a') / '/b' is Path('/b'): an absolute right side
    discards the left.
  - exists() hides permission errors by returning False; call
    stat() if you need to tell them apart.
  - Signatures and descriptions are Python's own, read from the
    module's docstrings for the version that runs this page.
    Version notes (added, changed, deprecated) are in the online
    documentation.
SEE: https://docs.python.org/3/library/pathlib.html, pydoc pathlib
"""

import tempfile
from pathlib import Path

path = Path("/usr/lib/python3/dist-packages/example.tar.gz")
print("name, stem, suffix:", path.name, path.stem, path.suffix, "  suffixes:", path.suffixes)
print("parent, parts[:3]  :", path.parent, path.parts[:3])
print("with_suffix        :", path.with_suffix(".zip"))
print("joined with /      :", Path("/etc") / "os-release")

print("net devices:", sorted(p.name for p in Path("/sys/class/net").iterdir()))
print("drm cards  :", sorted(p.name for p in Path("/sys/class/drm").glob("card[0-9]*") if "-" not in p.name))

with tempfile.TemporaryDirectory() as folder:
    note = Path(folder) / "note.txt"
    note.write_text("hello\n", encoding="utf-8")
    print("read_text:", repr(note.read_text(encoding="utf-8")), " size:", note.stat().st_size)
    note.unlink()
    print("exists after unlink:", note.exists())
try:
    Path("/no/such/dir").iterdir().__next__()
except OSError as error:
    print("iterdir on a missing path:", type(error).__name__)
