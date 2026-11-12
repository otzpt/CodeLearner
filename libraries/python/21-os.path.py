"""
TITLE: os.path: pathname manipulation
GROUP: Files, paths and processes
LIBRARY: Python standard library (os.path)
USES: import os   # then os.path
SUMMARY: Functions on path strings: join, split, test existence, expand ~.
  They work on strings and mostly do not touch the disk. The pathlib
  module offers the same operations as methods on Path objects.
ENTRY: os.path.join(a, *p)
   Join two or more pathname components, inserting '/' as needed. If
   any component is an absolute path, all previous path components
   will be discarded. An empty last part will result in a path that
   ends with a separator.
ENTRY: os.path.split(p)
   Split a pathname. Returns tuple "(head, tail)" where "tail" is
   everything after the final slash. Either part may be empty.
ENTRY: os.path.splitext(p)
   Split the extension from a pathname.
ENTRY: os.path.basename(p)
   Returns the final component of a pathname
ENTRY: os.path.dirname(p)
   Returns the directory component of a pathname
ENTRY: os.path.abspath(path)
   Return an absolute path.
ENTRY: os.path.realpath(filename, *, strict=False)
   Return the canonical path of the specified filename, eliminating
   any symbolic links encountered in the path.
ENTRY: os.path.normpath(path)
   Normalize path, eliminating double slashes, etc.
ENTRY: os.path.relpath(path, start=None)
   Return a relative version of a path
ENTRY: os.path.expanduser(path)
   Expand ~ and ~user constructions. If user or $HOME is unknown, do
   nothing.
ENTRY: os.path.exists(path)
   Test whether a path exists. Returns False for broken symbolic
   links
ENTRY: os.path.isfile(path)
   Test whether a path is a regular file
ENTRY: os.path.isdir(s)
   Return true if the pathname refers to an existing directory.
ENTRY: os.path.islink(path)
   Test whether a path is a symbolic link
ENTRY: os.path.getsize(filename)
   Return the size of a file, reported by os.stat().
ENTRY: os.path.getmtime(filename)
   Return the last modification time of a file, reported by
   os.stat().
ENTRY: os.path.commonpath(paths)
   Given a sequence of path names, returns the longest common sub-
   path.
NOTES:
  - os.path.join('a', '/abs') returns '/abs': an absolute second
    part discards the first.
  - os.path.splitext('.bashrc') gives ('.bashrc', ''): a leading
    dot is not an extension.
  - exists() is false for a broken symlink; lexists() is true for
    it.
  - Files under /proc often report size 0 although reading them
    returns data.
  - Signatures and descriptions are Python's own, read from the
    module's docstrings for the version that runs this page.
    Version notes (added, changed, deprecated) are in the online
    documentation.
SEE: https://docs.python.org/3/library/os.path.html, pydoc os.path
"""

import os.path

path = "/usr/local/lib/python3/site-packages/../json/__init__.py"
print("join       :", os.path.join("/usr", "lib", "python3"), os.path.join("a", "/abs"))
print("split      :", os.path.split("/usr/lib/libc.so.6"))
print("splitext   :", os.path.splitext("archive.tar.gz"), os.path.splitext(".bashrc"))
print("basename   :", os.path.basename("/usr/lib/"), "| dirname:", os.path.dirname("/usr/lib/libc.so"))
print("normpath   :", os.path.normpath(path))
print("expanduser :", os.path.expanduser("~") == os.environ.get("HOME"))
print("exists/isdir/isfile of /proc:", os.path.exists("/proc"), os.path.isdir("/proc"), os.path.isfile("/proc"))
print("relpath    :", os.path.relpath("/usr/lib/x", "/usr"))
print("commonpath :", os.path.commonpath(["/usr/lib/a", "/usr/share/b"]))
print("getsize of /proc/self/status is", os.path.getsize("/proc/self/status"), "(procfs reports 0)")
