"""
TITLE: errno: error numbers
GROUP: Files, paths and processes
LIBRARY: Python standard library (errno)
USES: import errno
SUMMARY: The numeric codes the operating system uses for failures (the C
  errno). An OSError has .errno; this module names the numbers and
  errno.errorcode maps a number back to its name.
ENTRY: errno.errorcode
   dict() -> new empty dictionary dict(mapping) -> new dictionary
   initialized from a mapping object's (key, value) pairs
   dict(iterable) -> new dictionary initialized as if via: d = {} for
   k, v in iterable: d[k] = v dict(**kwargs) -> new dictionary
   initialized with the name=value pairs in the keyword argument
   list. For example: dict(one=1, two=2)
ENTRY: errno.ENOENT = 2
   No such file or directory. The matching exception is
   FileNotFoundError.
ENTRY: errno.EACCES = 13
   Permission denied. The matching exception is PermissionError.
ENTRY: errno.EEXIST = 17
   File exists. The matching exception is FileExistsError.
ENTRY: errno.EINVAL = 22
   Invalid argument. The matching exception is OSError.
ENTRY: errno.EPERM = 1
   Operation not permitted. The matching exception is
   PermissionError.
ENTRY: errno.EAGAIN = 11
   Resource temporarily unavailable. The matching exception is
   BlockingIOError.
ENTRY: errno.EINTR = 4
   Interrupted system call. The matching exception is
   InterruptedError.
ENTRY: errno.ENOMEM = 12
   Cannot allocate memory. The matching exception is OSError.
ENTRY: errno.EPIPE = 32
   Broken pipe. The matching exception is BrokenPipeError.
ENTRY: errno.ENOTDIR = 20
   Not a directory. The matching exception is NotADirectoryError.
ENTRY: errno.EISDIR = 21
   Is a directory. The matching exception is IsADirectoryError.
ENTRY: errno.ENOSPC = 28
   No space left on device. The matching exception is OSError.
ENTRY: errno.EBADF = 9
   Bad file descriptor. The matching exception is OSError.
ENTRY: errno.ECHILD = 10
   No child processes. The matching exception is ChildProcessError.
ENTRY: errno.ETIMEDOUT = 110
   Connection timed out. The matching exception is TimeoutError.
ENTRY: errno.ECONNREFUSED = 111
   Connection refused. The matching exception is
   ConnectionRefusedError.
NOTES:
  - Compare error.errno with errno.ENOENT, never with a number:
    values differ between systems.
  - In most code, catch the subclass (FileNotFoundError,
    PermissionError) instead of testing errno.
  - The numbers shown are this machine's; Linux and macOS disagree
    on some.
  - Signatures and descriptions are Python's own, read from the
    module's docstrings for the version that runs this page.
    Version notes (added, changed, deprecated) are in the online
    documentation.
SEE: https://docs.python.org/3/library/errno.html, man 3 errno
"""

import errno
import os

for code in (errno.ENOENT, errno.EACCES, errno.EEXIST, errno.EPIPE, errno.ENOSPC):
    print(f"{code:>3} {errno.errorcode[code]:<8} {os.strerror(code)}")

try:
    os.mkdir("/proc/forbidden")
except OSError as error:
    print("os.mkdir('/proc/forbidden'):", type(error).__name__, error.errno, errno.errorcode[error.errno])
print("errorcode is a plain dict with", len(errno.errorcode), "entries on this system")
