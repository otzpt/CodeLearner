"""
TITLE: os: operating system interfaces
GROUP: Files, paths and processes
LIBRARY: Python standard library (os)
USES: import os
SUMMARY: Access to the operating system: environment variables, the working
  directory, files and directories, process ids and the system's
  identity. Paths are best handled with pathlib; os is where the
  system calls live.
ENTRY: os.environ = environ({...})
   A mapping of the environment variables, as strings. Read with
   os.environ['HOME'] (KeyError if missing) or os.environ.get('HOME',
   default). Changing it affects child processes.
ENTRY: os.name = 'posix'
   The operating system module in use: 'posix', 'nt' or 'java'.
ENTRY: os.getenv(key, default=None)
   Get an environment variable, return None if it doesn't exist. The
   optional second argument can specify an alternate default. key,
   default and the result are str.
ENTRY: os.getcwd()
   Return a unicode string representing the current working
   directory.
ENTRY: os.chdir(path)
   Change the current working directory to the specified path.
ENTRY: os.listdir(path=None)
   Return a list containing the names of the files in the directory.
ENTRY: os.scandir(path=None)
   Return an iterator of DirEntry objects for given path.
ENTRY: os.walk(top, topdown=True, onerror=None, followlinks=False)
   Directory tree generator.
ENTRY: os.stat(path, *, dir_fd=None, follow_symlinks=True)
   Perform a stat system call on the given path.
ENTRY: os.mkdir(path, mode=511, *, dir_fd=None)
   Create a directory.
ENTRY: os.makedirs(name, mode=511, exist_ok=False)
   (no description in the module's docstring)
ENTRY: os.remove(path, *, dir_fd=None)
   Remove a file (same as unlink()).
ENTRY: os.rename(src, dst, *, src_dir_fd=None, dst_dir_fd=None)
   Rename a file or directory.
ENTRY: os.access(path, mode, *, dir_fd=None, effective_ids=False,
    follow_symlinks=True)
   Use the real uid/gid to test for access to a path.
ENTRY: os.getpid()
   Return the current process id.
ENTRY: os.getppid()
   Return the parent's process id.
ENTRY: os.getuid()
   Return the current process's user id.
ENTRY: os.uname()
   Return an object identifying the current operating system.
ENTRY: os.getloadavg()
   Return average recent system load information.
ENTRY: os.cpu_count()
   Return the number of logical CPUs in the system.
ENTRY: os.urandom(size, /)
   Return a bytes object containing random bytes suitable for
   cryptographic use.
ENTRY: os.strerror(code, /)
   Translate an error code to a message string.
ENTRY: os.get_terminal_size
   Return the size of the terminal window as (columns, lines).
ENTRY: os.isatty(fd, /)
   Return True if the fd is connected to a terminal.
NOTES:
  - os.environ['X'] raises KeyError if X is unset;
    os.environ.get('X', default) does not.
  - os.uname() gives the kernel's name, release and machine.
    os.uname().nodename is the host name.
  - os.scandir() is faster than os.listdir() when you need file
    types: DirEntry caches them.
  - os.getloadavg() is the 1, 5 and 15 minute load average; it
    raises OSError where unavailable.
  - os.urandom() is for cryptographic random bytes; the random
    module is not.
  - Signatures and descriptions are Python's own, read from the
    module's docstrings for the version that runs this page.
    Version notes (added, changed, deprecated) are in the online
    documentation.
SEE: https://docs.python.org/3/library/os.html, pydoc os
"""

import os

print("HOME      :", os.environ.get("HOME", "(not set)"))
print("missing   :", os.environ.get("NO_SUCH_VARIABLE_HERE", "(not set)"))
print("pid / ppid:", os.getpid(), "/", os.getppid(), "  uid:", os.getuid())
print("cwd is a directory:", os.path.isdir(os.getcwd()))
print("cpu_count :", os.cpu_count())

name = os.uname()
print("uname     :", name.sysname, name.release, name.machine, "node", name.nodename)
print("load      : %.2f %.2f %.2f" % os.getloadavg())

with os.scandir("/sys/class/net") as entries:
    print("net devices:", sorted(entry.name for entry in entries))
print("listdir of a missing path:", end=" ")
try:
    os.listdir("/no/such/directory")
except OSError as error:
    print(type(error).__name__, "-", os.strerror(error.errno))
print("urandom(4) has", len(os.urandom(4)), "bytes;  stdout is a tty:", os.isatty(1))
