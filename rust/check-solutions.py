#!/usr/bin/env python3
"""Compile and run every example solution the Rust course shows, and compare
what it prints with the output its challenge promises.

The challenges are read straight out of the lesson sources, so this cannot
drift from them. A solution that does not build, builds with a warning, or
prints something else fails.

    python3 check-solutions.py
"""

import codecs
import glob
import os
import re
import subprocess
import sys
import tempfile

HERE = os.path.dirname(os.path.abspath(__file__))

STRING = re.compile(r'"((?:[^"\\]|\\.)*)"')
CALL = re.compile(
    r"challenge\(\s*&\[(.*?)\],\s*&\[(.*?)\],\s*&\[(.*?)\],\s*&\[(.*?)\],\s*\);", re.S)


def strings_of(body):
    return [codecs.decode(raw, "unicode_escape") for raw in STRING.findall(body)]


def challenges():
    for path in sorted(glob.glob(os.path.join(HERE, "src", "lessons_*.rs"))):
        text = open(path, encoding="utf-8").read()
        for match in CALL.finditer(text):
            line = text.count("\n", 0, match.start()) + 1
            task, given, expected, solution = (strings_of(group) for group in match.groups())
            yield os.path.basename(path), line, given, expected, solution


def main():
    failed = 0
    total = 0
    with tempfile.TemporaryDirectory() as scratch:
        for source, line, given, expected, solution in challenges():
            total += 1
            label = "{}:{}".format(source, line)
            code = os.path.join(scratch, "solution.rs")
            binary = os.path.join(scratch, "solution")
            with open(code, "w", encoding="utf-8") as handle:
                handle.write("\n".join(solution) + "\n")

            build = subprocess.run(["rustc", "--edition", "2021", code, "-o", binary],
                                   capture_output=True, text=True)
            if build.returncode != 0 or build.stderr.strip():
                print("FAIL   {}  build problem\n{}".format(label, build.stderr))
                failed += 1
                continue

            run = subprocess.run([binary], input="\n".join(given) + ("\n" if given else ""),
                                 capture_output=True, text=True, timeout=20)
            printed = [text.rstrip() for text in run.stdout.splitlines()]
            ok = run.returncode == 0 and printed == expected
            print("{}  {}".format("ok    " if ok else "FAIL  ", label))
            if not ok:
                failed += 1
                print("   expected:", expected)
                print("   printed: ", printed, run.stderr.strip())

    if total == 0:
        sys.exit("no challenges found: the checker is looking in the wrong place")
    print("{} solution(s) checked, {} failed".format(total, failed))
    sys.exit(1 if failed else 0)


if __name__ == "__main__":
    main()
