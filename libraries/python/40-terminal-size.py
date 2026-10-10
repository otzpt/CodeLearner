"""
TITLE: Terminal width and height
GROUP: Terminal
USES: import shutil, os, fcntl, termios, struct
SUMMARY: shutil.get_terminal_size() answers in one call. Underneath it is the
  TIOCGWINSZ ioctl, which asks the terminal how many columns and rows it has.
  sys.stdout.isatty() says whether output is a terminal at all.
NOTES:
  - shutil.get_terminal_size() looks at $COLUMNS and $LINES first, then asks
    the terminal behind stdout, then falls back to (80, 24).
  - When output is piped or redirected (prog | less, prog > file), stdout is
    not a terminal and the ask fails. Open /dev/tty, which is your terminal
    even when everything is redirected, as the code below does.
  - os.get_terminal_size(fd) asks a specific file descriptor and raises OSError
    if it is not a terminal.
  - The size changes when the window is resized: catch SIGWINCH
    (signal.signal) and ask again.
  - Do not print more than the width, or lines wrap and a logo falls apart.
SEE: pydoc shutil.get_terminal_size, man 4 tty_ioctl
"""

import fcntl
import os
import shutil
import struct
import sys
import termios


def size_from_dev_tty():
    """(columns, rows) from the controlling terminal, or None."""
    try:
        descriptor = os.open("/dev/tty", os.O_RDONLY)
    except OSError:
        return None
    try:
        rows, columns, _x, _y = struct.unpack("HHHH", fcntl.ioctl(descriptor, termios.TIOCGWINSZ, b"\0" * 8))
        return (columns, rows) if columns else None
    except OSError:
        return None
    finally:
        os.close(descriptor)


print("stdout is a terminal:", "yes" if sys.stdout.isatty() else "no")
print("shutil.get_terminal_size():", tuple(shutil.get_terminal_size()))
print("from /dev/tty:", size_from_dev_tty() or "(no controlling terminal)")
