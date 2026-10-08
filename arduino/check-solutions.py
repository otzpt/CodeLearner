#!/usr/bin/env python3
"""Compile and run every example solution the course shows, and compare what
it prints with the output its challenge promises.

A solution shown to a student that does not build, or does not produce the
output the task says it must, is worse than no solution. The challenges are
read straight out of the lesson sources, so this cannot drift from them.

    python3 check-solutions.py

Exits 0 when every solution builds warning-free and prints exactly what its
challenge expects.
"""

import codecs
import glob
import os
import re
import subprocess
import sys
import tempfile

HERE = os.path.dirname(os.path.abspath(__file__))
SHIM = os.path.join(HERE, "shim")

STRING = re.compile(r'"((?:[^"\\]|\\.)*)"')
# A brace-initialised list of string literals. Matching whole literals keeps a
# "};" INSIDE a solution line from ending the list early.
ARRAY = re.compile(r'const char \*(\w+)\[\] = \{((?:\s*"(?:[^"\\]|\\.)*"\s*,?)*)\s*\};')
LOOPS = re.compile(r"LOOPS=(\d+)")


def strings_of(body):
    return [codecs.decode(raw, "unicode_escape") for raw in STRING.findall(body)]


def challenges():
    """Yield (source file, index, task, input, expected, solution) per challenge."""
    for path in sorted(glob.glob(os.path.join(HERE, "course", "lessons_*.ino"))):
        text = open(path, encoding="utf-8").read()
        for block in re.finditer(r"\{\s*\n(\s*const char \*task\[\].*?)challenge\(", text, re.S):
            arrays = {name: strings_of(body) for name, body in ARRAY.findall(block.group(1))}
            yield (
                os.path.basename(path),
                arrays["task"],
                arrays.get("input", []),
                arrays["expected"],
                arrays["solution"],
            )


def main():
    failed = 0
    total = 0
    with tempfile.TemporaryDirectory() as scratch:
        for number, (source, task, given, expected, solution) in enumerate(challenges(), start=1):
            total += 1
            code = os.path.join(scratch, "solution.cpp")
            with open(code, "w", encoding="utf-8") as handle:
                handle.write("#include <Arduino.h>\n" + "\n".join(solution) + "\n")

            binary = os.path.join(scratch, "solution")
            build = subprocess.run(
                ["g++", "-std=gnu++17", "-Wall", "-Wextra", f"-I{SHIM}", "-o", binary,
                 code, os.path.join(SHIM, "main.cpp")],
                capture_output=True, text=True)
            if build.returncode != 0 or build.stderr.strip():
                print(f"challenge {number:2d} ({source}): BUILD PROBLEM\n{build.stderr}")
                failed += 1
                continue

            loops = LOOPS.search(" ".join(task))
            run = subprocess.run(
                [binary], input="\n".join(given) + ("\n" if given else ""),
                capture_output=True, text=True, timeout=20,
                env={**os.environ, "SIM_LOOPS": loops.group(1) if loops else "1"})
            printed = [line.rstrip() for line in run.stdout.splitlines()]
            ok = printed == expected
            print(f"challenge {number:2d} ({source}): {'ok' if ok else 'FAILED'}")
            if not ok:
                failed += 1
                print("   expected:", expected)
                print("   printed: ", printed)

    print(f"{total - failed}/{total} solutions correct")
    sys.exit(1 if failed else 0)


if __name__ == "__main__":
    main()
