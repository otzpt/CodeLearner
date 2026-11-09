"""
TITLE: str methods, int(), float(): parsing text
GROUP: Common tasks
USES: built-in str methods, int(), float()
SUMMARY: Text from files and other programs usually comes as "Key: value"
  lines, "KEY=value" lines and numbers followed by units. These few methods
  parse all three.
SIGNATURE:
  str.partition(sep)
  str.split(sep=None, maxsplit=-1)
  str.strip(chars=None)
  int(text, base=10), float(text)
PARAMETERS:
  - sep: where to cut. None means any run of whitespace.
  - maxsplit: the most cuts to make.
  - chars: the characters to remove from both ends.
  - base: 10 unless the text is in another base.
RETURNS: partition: a tuple of three strings. split: a list. strip: a new
  str. int and float: the number.
ERRORS:
  - ValueError from int and float when the text is not a number.
  - partition never raises when sep is missing, and split raises ValueError
    only for an empty sep.
NOTES:
  - line.partition(":") splits at the FIRST colon into (key, colon, value). It
    never raises, even if the colon is missing; split(":") can give surprises
    when the value itself contains a colon.
  - int("42 apples") raises ValueError; int("42") and int(" 42 ") work. Wrap
    conversions of outside data in try/except ValueError.
  - line.startswith("MemTotal:") tests a key; text.removeprefix() and
    removesuffix() (3.9+) strip a known part.
  - Strip quotes from a value with value.strip('"').
  - re (regular expressions) is for patterns; for fixed formats the methods
    above are simpler and faster.
SEE: pydoc str, pydoc int
"""

meminfo_line = "MemTotal:       16384000 kB"
key, _, rest = meminfo_line.partition(":")
kib = int(rest.split()[0])
print(f"{key} = {kib} kB = {kib // 1024} MiB")

entry = 'PRETTY_NAME="Ubuntu 26.04 LTS"'
name, _, value = entry.partition("=")
quote = '"'
print(f"key {name!r}, value {value.strip(quote)!r}")
print("starts with PRETTY_NAME=:", entry.startswith("PRETTY_NAME="))

for text in ("42", " 42 ", "42 apples", ""):
    try:
        print(f"int({text!r}) = {int(text)}")
    except ValueError as error:
        print(f"int({text!r}) -> ValueError: {error}")
