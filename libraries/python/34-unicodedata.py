"""
TITLE: unicodedata: Unicode character database
GROUP: Text and data
LIBRARY: Python standard library (unicodedata)
USES: import unicodedata
SUMMARY: Properties of Unicode characters: name, category, combining class,
  and how many columns they take (east_asian_width). len() counts
  characters, not screen columns.
ENTRY: unicodedata.east_asian_width(chr, /)
   Returns the east asian width assigned to the character chr as
   string.
ENTRY: unicodedata.combining(chr, /)
   Returns the canonical combining class assigned to the character
   chr as integer.
ENTRY: unicodedata.category(chr, /)
   Returns the general category assigned to the character chr as
   string.
ENTRY: unicodedata.name
   Returns the name assigned to the character chr as a string.
ENTRY: unicodedata.lookup(name, /)
   Look up character by name.
ENTRY: unicodedata.normalize(form, unistr, /)
   Return the normal form 'form' for the Unicode string unistr.
ENTRY: unicodedata.numeric
   Converts a Unicode character into its equivalent numeric value.
ENTRY: unicodedata.decimal
   Converts a Unicode character into its equivalent decimal value.
NOTES:
  - east_asian_width 'W' and 'F' take two columns in a terminal;
    combining marks take none.
  - Compare text after normalising (NFC or NFKC): 'é' can be one
    character or two.
  - A terminal's own idea of width can differ for emoji and
    ambiguous-width characters.
  - Signatures and descriptions are Python's own, read from the
    module's docstrings for the version that runs this page.
    Version notes (added, changed, deprecated) are in the online
    documentation.
SEE: https://docs.python.org/3/library/unicodedata.html, pydoc unicodedata
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


labels = ["\x1b[1;34mOS\x1b[0m", "Disk", "日本語", "e\u0301"]
for label in labels:
    print(f"len={len(label):<2} columns={visible_width(label):<2} {label!r}")
print("name:", unicodedata.name("é"), "|", unicodedata.lookup("SNOWMAN"), "| category of '5':", unicodedata.category("5"))
print("NFC makes e + combining acute one character:", len(unicodedata.normalize("NFC", "e\u0301")))
print("numeric('½') =", unicodedata.numeric("½"))
