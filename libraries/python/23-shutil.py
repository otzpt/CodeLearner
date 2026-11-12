"""
TITLE: shutil: high-level file operations
GROUP: Files, paths and processes
LIBRARY: Python standard library (shutil)
USES: import shutil
SUMMARY: Copying, moving and removing files and trees, finding programs on
  PATH, disk usage and the terminal size.
ENTRY: shutil.which(cmd, mode=1, path=None)
   Given a command, mode, and a PATH string, return the path which
   conforms to the given mode on the PATH, or None if there is no
   such file.
ENTRY: shutil.copy(src, dst, *, follow_symlinks=True)
   Copy data and mode bits ("cp src dst"). Return the file's
   destination.
ENTRY: shutil.copy2(src, dst, *, follow_symlinks=True)
   Copy data and metadata. Return the file's destination.
ENTRY: shutil.copyfile(src, dst, *, follow_symlinks=True)
   Copy data from src to dst in the most efficient way possible.
ENTRY: shutil.copytree(src, dst, symlinks=False, ignore=None,
    copy_function=<function copy2 at 0x7fbe844db8a0>,
    ignore_dangling_symlinks=False, dirs_exist_ok=False)
   Recursively copy a directory tree and return the destination
   directory.
ENTRY: shutil.move(src, dst, copy_function=<function copy2 at 0x7fbe844db8a0>)
   Recursively move a file or directory to another location. This is
   similar to the Unix "mv" command. Return the file or directory's
   destination.
ENTRY: shutil.rmtree(path, ignore_errors=False, onerror=None, *, onexc=None,
    dir_fd=None)
   Recursively delete a directory tree.
ENTRY: shutil.disk_usage(path)
   Return disk usage statistics about the given path.
ENTRY: shutil.get_terminal_size(fallback=(80, 24))
   Get the size of the terminal window.
ENTRY: shutil.make_archive(base_name, format, root_dir=None, base_dir=None,
    verbose=0, dry_run=0, owner=None, group=None, logger=None)
   Create an archive file (eg. zip or tar).
NOTES:
  - which() returns None when the program is not found; it follows
    PATH like the shell.
  - disk_usage().used + .free is less than .total on ext4: root
    reserves some blocks. Percentages usually use used + free.
  - get_terminal_size() never raises: it falls back to (80, 24)
    when stdout is not a terminal.
  - rmtree() deletes without asking. Check the path before you
    call it.
  - Signatures and descriptions are Python's own, read from the
    module's docstrings for the version that runs this page.
    Version notes (added, changed, deprecated) are in the online
    documentation.
SEE: https://docs.python.org/3/library/shutil.html, pydoc shutil
"""

import shutil

print("which('python3'):", shutil.which("python3"))
print("which('no-such-program'):", shutil.which("no-such-program"))

usage = shutil.disk_usage("/")
shown_total = usage.used + usage.free
print(f"/   {usage.used / 2**30:.1f} GiB / {shown_total / 2**30:.1f} GiB ({usage.used / shown_total * 100:.0f}%)")
print(f"raw total {usage.total / 2**30:.1f} GiB; reserved for root {(usage.total - shown_total) / 2**30:.1f} GiB")

size = shutil.get_terminal_size(fallback=(80, 24))
print("terminal size:", tuple(size), "(an environment variable COLUMNS / LINES overrides, then the terminal, then the fallback)")
