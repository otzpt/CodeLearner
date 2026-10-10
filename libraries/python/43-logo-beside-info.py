"""
TITLE: A logo on the left, information on the right
GROUP: Terminal
USES: import itertools (zip_longest)
SUMMARY: A fastfetch layout is two lists printed side by side: line N of the
  logo, a gap, then line N of the information. Pad the logo lines to equal
  width so the information column lines up.
NOTES:
  - The logo is a list of strings, one per row. Its width is the widest row,
    measured in VISIBLE columns (see the alignment page when a row has colours).
  - itertools.zip_longest(logo, info, fillvalue="") walks both lists to the end
    of the longer one, so neither runs out early.
  - Colour the logo with one colour for the whole logo (the distribution's
    ANSI_COLOR), reset at the end of EACH row, then print the information.
  - Check the terminal width first and drop the logo on a narrow terminal.
  - Real fastfetch logos are ASCII art with colour placeholders; store each
    as a text file and read it into the list.
SEE: the alignment page, the terminal size page
"""

from itertools import zip_longest

logo = [
    "    .-----.    ",
    "   /  o o  \\   ",
    "  |    >    |  ",
    "  |  \\___/  |  ",
    "   \\       /   ",
    "    '-----'    ",
]
info = [
    "user@host",
    "---------",
    "OS: Example Linux",
    "Kernel: 6.17.0",
    "Uptime: 3 hours, 12 mins",
    "Memory: 4.2 GiB / 15.6 GiB",
    "Shell: bash",
]

logo_width = max(len(row) for row in logo)
for left, right in zip_longest(logo, info, fillvalue=""):
    print(f"\033[1;36m{left:<{logo_width}}\033[0m  {right}")
