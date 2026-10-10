"""
TITLE: Colours and styles (ANSI escape codes)
GROUP: Terminal
USES: print escape sequences; no library
SUMMARY: A terminal changes colour when it reads ESC [ code m. ESC is the
  character number 27, written as an escape in a Python string. The reset code
  0 undoes everything.
NOTES:
  - Styles: 1 bold, 2 dim, 3 italic, 4 underline, 7 reverse.
  - Colours: 30-37 text (black red green yellow blue magenta cyan white), 40-47
    background, 90-97 and 100-107 the bright versions.
  - 256 colours: ESC[38;5;Nm for text, ESC[48;5;Nm for background.
  - True colour: ESC[38;2;R;G;Bm. Check $COLORTERM for "truecolor" or "24bit".
  - ALWAYS reset (ESC[0m) before the end of a line, or the colour leaks into the
    shell prompt.
  - Print colours only when stdout is a terminal, and never when $NO_COLOR is
    set (https://no-color.org). A tool whose output is piped to a file should
    contain no escape bytes.
  - The distribution's own colour, for a logo, is ANSI_COLOR in os-release.
  - On Windows the third-party colorama package translates these codes.
SEE: man 4 console_codes
"""

import os
import sys

ESC = "\033["


def use_colour(force=False):
    """Colour only for a person at a terminal who has not opted out."""
    if "NO_COLOR" in os.environ:
        return False
    return force or sys.stdout.isatty()


def paint(text, code):
    return f"{ESC}{code}m{text}{ESC}0m"


if not use_colour(force=True):          # forced on: this page's output is piped
    print("colour is off ($NO_COLOR is set)")
    raise SystemExit

print("text colours:  ", " ".join(paint(code, code) for code in range(30, 38)))
print("bright:        ", " ".join(paint(code, code) for code in range(90, 98)))
print("styles:        ", " ".join(paint(name, code) for code, name in ((1, "bold"), (2, "dim"), (3, "italic"), (4, "underline"), (7, "reverse"))))
print("blocks:        ", "".join(paint("  ", code) for code in range(40, 48)))
print("256 colours:   ", "".join(paint(" ", f"48;5;{n}") for n in range(16, 52)))
print("true colour:   ", "".join(paint(" ", f"48;2;{step * 7};{255 - step * 7};255") for step in range(36)))
