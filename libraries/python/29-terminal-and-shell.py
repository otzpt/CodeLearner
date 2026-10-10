"""
TITLE: Shell and terminal emulator (walking up the process tree)
GROUP: System information
USES: import os; read /proc/<pid>/stat and /proc/<pid>/comm
SUMMARY: The shell and the terminal emulator are your program's ancestors.
  Every process records its parent's pid; /proc/<pid>/comm names a process.
  Walk upward from os.getppid() and read the names.
NOTES:
  - $SHELL is your LOGIN shell, set at login. The shell you are using is the
    parent process, which may be different (you typed zsh inside bash).
  - Typical chain: your program -> bash -> gnome-terminal-server (or kitty,
    alacritty, konsole, foot) -> systemd. The first non-shell ancestor is the
    terminal emulator.
  - Field 4 of /proc/<pid>/stat is the parent pid, but field 2 is the command
    name in parentheses and may itself contain spaces and a closing bracket.
    Split from the LAST closing bracket, as below.
  - comm holds at most 15 characters, so long names are cut.
  - $TERM names the terminal TYPE (xterm-256color), not the program.
    $TERM_PROGRAM, when set, names the program.
  - This page is run by the reference browser, so the chain it prints starts
    with the browser's own helpers, not with a bare shell.
SEE: man 5 proc
"""

import os
from pathlib import Path


def parent_of(pid):
    try:
        line = Path(f"/proc/{pid}/stat").read_text()
    except OSError:
        return None
    after_name = line[line.rindex(")") + 2:]          # "S 1234 ..." after the name
    return int(after_name.split()[1])


def name_of(pid):
    try:
        return Path(f"/proc/{pid}/comm").read_text().strip()
    except OSError:
        return None


print("$TERM         ", os.environ.get("TERM", "(not set)"))
print("$TERM_PROGRAM ", os.environ.get("TERM_PROGRAM", "(not set)"))
print("ancestors, nearest first:")

pid = os.getppid()
for _ in range(8):
    name = name_of(pid)
    if pid <= 1 or name is None:
        break
    print(f"  {pid:<7}{name}")
    pid = parent_of(pid)
