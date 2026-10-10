"""
TITLE: Environment, arguments and the interpreter (os, sys)
GROUP: Basics
USES: import os, sys
SUMMARY: os.environ is the environment as a dict, sys.argv the command-line
  arguments, sys.platform and sys.version_info say where the program runs.
NOTES:
  - os.environ.get("HOME") returns None when the variable is missing;
    os.environ["HOME"] raises KeyError. Use get() with a default.
  - sys.argv[0] is the program's own name; the arguments start at index 1.
  - sys.platform is "linux" on Linux; check it before reading /proc.
  - sys.exit(1) ends the program with a failure status that scripts can test.
  - os.getpid() and os.getppid() are this process and its parent.
SEE: pydoc os.environ, pydoc sys
"""

import os
import sys

print("HOME      :", os.environ.get("HOME", "(not set)"))
print("MISSING   :", os.environ.get("NO_SUCH_VARIABLE_HERE", "(not set)"))
print("platform  :", sys.platform)
print("python    :", ".".join(map(str, sys.version_info[:3])))
print("argv      :", sys.argv[1:] or "(no arguments)")
print("pid / ppid:", os.getpid(), "/", os.getppid())
print("cwd is a directory:", os.path.isdir(os.getcwd()))
