#!/usr/bin/env python3
"""Play every lesson of a course from start to finish.

Each lesson is played in a fresh practice folder, answering "k" to every
prompt (next screen, a wrong quiz answer, skip the exercise). It must reach
WHAT YOU LEARNED, save the lesson as done, and quit cleanly. Then the first
lesson with an exercise is played with its solution already written in the
practice file, which the checker must accept.

    python3 learn/check-course.py <course folder> [more course folders...]
"""

import json
import os
import re
import subprocess
import sys
import tempfile


def run(entry, lines, practice):
    env = dict(os.environ, CODELEARNER_PRACTICE_DIR=practice, NO_COLOR="1")
    done = subprocess.run(["node", entry], input="\n".join(lines) + "\n", env=env,
                          capture_output=True, text=True, timeout=600)
    return done.stdout + done.stderr, done.returncode


def first_exercise(folder):
    """(lesson number, exercise file name, solution) of the first exercise."""
    script = """
const {loadCourse} = require(%s);
const c = loadCourse(%s);
for (const [n, l] of c.lessons.entries())
  for (const s of l.screens) for (const b of s)
    if (b && b.exercise) {
      const ext = require("path").extname(c.file);
      console.log(JSON.stringify([n + 1, String(n + 1).padStart(2, "0") + "-" + b.exercise + ext, b.solution]));
      process.exit(0);
    }
""" % (json.dumps(os.path.join(os.path.dirname(os.path.abspath(__file__)), "main.js")), json.dumps(folder))
    out = subprocess.run(["node", "-e", script], capture_output=True, text=True).stdout
    return json.loads(out) if out.strip() else None


def check(folder):
    folder = os.path.abspath(folder)
    entry = os.path.join(folder, "learn.js")
    failed = 0
    with tempfile.TemporaryDirectory() as practice:
        menu, _ = run(entry, ["q"], practice)
    count = len(re.findall(r"^\s+\[[ x]\]\s+\d+  ", menu, re.M))
    if count == 0:
        print("no lessons in the menu:\n" + menu)
        return 1

    for number in range(1, count + 1):
        with tempfile.TemporaryDirectory() as practice:
            text, code = run(entry, [str(number)] + ["k"] * 150 + ["q"], practice)
            try:
                with open(os.path.join(practice, ".progress.json")) as f:
                    saved = len(json.load(f)["done"]) == 1
            except OSError:
                saved = False
        ok = (code == 0 and "WHAT YOU LEARNED" in text and "See you next time." in text
              and "Something went wrong" not in text and saved)
        print("{} lesson {:2d}: {}".format(os.path.basename(folder), number, "ok" if ok else "FAILED"))
        if not ok:
            failed += 1
            print("  exit", code, "saved", saved, "| tail:", repr(text[-500:]))

    found = first_exercise(folder)
    if found:
        number, name, solution = found
        with tempfile.TemporaryDirectory() as practice:
            with open(os.path.join(practice, name), "w") as f:
                f.write(solution + "\n")
            # ENTER through screens and quizzes until the exercise checks the file.
            text, code = run(entry, [str(number)] + [""] * 60 + ["m", "q"], practice)
        ok = "Correct!" in text
        print("{} correct answer accepted: {}".format(os.path.basename(folder), "ok" if ok else "FAILED"))
        if not ok:
            failed += 1
            print("  tail:", repr(text[-600:]))
    return failed


def main():
    if len(sys.argv) < 2:
        sys.exit("usage: python3 learn/check-course.py <course folder>...")
    failed = sum(check(folder) for folder in sys.argv[1:])
    sys.exit(1 if failed else 0)


if __name__ == "__main__":
    main()
