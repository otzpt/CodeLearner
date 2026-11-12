"""
TITLE: re: regular expressions
GROUP: Text and data
LIBRARY: Python standard library (re)
USES: import re
SUMMARY: Pattern matching on strings with regular expressions. search()
  finds a match anywhere; match() only at the start; fullmatch()
  must cover the whole string; findall(), sub() and split() work
  over all matches.
ENTRY: re.compile(pattern, flags=0)
   Compile a regular expression pattern, returning a Pattern object.
ENTRY: re.search(pattern, string, flags=0)
   Scan through string looking for a match to the pattern, returning
   a Match object, or None if no match was found.
ENTRY: re.match(pattern, string, flags=0)
   Try to apply the pattern at the start of the string, returning a
   Match object, or None if no match was found.
ENTRY: re.fullmatch(pattern, string, flags=0)
   Try to apply the pattern to all of the string, returning a Match
   object, or None if no match was found.
ENTRY: re.findall(pattern, string, flags=0)
   Return a list of all non-overlapping matches in the string.
ENTRY: re.finditer(pattern, string, flags=0)
   Return an iterator over all non-overlapping matches in the string.
   For each match, the iterator returns a Match object.
ENTRY: re.sub(pattern, repl, string, count=0, flags=0)
   Return the string obtained by replacing the leftmost non-
   overlapping occurrences of the pattern in string by the
   replacement repl. repl can be either a string or a callable; if a
   string, backslash escapes in it are processed. If it is a
   callable, it's passed the Match object and must return a
   replacement string to be used.
ENTRY: re.subn(pattern, repl, string, count=0, flags=0)
   Return a 2-tuple containing (new_string, number). new_string is
   the string obtained by replacing the leftmost non-overlapping
   occurrences of the pattern in the source string by the replacement
   repl. number is the number of substitutions that were made. repl
   can be either a string or a callable; if a string, backslash
   escapes in it are processed. If it is a callable, it's passed the
   Match object and must return a replacement string to be used.
ENTRY: re.split(pattern, string, maxsplit=0, flags=0)
   Split the source string by the occurrences of the pattern,
   returning a list containing the resulting substrings. If capturing
   parentheses are used in pattern, then the text of all groups in
   the pattern are also returned as part of the resulting list. If
   maxsplit is nonzero, at most maxsplit splits occur, and the
   remainder of the string is returned as the final element of the
   list.
ENTRY: re.escape(pattern)
   Escape special characters in a string.
ENTRY: re.IGNORECASE = re.IGNORECASE
   A flag: match letters regardless of case. Short form re.I.
ENTRY: re.MULTILINE = re.MULTILINE
   A flag: ^ and $ match at the start and end of every line, not only
   of the whole string. Short form re.M.
ENTRY: re.DOTALL = re.DOTALL
   A flag: . also matches a newline. Short form re.S.
ENTRY: re.error(msg, pattern=None, pos=None)
   Exception raised for invalid regular expressions.
NOTES:
  - Write patterns as raw strings, r'...': otherwise \\d needs a
    doubled backslash.
  - search() and match() return None on failure: test the result
    before calling .group().
  - Greedy .* matches as much as possible; use .*? for the
    shortest match.
  - Do not parse nested structures such as HTML or JSON with a
    regular expression.
  - Signatures and descriptions are Python's own, read from the
    module's docstrings for the version that runs this page.
    Version notes (added, changed, deprecated) are in the online
    documentation.
SEE: https://docs.python.org/3/library/re.html, pydoc re
"""

import re

cpuinfo = "processor\t: 0\nmodel name\t: AMD Ryzen 5 7600X 6-Core Processor\ncpu MHz\t\t: 4700.123\n"

found = re.search(r"^model name\s*:\s*(.+)$", cpuinfo, re.MULTILINE)
print("model:", found.group(1) if found else "(not found)", " span:", found.span() if found else None)
print("findall numbers:", re.findall(r"\d+\.\d+|\d+", cpuinfo))
print("no match returns:", re.search(r"GPU", cpuinfo))
print("match only at the start:", re.match(r"model", cpuinfo), "| fullmatch:", bool(re.fullmatch(r"\d+", "123")))
print("sub:", re.sub(r"\x1b\[[0-9;]*m", "", "\x1b[1;34mOS\x1b[0m"))
print("named group:", re.search(r"(?P<key>\w+)=(?P<value>\d+)", "x=5").groupdict())
print("split:", re.split(r"[,;]\s*", "a, b;c"))
print("escape:", re.escape("1+1=2?"))
pattern = re.compile(r"\d+")
print("compiled pattern reuse:", pattern.findall("a1b22c333"))
try:
    re.compile("(unclosed")
except re.error as error:
    print("re.error:", error)
