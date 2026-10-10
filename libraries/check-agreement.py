#!/usr/bin/env python3
"""Check that the C and the Python mini fastfetch agree.

The two programs read the same machine through different APIs (libc and
/proc on one side, the standard library and /proc on the other). If a field
differs, one of the two pages is wrong. Uptime, memory and disk change from
one run to the next, so they are not compared.

    python3 check-agreement.py
"""

import os
import re
import subprocess
import sys
import tempfile

HERE = os.path.dirname(os.path.abspath(__file__))
ANSI = re.compile(r"\x1b\[[0-9;]*m")
VOLATILE = {"Uptime", "Memory", "Swap", "Disk"}


LABELS = ("OS", "Host", "Kernel", "Uptime", "Packages", "Shell", "Desktop", "Display", "CPU", "GPU",
          "Memory", "Swap", "Disk", "Local IP")
LINE = re.compile(r"(?:^|\s)(" + "|".join(LABELS) + r"): (.+?)\s*$")


def fields(output):
    """Label -> value for every field line, with the logo and the colours ignored."""
    result = {}
    for line in ANSI.sub("", output).splitlines():
        match = LINE.search(line)
        if match:
            result[match.group(1)] = match.group(2)
    return result


def run_c(scratch):
    program = os.path.join(scratch, "minifetch")
    subprocess.run(["cc", "-std=gnu11", "-o", program, os.path.join(HERE, "c", "50-minifetch.c")], check=True)
    return subprocess.run([program], capture_output=True, text=True, check=True).stdout


def run_python():
    return subprocess.run([sys.executable, os.path.join(HERE, "python", "50-minifetch.py")],
                          capture_output=True, text=True, check=True).stdout


def main():
    with tempfile.TemporaryDirectory() as scratch:
        in_c, in_python = fields(run_c(scratch)), fields(run_python())

    failed = 0
    for label in sorted(set(in_c) | set(in_python)):
        if label in VOLATILE:
            continue
        same = in_c.get(label) == in_python.get(label)
        print("{}  {:<9} C: {!r}  Python: {!r}".format("ok  " if same else "FAIL", label, in_c.get(label), in_python.get(label)))
        failed += 0 if same else 1

    for label in sorted(VOLATILE):
        present = (label in in_c) == (label in in_python)
        print("{}  {:<9} present in both or neither".format("ok  " if present else "FAIL", label))
        failed += 0 if present else 1

    if not in_c:
        sys.exit("the C program printed no fields")
    sys.exit(1 if failed else 0)


if __name__ == "__main__":
    main()
