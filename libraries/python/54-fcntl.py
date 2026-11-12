"""
TITLE: fcntl: fcntl and ioctl system calls
GROUP: System information
LIBRARY: Python standard library (fcntl)
USES: import fcntl
SUMMARY: File-descriptor control (fcntl), device control requests (ioctl)
  and file locking (flock, lockf). Unix only. ioctl request numbers
  and structures are system-specific.
ENTRY: fcntl.fcntl(fd, cmd, arg=0, /)
   Perform the operation cmd on file descriptor fd.
ENTRY: fcntl.ioctl(fd, request, arg=0, mutate_flag=True, /)
   Perform the operation request on file descriptor fd.
ENTRY: fcntl.flock(fd, operation, /)
   Perform the lock operation on file descriptor fd.
ENTRY: fcntl.lockf(fd, cmd, len=0, start=0, whence=0, /)
   A wrapper around the fcntl() locking calls.
NOTES:
  - Locks from flock() are advisory: they only stop other
    processes that also take the lock.
  - Read ioctl request numbers from termios (termios.TIOCGWINSZ),
    not from a copy typed out of a header.
  - fcntl.ioctl() takes a bytes buffer and returns the filled
    bytes.
  - Signatures and descriptions are Python's own, read from the
    module's docstrings for the version that runs this page.
    Version notes (added, changed, deprecated) are in the online
    documentation.
SEE: https://docs.python.org/3/library/fcntl.html, pydoc fcntl
"""

import fcntl
import os
import struct
import tempfile
import termios

# fcntl: read the flags of a file descriptor.
descriptor = os.open("/dev/null", os.O_RDONLY)
flags = fcntl.fcntl(descriptor, fcntl.F_GETFL)
print("F_GETFL of /dev/null opened O_RDONLY:", flags, " access mode is O_RDONLY:", flags & os.O_ACCMODE == os.O_RDONLY)
os.close(descriptor)

# flock: an advisory lock on a file.
with tempfile.TemporaryFile() as lockfile:
    fcntl.flock(lockfile, fcntl.LOCK_EX | fcntl.LOCK_NB)
    print("got an exclusive lock on a temporary file")
    fcntl.flock(lockfile, fcntl.LOCK_UN)

# ioctl: ask the controlling terminal for its size (TIOCGWINSZ).
try:
    tty = os.open("/dev/tty", os.O_RDONLY)
except OSError:
    print("no controlling terminal to ask")
else:
    try:
        rows, columns, _x, _y = struct.unpack("HHHH", fcntl.ioctl(tty, termios.TIOCGWINSZ, b"\0" * 8))
        print("terminal size from ioctl:", columns, "x", rows)
    finally:
        os.close(tty)
