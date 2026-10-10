#!/usr/bin/env python3
"""Open every page of the Libraries reference through the real browser.

check-pages.py runs the pages directly. This drives ./libraries the way a
person does: pick a language, open each page in turn, go back, quit. It fails
if a page cannot be opened, did not compile, ran without output, or if the
browser does not exit cleanly. It also tries the search.

    make && python3 check-browser.py
"""

import os
import re
import subprocess
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from header_list import C_HEADERS  # noqa: E402

HERE = os.path.dirname(os.path.abspath(__file__))
BROWSER = os.path.join(HERE, "libraries")
ANSI = re.compile(r"\x1b\[[0-9;]*[A-Za-z]")


def run(lines, timeout=300):
    done = subprocess.run([BROWSER], input="\n".join(lines) + "\n", capture_output=True, text=True, timeout=timeout)
    return ANSI.sub("", done.stdout), done.returncode


def main():
    if not os.path.exists(BROWSER):
        sys.exit("build first: make")

    failed = 0
    for number, track in ((1, "C"), (2, "Python")):
        menu, _ = run([str(number), "", "0"])
        listing = menu.split("LIBRARIES - " + track)[1].split("Type a number to open a page")[0]
        count = len(re.findall(r"^\s+\[\s*\d+\]  ", listing, re.M))

        # Open every page: its number, then ENTER at "Press ENTER to continue".
        script = [str(number)]
        for page in range(1, count + 1):
            script += [str(page), ""]
        script += ["", "0"]
        text, code = run(script)

        runnable = len(re.findall(r"RUN, on this machine", text))
        problems = []
        if code != 0:
            problems.append(f"exit status {code}")
        if "did not compile" in text:
            problems.append("a page did not compile")
        if "No " + track + " pages found" in text:
            problems.append("no pages found")
        if "See you next time." not in text:
            problems.append("did not exit cleanly")
        # Pages ending in .txt are documentation only; every other page runs.
        documentation = len([n for n in os.listdir(os.path.join(HERE, track.lower())) if n.endswith(".txt")])
        if runnable != count - documentation:
            problems.append(f"{runnable} pages ran but {count - documentation} are runnable")
        # Each RUN section must have at least one indented line of output.
        for section in text.split("RUN, on this machine")[1:]:
            body = section.split("Press ENTER")[0]
            if len([line for line in body.splitlines() if line.startswith("      ")]) == 0:
                problems.append("a page ran and printed nothing")
                break

        search, _ = run([str(number), "memory", "", "0"])
        if "Pages mentioning" not in search or "Memory and swap" not in search:
            problems.append("search for 'memory' did not find the memory page")

        print(f"{'FAIL' if problems else 'ok  '}  {track}: {count} pages opened, {runnable} ran")
        for problem in problems:
            print("      " + problem)
        failed += 1 if problems else 0

    # Every header must be found by typing its name, with a page titled for it.
    queries = ["1"]
    for header in C_HEADERS:
        queries += [header, ""]
    queries += ["", "0"]
    text, code = run(queries)
    results = text.split('Pages mentioning "')[1:]
    missing = []
    for header, result in zip(C_HEADERS, results):
        listing = result.split("Type a number to open a page")[0]
        if not re.search(r"\]\s+" + re.escape(header) + r"\b", listing):
            missing.append(header)
    problems = []
    if len(results) != len(C_HEADERS):
        problems.append(f"expected {len(C_HEADERS)} search results, got {len(results)}")
    if missing:
        problems.append("search did not list a page titled for: " + ", ".join(missing))
    print(f"{'FAIL' if problems else 'ok  '}  C: all {len(C_HEADERS)} headers found by search")
    for problem in problems:
        print("      " + problem)
    failed += 1 if problems else 0

    sys.exit(1 if failed else 0)


if __name__ == "__main__":
    main()
