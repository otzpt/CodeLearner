"""
TITLE: Printing and formatting text
GROUP: Basics
USES: built-in print(), f-strings, str.format
SUMMARY: print() writes to the terminal. f-strings put values inside text and
  the format spec after the colon controls width, alignment and decimals.
NOTES:
  - f"{value:>8}" right-aligns in 8 columns, "<8" left, "^8" centres.
  - f"{x:.2f}" two decimals, f"{n:,}" thousands separators, f"{n:08b}" binary.
  - print(..., file=sys.stderr) writes to the error stream, which a pipe keeps
    apart from normal output.
  - print(..., end="") does not add the newline; print(..., sep="") joins
    arguments with nothing.
  - Padding counts characters, not screen columns: coloured or wide text needs
    the width page.
SEE: pydoc print, https://docs.python.org/3/library/string.html#formatspec
"""

import sys

cpu = "Ryzen 5"
cores = 12
used, total = 8.27, 29.73

print(f"{'cpu':<8}|{cores:>5}|{used / total * 100:6.2f}%")
print(f"memory {used:.2f} GiB / {total:.2f} GiB")
print(f"{16384000 * 1024:,} bytes")
print(f"{cores:08b} is {cores} in binary")
print("a", "b", "c", sep="-", end="!\n")
sys.stdout.flush()
print("this goes to stderr", file=sys.stderr)
