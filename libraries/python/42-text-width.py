"""
TITLE: Aligning columns when the text has colours or wide characters
GROUP: Terminal
USES: import re, unicodedata
SUMMARY: str.ljust and f-string padding count CHARACTERS. Colour escape codes
  take characters but no screen space, and East Asian characters or emoji take
  two columns each, so a column of such text misaligns. Pad by VISIBLE width
  instead.
NOTES:
  - Remove escape codes with the regular expression ESC [ digits/semicolons m,
    then measure what is left.
  - unicodedata.east_asian_width(ch) is "W" or "F" for characters that take two
    columns; unicodedata.combining(ch) is non-zero for accents that take none.
  - The third-party wcwidth package does this thoroughly (including emoji
    sequences); the code below handles the common cases with the standard
    library only.
  - Build padding as " " * (width - visible_width(text)); never rely on len()
    for layout.
SEE: pydoc unicodedata
"""

import re
import unicodedata

ANSI = re.compile("\x1b\\[[0-9;]*[A-Za-z]")


def visible_width(text):
    """Columns the text takes on screen."""
    width = 0
    for character in ANSI.sub("", text):
        if unicodedata.combining(character):
            continue
        width += 2 if unicodedata.east_asian_width(character) in ("W", "F") else 1
    return width


def pad(text, width):
    return text + " " * max(0, width - visible_width(text))


labels = ["\x1b[1;34mOS\x1b[0m", "\x1b[1;34mKernel\x1b[0m", "日本語", "Disk"]

print(f"len of a coloured 'OS' = {len(labels[0])} characters, visible_width = {visible_width(labels[0])} columns")
print(f"len of 日本語 = {len(labels[2])} characters, visible_width = {visible_width(labels[2])} columns")
print()
print("f-string padding, counts characters:")
for label in labels:
    print(f"  {label:<8}|")
print("padded by visible width:")
for label in labels:
    print(f"  {pad(label, 8)}|")
