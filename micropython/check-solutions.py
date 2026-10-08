#!/usr/bin/env python3
"""Run every example solution the MicroPython course shows and compare what
it prints with the output its challenge promises.

The challenges are read straight out of the lesson sources (the four lists
passed to ui.challenge), so this cannot drift from them. A solution that
needs the Pico's assembler cannot run on a PC; it is assembled instead, by
check-asm.py.

    python3 check-solutions.py
"""

import ast
import glob
import os
import subprocess
import sys
import tempfile

HERE = os.path.dirname(os.path.abspath(__file__))
SRC = os.path.join(HERE, "src")


def challenges():
    for path in sorted(glob.glob(os.path.join(SRC, "lessons_*.py"))):
        tree = ast.parse(open(path, encoding="utf-8").read())
        for node in ast.walk(tree):
            if (isinstance(node, ast.Call) and isinstance(node.func, ast.Attribute)
                    and node.func.attr == "challenge" and len(node.args) == 4):
                task, given, expected, solution = (ast.literal_eval(a) for a in node.args)
                yield os.path.basename(path), node.lineno, given, expected, solution


def main():
    failed = 0
    total = 0
    for source, line, given, expected, solution in sorted(challenges()):
        total += 1
        label = "{}:{}".format(source, line)
        if any("asm_thumb" in text for text in solution):
            print("skip  {}  (assembler: see check-asm.py)".format(label))
            continue
        with tempfile.NamedTemporaryFile("w", suffix=".py", dir=SRC, delete=False) as handle:
            handle.write("\n".join(solution) + "\n")
            path = handle.name
        try:
            run = subprocess.run(
                ["micropython", os.path.basename(path)], cwd=SRC,
                input="\n".join(given) + ("\n" if given else ""),
                capture_output=True, text=True, timeout=30)
        finally:
            os.unlink(path)
        printed = [text.rstrip() for text in run.stdout.splitlines()]
        ok = run.returncode == 0 and printed == expected
        print("{}  {}".format("ok   " if ok else "FAIL ", label))
        if not ok:
            failed += 1
            print("   expected:", expected)
            print("   printed: ", printed, run.stderr.strip())

    print("{} challenge(s) checked, {} failed".format(total, failed))
    sys.exit(1 if failed else 0)


if __name__ == "__main__":
    main()
