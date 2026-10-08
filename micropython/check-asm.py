#!/usr/bin/env python3
"""Assemble every inline-assembly listing the course shows, for the Pico's CPU.

A PC cannot run @micropython.asm_thumb code, so module 10 cannot show it
running. What can be checked is that each listing is valid: mpy-cross, the
MicroPython compiler, assembles it for ARMv6-M (the Cortex-M0+ in the RP2040)
and reports a syntax error for any instruction it does not know. This fails
if a listing in the course stops assembling.

    pip install mpy-cross          # once; a dev-time tool, not a course dependency
    python3 check-asm.py

Assembled is not executed: this proves the syntax, not the behaviour.
"""

import ast
import glob
import os
import shutil
import subprocess
import sys
import tempfile

HERE = os.path.dirname(os.path.abspath(__file__))
SRC = os.path.join(HERE, "src")


def listings():
    """Every list of source lines in the lessons that holds an asm_thumb function."""
    for path in sorted(glob.glob(os.path.join(SRC, "lessons_*.py"))):
        tree = ast.parse(open(path, encoding="utf-8").read())
        for node in ast.walk(tree):
            if isinstance(node, (ast.Assign, ast.Call)):
                for part in ([node.value] if isinstance(node, ast.Assign) else node.args):
                    if isinstance(part, ast.List) and all(
                            isinstance(e, ast.Constant) and isinstance(e.value, str)
                            for e in part.elts):
                        lines = [e.value for e in part.elts]
                        if any(text.startswith("@micropython.asm_thumb") for text in lines):
                            yield os.path.basename(path), node.lineno, lines


def main():
    compiler = os.environ.get("MPY_CROSS") or shutil.which("mpy-cross")
    if compiler is None:
        sys.exit("mpy-cross not found. Install it with: pip install mpy-cross")

    failed = 0
    count = 0
    for source, line, lines in listings():
        count += 1
        text = "import micropython\n" + "\n".join(lines) + "\n"
        with tempfile.TemporaryDirectory() as scratch:
            path = os.path.join(scratch, "listing.py")
            open(path, "w").write(text)
            done = subprocess.run(
                [compiler, "-march=armv6m", path, "-o", os.path.join(scratch, "out.mpy")],
                capture_output=True, text=True)
        ok = done.returncode == 0
        print("{}  {}:{}".format("ok  " if ok else "FAIL", source, line))
        if not ok:
            failed += 1
            print(done.stderr.strip())

    if count == 0:
        sys.exit("no asm_thumb listings found: the checker is looking in the wrong place")
    print("{} listing(s) assembled, {} failed".format(count, failed))
    sys.exit(1 if failed else 0)


if __name__ == "__main__":
    main()
