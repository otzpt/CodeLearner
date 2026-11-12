"""
TITLE: sys: the interpreter and its streams
GROUP: Files, paths and processes
LIBRARY: Python standard library (sys)
USES: import sys
SUMMARY: Parameters and functions of the running interpreter: command-line
  arguments, the standard streams, the module search path, the
  version, and how to exit.
ENTRY: sys.argv = [...]
   The list of command-line arguments passed to the script.
   sys.argv[0] is the script name; the rest are the arguments, as
   strings.
ENTRY: sys.exit(status=None, /)
   Exit the interpreter by raising SystemExit(status).
ENTRY: sys.platform = 'linux'
   A string identifying the platform: 'linux', 'darwin', 'win32'.
   Compare with startswith('linux') on older Pythons.
ENTRY: sys.version = '3.14.8 ...'
   A string with the interpreter's version number and build
   information.
ENTRY: sys.version_info = sys.version_info(major=..., minor=..., micro=...,
    releaselevel=..., serial=...)
   The version of the running interpreter as a named tuple. Compare
   tuples: sys.version_info >= (3, 10).
ENTRY: sys.executable = '/usr/bin/python3'
   The absolute path of the running Python interpreter.
ENTRY: sys.path = [...]
   The list of directories searched for modules to import. The first
   entry is the script's own directory.
ENTRY: sys.stdin = <_io.TextIOWrapper ...>
   The file object for standard input.
ENTRY: sys.stdout = <_io.TextIOWrapper ...>
   The file object for standard output. print() writes here. Its
   isatty() tells whether it is a terminal.
ENTRY: sys.stderr = <_io.TextIOWrapper ...>
   The file object for standard error, for messages that must not be
   mixed into piped output.
ENTRY: sys.byteorder = 'little'
   'little' or 'big': the byte order of this machine.
ENTRY: sys.maxsize = 9223372036854775807
   The largest value a Py_ssize_t can hold: the largest list index
   plus one on this build.
ENTRY: sys.getsizeof(object [, default]) -> int
   (no description in the module's docstring)
ENTRY: sys.getdefaultencoding()
   Return the current default encoding used by the Unicode
   implementation.
ENTRY: sys.getrecursionlimit()
   Return the current value of the recursion limit.
NOTES:
  - sys.exit('message') prints the message to stderr and exits
    with status 1.
  - sys.argv elements are strings; convert with int() where you
    need numbers. For options use argparse.
  - Compare versions as tuples: sys.version_info >= (3, 10), never
    as strings.
  - Signatures and descriptions are Python's own, read from the
    module's docstrings for the version that runs this page.
    Version notes (added, changed, deprecated) are in the online
    documentation.
SEE: https://docs.python.org/3/library/sys.html, pydoc sys
"""

import sys

print("platform :", sys.platform, " byteorder:", sys.byteorder)
print("python   :", ".".join(map(str, sys.version_info[:3])), " >= 3.10:", sys.version_info >= (3, 10))
print("argv     :", sys.argv[1:] or "(no arguments)")
print("stdout is a terminal:", sys.stdout.isatty())
print("default encoding:", sys.getdefaultencoding(), " stdout encoding:", sys.stdout.encoding)
print("getsizeof(0), getsizeof('abc'):", sys.getsizeof(0), sys.getsizeof("abc"))
print("recursion limit:", sys.getrecursionlimit())
print("first sys.path entry is this script's folder:", bool(sys.path[0]))
sys.stderr.write("(this line went to stderr)\n")
sys.exit(0)        # same as raising SystemExit(0); the exit status is 0
