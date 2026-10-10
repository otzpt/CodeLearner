"""
TITLE: Errors: try/except, OSError, errno
GROUP: Basics
USES: built-in exceptions, import errno
SUMMARY: Reading system files fails in ordinary ways (missing, forbidden,
  not a number). Catch the specific exception, so a real bug is not hidden.
NOTES:
  - FileNotFoundError, PermissionError and IsADirectoryError are all OSError;
    error.errno holds the number and error.strerror the message.
  - `except Exception:` hides typos and real bugs; name what you expect.
  - A /proc or /sys file may exist on one machine and not another (no battery
    on a desktop). Treat FileNotFoundError as "not available here".
  - Use `else:` for code that should run only if nothing was raised, and
    `finally:` for clean-up.
  - raise ... from error keeps the original cause visible.
SEE: https://docs.python.org/3/library/exceptions.html
"""

import errno

try:
    open("/sys/class/power_supply/BAT0/capacity", encoding="utf-8")
    print("this machine has a battery")
except FileNotFoundError as error:
    print(f"FileNotFoundError: errno {error.errno} = {error.strerror}")
    print("ENOENT means 'not available here':", error.errno == errno.ENOENT)

try:
    open("/etc/shadow", encoding="utf-8")
except PermissionError as error:
    print("/etc/shadow:", error.strerror)

try:
    int("not a number")
except ValueError as error:
    print("ValueError:", error)
