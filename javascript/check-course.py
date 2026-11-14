#!/usr/bin/env python3
"""Play every lesson of the JavaScript course from start to finish.

Each lesson is played in a fresh practice folder, answering "k" to every
prompt (next screen, a wrong quiz answer, skip the exercise). It must reach
WHAT YOU LEARNED, save the lesson as done, and quit cleanly. Then lesson 1 is
played with a correct practice file, which the checker must accept.

    python3 check-course.py
"""

import json
import os
import re
import subprocess
import sys
import tempfile

HERE = os.path.dirname(os.path.abspath(__file__))
MAIN = os.path.join(HERE, "src", "main.js")


def run(lines, practice):
    env = dict(os.environ, CODELEARNER_JS_DIR=practice, NO_COLOR="1")
    done = subprocess.run(["node", MAIN], input="\n".join(lines) + "\n", env=env,
                          capture_output=True, text=True, timeout=120)
    return done.stdout + done.stderr, done.returncode


def main():
    failed = 0
    with tempfile.TemporaryDirectory() as practice:
        menu, _ = run(["q"], practice)
    count = len(re.findall(r"^\s+\[[ x]\]\s+\d+  ", menu, re.M))
    if count == 0:
        sys.exit("no lessons in the menu:\n" + menu)

    for number in range(1, count + 1):
        with tempfile.TemporaryDirectory() as practice:
            text, code = run([str(number)] + ["k"] * 100 + ["q"], practice)
            try:
                with open(os.path.join(practice, ".progress.json")) as f:
                    saved = len(json.load(f)["done"]) == 1
            except OSError:
                saved = False
        ok = (code == 0 and "WHAT YOU LEARNED" in text and "See you next time." in text
              and "Something went wrong" not in text and saved)
        print("lesson {:2d}: {}".format(number, "ok" if ok else "FAILED"))
        if not ok:
            failed += 1
            print("  exit", code, "saved", saved, "| tail:", repr(text[-400:]))

    with tempfile.TemporaryDirectory() as practice:
        with open(os.path.join(practice, "01-hello.js"), "w") as f:
            f.write('console.log("Ana");\nconsole.log(2026);\n')
        text, code = run(["1"] + [""] * 4 + ["2"] + [""] * 2 + ["", "m", "q"], practice)
    ok = "Correct!" in text
    print("correct answer accepted:", "ok" if ok else "FAILED")
    if not ok:
        failed += 1
        print("  tail:", repr(text[-600:]))

    sys.exit(1 if failed else 0)


if __name__ == "__main__":
    main()
