"""
TITLE: termios: POSIX style tty control
GROUP: System information
LIBRARY: Python standard library (termios)
USES: import termios
SUMMARY: Terminal modes: echo, line editing and raw input, set with
  tcgetattr and tcsetattr. Always put the terminal back the way you
  found it.
ENTRY: termios.tcgetattr(fd, /)
   Get the tty attributes for file descriptor fd.
ENTRY: termios.tcsetattr(fd, when, attributes, /)
   Set the tty attributes for file descriptor fd.
ENTRY: termios.tcflush(fd, queue, /)
   Discard queued data on file descriptor fd.
ENTRY: termios.TIOCGWINSZ = 0x5413
   The ioctl request that asks a terminal for its size, for
   fcntl.ioctl(). The value is this system's; read it from this
   module, do not type the number.
ENTRY: termios.ECHO = 0x8
   A local-mode flag: characters typed are echoed back to the screen.
   Clear it (for passwords) in the lflag word of tcgetattr().
ENTRY: termios.ICANON = 0x2
   A local-mode flag: canonical (line-at-a-time) input with line
   editing. Clear it to read single keys.
ENTRY: termios.TCSANOW = 0
   A 'when' argument for tcsetattr(): apply the change immediately.
   TCSADRAIN waits for output to drain, TCSAFLUSH also discards
   unread input.
NOTES:
  - Restore the saved attributes in a finally block, or a crash
    leaves the user's terminal without echo.
  - tcgetattr returns a list: [iflag, oflag, cflag, lflag, ispeed,
    ospeed, cc]. Copy it before modifying.
  - tty.setcbreak() and tty.setraw() wrap the common cases.
  - Signatures and descriptions are Python's own, read from the
    module's docstrings for the version that runs this page.
    Version notes (added, changed, deprecated) are in the online
    documentation.
SEE: https://docs.python.org/3/library/termios.html, pydoc termios
"""

import os
import sys
import termios

if sys.stdin.isatty():
    saved = termios.tcgetattr(sys.stdin)
    lflag = saved[3]
    print("echo is on:", bool(lflag & termios.ECHO), " line editing (ICANON) is on:", bool(lflag & termios.ICANON))
    # To read single keys: clear ICANON and ECHO, call tcsetattr, and ALWAYS
    # restore `saved` in a finally block.
else:
    print("stdin is not a terminal, so there are no modes to read (this page runs without one)")
print("TIOCGWINSZ is a constant of this module:", hasattr(termios, "TIOCGWINSZ"))
