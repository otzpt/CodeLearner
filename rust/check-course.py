#!/usr/bin/env python3
"""Walk every module of the Rust course and check it runs to its SUMMARY.

Each module is started on its own and fed blank answers, which the course
accepts (a wrong answer just shows the right one). Fails on any panic, a
module that never reaches SUMMARY, or a menu that does not quit cleanly.

    make && python3 check-course.py [module numbers...]
"""

import os
import re
import subprocess
import sys

BINARY = os.path.join(os.path.dirname(os.path.abspath(__file__)), "rust-course")


def run(lines):
    done = subprocess.run([BINARY], input="\n".join(lines) + "\n",
                          capture_output=True, text=True, timeout=120)
    return done.stdout + done.stderr, done.returncode


def main():
    if not os.path.exists(BINARY):
        sys.exit("build first: make")

    listing, _ = run(["0"])
    count = len(re.findall(r"^\s+\[\s*\d+\]", listing, re.M)) - 1  # minus Quit
    wanted = [int(n) for n in sys.argv[1:]] or list(range(1, count + 1))

    failed = 0
    for number in wanted:
        text, code = run([str(number)] + [""] * 60 + ["0"])
        ok = ("SUMMARY" in text and code == 0 and "See you next time." in text
              and "panicked" not in text)
        print("module {:2d}: {}".format(number, "ok" if ok else "FAILED"))
        if not ok:
            failed += 1
            print("  exit code", code, "| tail:", repr(text[-400:]))
    sys.exit(1 if failed else 0)


if __name__ == "__main__":
    main()
