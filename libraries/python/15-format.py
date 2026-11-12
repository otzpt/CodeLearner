"""
TITLE: format: f-strings and the format mini-language
GROUP: Built-ins
LIBRARY: Python built-ins (no import needed)
SUMMARY: A format spec after a colon inside {} controls how a value is
  shown: [[fill]align][sign][0][width][,][.precision][type]. The
  same mini-language is used by str.format() and format().
ENTRY: f"{value}"
   An f-string. The expression in braces is evaluated, then converted
   with str().
ENTRY: {value:>10}   {value:<10}   {value:^10}
   Align in a field: > right, < left, ^ centre. Numbers default to
   right, text to left.
ENTRY: {value:*^10}
   A fill character comes before the alignment: centred in 10
   columns, padded with *.
ENTRY: {value:.2f}
   Fixed-point with 2 digits after the point. Rounds the stored
   binary value.
ENTRY: {value:10.3f}   {value:010.3f}
   Width 10, 3 decimals. A leading 0 pads with zeros instead of
   spaces.
ENTRY: {value:,}   {value:_}
   A thousands separator: 1234567 shows as 1,234,567 (or 1_234_567).
ENTRY: {value:+d}   {value: d}
   Always show a sign, or a space for positive numbers.
ENTRY: {value:x}  {value:X}  {value:o}  {value:b}  {value:#x}
   Hexadecimal (lower, upper), octal, binary. # adds the 0x / 0o / 0b
   prefix.
ENTRY: {value:.1%}
   Multiply by 100 and add a percent sign: 0.256 shows as 25.6%.
ENTRY: {value:e}   {value:g}
   Scientific notation; general format (switches between fixed and
   scientific).
ENTRY: {text:.5}
   On a string, precision truncates: only the first 5 characters.
ENTRY: {value!r}   {value!s}
   Convert with repr() or str() first. !r shows quotes and escapes.
ENTRY: {value=}
   Show the expression and its value: f'{x=}' gives x=5. Handy for
   debugging.
ENTRY: {width}
   The spec itself can contain fields:
   f'{value:{width}.{precision}f}'.
ENTRY: {{ and }}
   Literal braces.
NOTES:
  - f-strings need Python 3.6 or later; the {value=} form needs
    3.8.
  - Width counts characters, not columns. East Asian wide
    characters take two columns on screen: see the unicodedata
    page.
  - A coloured string includes invisible escape codes that width
    counts too: pad the plain text, then colour it.
  - Signatures and descriptions are Python's own, read from the
    module's docstrings for the version that runs this page.
    Version notes (added, changed, deprecated) are in the online
    documentation.
SEE: https://docs.python.org/3/library/string.html#format-specification-mini-language
"""

value, text, width = 1234567.891, "formatting", 12
print(f"[{value}]")
print(f"[{value:.2f}] [{value:,.2f}] [{value:15,.1f}] [{value:015.1f}]")
print(f"[{'left':<10}] [{'right':>10}] [{'mid':^10}] [{'fill':*^10}]")
print(f"[{42:+d}] [{42:x}] [{42:#x}] [{42:#b}] [{42:o}] [{255:08b}]")
print(f"[{0.256:.1%}] [{1e-5:e}] [{value:g}]")
print(f"[{text:.5}] [{text!r}] [{text:>{width}}]")
print(f"{width=} {width * 2=}")
print(f"literal braces: {{width}} = {width}")
print("old style:", "%5.1f|%-6s|%03d" % (3.14159, "ab", 7), "  str.format:", "{:>6}|{:06.2f}".format("ab", 3.14159))
