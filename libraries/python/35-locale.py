"""
TITLE: locale: internationalisation services
GROUP: Text and data
LIBRARY: Python standard library (locale)
USES: import locale
SUMMARY: The user's locale: language, number formatting and the character
  encoding the system prefers. setlocale(LC_ALL, '') adopts the
  user's environment settings.
ENTRY: locale.setlocale(category, locale=None)
   Set the locale for the given category. The locale can be a string,
   an iterable of two strings (language code and encoding), or None.
ENTRY: locale.getlocale(category=0)
   Returns the current setting for the given locale category as tuple
   (language code, encoding).
ENTRY: locale.getpreferredencoding(do_setlocale=True)
   Return the charset that the user is likely using, according to the
   system configuration.
ENTRY: locale.LC_ALL = 6
   A locale category for setlocale(): all categories at once.
ENTRY: locale.format_string(f, val, grouping=False, monetary=False)
   Formats a string in the same way that the % formatting would use,
   but takes the current locale into account.
ENTRY: locale.currency(val, symbol=True, grouping=False, international=False)
   Formats val according to the currency settings in the current
   locale.
ENTRY: locale.strcoll(os1, os2, /)
   Compares two strings according to the locale.
NOTES:
  - Python 3.7+ coerces the C locale to UTF-8 for text I/O; the
    shell's LANG still matters.
  - Call setlocale once at start-up: it changes process-wide state
    and is not thread-safe.
  - For text files always give an explicit encoding rather than
    relying on the locale.
  - Signatures and descriptions are Python's own, read from the
    module's docstrings for the version that runs this page.
    Version notes (added, changed, deprecated) are in the online
    documentation.
SEE: https://docs.python.org/3/library/locale.html, pydoc locale
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
print("grouped number:", locale.format_string("%d", 1234567, grouping=True))
