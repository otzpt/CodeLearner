"""
TITLE: Language, locale and character encoding
GROUP: System information
USES: import locale, os, sys
SUMMARY: The locale decides the language, date formats and the character
  encoding. It comes from the environment (LC_ALL, then LC_*, then LANG).
  locale.setlocale(locale.LC_ALL, "") adopts it.
NOTES:
  - sys.stdout.encoding is what print() uses to turn text into bytes. If it is
    "utf-8" you can print box-drawing characters and accents. If it is "ascii"
    (LANG=C in old setups) printing them raises UnicodeEncodeError.
  - LANG looks like en_US.UTF-8 or pt_PT.UTF-8: language, country, encoding.
  - locale.getlocale() returns (language, encoding), or (None, None) in the C
    locale.
  - Over ssh or in cron, LANG is often unset and the answer is "C".
  - Python 3.7+ switches to UTF-8 automatically when the locale is "C".
SEE: pydoc locale, man 7 locale
"""

import locale
import os
import sys

print("LANG      ", os.environ.get("LANG", "(not set)"))
print("LC_ALL    ", os.environ.get("LC_ALL", "(not set)"))
print("setlocale ", locale.setlocale(locale.LC_ALL, ""))
print("getlocale ", locale.getlocale())
print("stdout encoding:", sys.stdout.encoding)
print("preferred encoding:", locale.getpreferredencoding(False))
