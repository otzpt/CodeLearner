#!/usr/bin/env python3
"""Walk every module of the JavaScript course and check it runs to its SUMMARY.

Each module is started on its own and fed blank answers, which the course
accepts (a blank prediction is skipped, a wrong answer shows the right one).
Every TYPE THIS and PREDICT snippet in a module runs as its own node process
on the way, so a snippet that fails to run fails its module here.

    python3 check-course.py [module numbers...]
"""

import os
import re
import subprocess
import sys

MAIN = os.path.join(os.path.dirname(os.path.abspath(__file__)), "src", "main.js")


def run(lines):
    done = subprocess.run(["node", MAIN], input="\n".join(lines) + "\n",
                          capture_output=True, text=True, timeout=120)
    return done.stdout + done.stderr, done.returncode


def main():
    listing, _ = run(["0"])
    count = len(re.findall(r"^\s+\[\s*\d+\]", listing, re.M)) - 1  # minus Quit
    if count < 1:
        sys.exit("no modules found in the menu:\n" + listing)
    wanted = [int(n) for n in sys.argv[1:]] or list(range(1, count + 1))

    failed = 0
    for number in wanted:
        text, code = run([str(number)] + [""] * 100 + ["0"])
        ok = ("SUMMARY" in text and code == 0 and "See you next time." in text
              and "Something went wrong" not in text)
        print("module {:2d}: {}".format(number, "ok" if ok else "FAILED"))
        if not ok:
            failed += 1
            print("  exit code", code, "| tail:", repr(text[-400:]))
    sys.exit(1 if failed else 0)


if __name__ == "__main__":
    main()
