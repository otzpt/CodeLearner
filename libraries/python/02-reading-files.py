"""
TITLE: Reading a text file
GROUP: Basics
USES: built-in open(), pathlib.Path
SUMMARY: open() in a with block reads a file and closes it afterwards.
  Iterating over the file gives one line at a time. Path.read_text() reads
  it all at once.
NOTES:
  - Always use `with open(...) as f`: the file is closed even if an exception
    happens.
  - Lines keep their newline character: use line.rstrip() or line.strip().
  - A missing file raises FileNotFoundError, a forbidden one PermissionError.
    Both are subclasses of OSError.
  - Files under /proc and /sys report a size of 0 but have content: read them
    to the end, never by size.
  - Pass encoding="utf-8" for text you did not write yourself, so the result
    does not depend on the machine's locale.
SEE: pydoc open, pydoc pathlib.Path.read_text
"""

from pathlib import Path

with open("/proc/loadavg", encoding="utf-8") as file:
    for line in file:
        print("read:", repr(line.rstrip("\n")))

meminfo = Path("/proc/meminfo")
print("size reported by stat:", meminfo.stat().st_size)
print("characters actually read:", len(meminfo.read_text()))

try:
    open("/no/such/file", encoding="utf-8")
except FileNotFoundError as error:
    print("FileNotFoundError:", error.strerror)
