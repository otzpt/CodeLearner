"""
TITLE: str: text strings (methods)
GROUP: Built-ins
LIBRARY: Python built-ins (no import needed)
SUMMARY: A str is an immutable sequence of Unicode characters. Its methods
  return new strings; none of them changes the original.
ENTRY: str.strip(self, chars=None, /)
   Return a copy of the string with leading and trailing whitespace
   removed.
ENTRY: str.lstrip(self, chars=None, /)
   Return a copy of the string with leading whitespace removed.
ENTRY: str.rstrip(self, chars=None, /)
   Return a copy of the string with trailing whitespace removed.
ENTRY: str.split(self, /, sep=None, maxsplit=-1)
   Return a list of the substrings in the string, using sep as the
   separator string.
ENTRY: str.rsplit(self, /, sep=None, maxsplit=-1)
   Return a list of the substrings in the string, using sep as the
   separator string.
ENTRY: str.splitlines(self, /, keepends=False)
   Return a list of the lines in the string, breaking at line
   boundaries.
ENTRY: str.join(self, iterable, /)
   Concatenate any number of strings.
ENTRY: str.replace(self, old, new, /, count=-1)
   Return a copy with all occurrences of substring old replaced by
   new.
ENTRY: str.startswith
   Return True if the string starts with the specified prefix, False
   otherwise.
ENTRY: str.endswith
   Return True if the string ends with the specified suffix, False
   otherwise.
ENTRY: str.find
   Return the lowest index in S where substring sub is found, such
   that sub is contained within S[start:end].
ENTRY: str.index
   Return the lowest index in S where substring sub is found, such
   that sub is contained within S[start:end].
ENTRY: str.count
   Return the number of non-overlapping occurrences of substring sub
   in string S[start:end].
ENTRY: str.lower(self, /)
   Return a copy of the string converted to lowercase.
ENTRY: str.upper(self, /)
   Return a copy of the string converted to uppercase.
ENTRY: str.title(self, /)
   Return a version of the string where each word is titlecased.
ENTRY: str.capitalize(self, /)
   Return a capitalized version of the string.
ENTRY: str.partition(self, sep, /)
   Partition the string into three parts using the given separator.
ENTRY: str.zfill(self, width, /)
   Pad a numeric string with zeros on the left, to fill a field of
   the given width.
ENTRY: str.center(self, width, fillchar=' ', /)
   Return a centered string of length width.
ENTRY: str.ljust(self, width, fillchar=' ', /)
   Return a left-justified string of length width.
ENTRY: str.rjust(self, width, fillchar=' ', /)
   Return a right-justified string of length width.
ENTRY: str.isdigit(self, /)
   Return True if the string is a digit string, False otherwise.
ENTRY: str.isalpha(self, /)
   Return True if the string is an alphabetic string, False
   otherwise.
ENTRY: str.encode(self, /, encoding='utf-8', errors='strict')
   Encode the string using the codec registered for encoding.
NOTES:
  - strip() with no argument removes whitespace; with an argument
    it removes any of those characters from the ends, not a
    prefix.
  - split() with no argument splits on any run of whitespace and
    drops empty strings; split(',') keeps them.
  - find() returns -1 when absent; index() raises ValueError. Use
    `in` to only test presence.
  - isdigit() is true for digits of other scripts too; int()
    accepts them as well.
  - Signatures and descriptions are Python's own, read from the
    module's docstrings for the version that runs this page.
    Version notes (added, changed, deprecated) are in the online
    documentation.
SEE: https://docs.python.org/3/library/stdtypes.html#string-methods, help(str)
"""

line = "  model name\t: AMD Ryzen 5 7600X  \n"
print(repr(line.strip()))
key, _, value = line.partition(":")
print("partition:", repr(key.strip()), repr(value.strip()))
print("split:", "a,b,,c".split(","), " ".join(["x", "y"]), "a b  c".split())
print("find / index:", "mississippi".find("ss"), "mississippi".find("zz"))
try:
    "mississippi".index("zz")
except ValueError as error:
    print("index raises ValueError:", error)
print("startswith a tuple:", "main.py".endswith((".py", ".pyw")))
print("replace:", "a-b-c".replace("-", "+", 1), " count:", "banana".count("an"))
print("'7'.zfill(3) =", "7".zfill(3), " 'abc'.center(9, '*') =", "abc".center(9, "*"))
print("'١٢٣'.isdigit() =", "١٢٣".isdigit(), " '12'.isdigit() =", "12".isdigit())
print("encode:", "é".encode("utf-8"))
