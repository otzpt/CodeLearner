"""
TITLE: Regular expressions for system files
GROUP: Basics
USES: import re
SUMMARY: re.search finds a pattern in text; groups (parentheses) pull the
  pieces out. Use it when a line's shape varies; use str methods when it is
  fixed.
NOTES:
  - Write patterns as raw strings (a leading r) so that backslashes reach re
    unchanged: r"digits" style patterns are in the code below.
  - re.search(pattern, text) scans the whole text; re.match only the start.
    Both return None when nothing matches: check before using .group().
  - re.findall returns all matches; re.sub replaces them.
  - re.compile once if the pattern runs in a loop.
  - Colour escape codes can be removed with re.sub; the pattern is in the code
    below and is reused on the width page.
SEE: pydoc re, https://docs.python.org/3/howto/regex.html
"""

import re

cpuinfo = "processor\t: 0\nmodel name\t: AMD Ryzen 5 7600X 6-Core Processor\ncpu MHz\t\t: 4700.123\n"

match = re.search(r"^model name\s*:\s*(.+)$", cpuinfo, re.MULTILINE)
print("model:", match.group(1) if match else "(not found)")
print("numbers:", re.findall(r"\d+\.\d+|\d+", cpuinfo))
print("no match:", re.search(r"GPU", cpuinfo))
print("stripped:", re.sub(r"\x1b\[[0-9;]*m", "", "\x1b[1;34mOS\x1b[0m"))
