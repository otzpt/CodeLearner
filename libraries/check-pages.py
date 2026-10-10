#!/usr/bin/env python3
"""Compile and run every page of the Libraries reference, and check each one.

A reference that shows code which does not build, or output that is empty or
full of warnings, is worse than no reference. This builds every C page with
-Wall -Wextra (any diagnostic fails), runs every page with Python warnings
turned into errors, and checks the page header the browser reads.

    python3 check-pages.py
"""

import glob
import os
import re
import subprocess
import sys
import tempfile

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

HERE = os.path.dirname(os.path.abspath(__file__))
FIELD = re.compile(r"^([A-Z]+):")

from header_list import C_HEADERS  # noqa: E402


def header(path):
    """The KEY: value fields of a page's header comment, as a dict."""
    text = open(path, encoding="utf-8").read()
    if path.endswith(".txt"):
        block = text.split("\n---\n", 1)[0]
    elif text.lstrip().startswith("/*"):
        block = text.split("*/", 1)[0].split("/*", 1)[1]
    else:
        block = text.split('"""', 2)[1]
    fields, current = {}, None
    for raw in block.splitlines():
        line = re.sub(r"^\s*\*? ?", "", raw) if text.lstrip().startswith("/*") else raw
        match = FIELD.match(line)
        if match:
            current = match.group(1)
            fields[current] = line[len(current) + 1:].strip()
        elif current and line.strip():
            fields[current] += "\n" + line.strip()
    return fields


def main():
    failures = []
    titles = {}
    count = 0

    with tempfile.TemporaryDirectory() as scratch:
        for track, pattern in (("c", "*.c"), ("python", "*.py"), ("c", "*.txt"), ("python", "*.txt")):
            for path in sorted(glob.glob(os.path.join(HERE, track, pattern))):
                name = os.path.relpath(path, HERE)
                count += 1
                fields = header(path)
                problems = []

                for required in ("TITLE", "GROUP", "SUMMARY"):
                    if not fields.get(required):
                        problems.append("header lacks " + required)
                if fields.get("GROUP", "").startswith("Header reference"):
                    for required in ("STANDARD", "PROVIDES", "TOOL", "SEE"):
                        if not fields.get(required):
                            problems.append("header reference page lacks " + required)
                title = (track, fields.get("TITLE"))
                if title in titles:
                    problems.append("same TITLE as " + titles[title])
                titles[title] = name

                if path.endswith(".c"):
                    program = os.path.join(scratch, "program")
                    libs = fields.get("LIBS", "").split()
                    build = subprocess.run(["cc", "-std=gnu11", "-Wall", "-Wextra", "-o", program, path] + libs,
                                           capture_output=True, text=True)
                    if build.returncode != 0 or build.stderr.strip():
                        problems.append("build problem:\n" + build.stderr)
                    else:
                        command = [program]
                elif path.endswith(".py"):
                    command = [sys.executable, "-W", "error", path]
                else:
                    command = None

                if command and not problems:
                    run = subprocess.run(command, capture_output=True, text=True, timeout=30, stdin=subprocess.DEVNULL)
                    if run.returncode != 0:
                        problems.append(f"exit status {run.returncode}: {run.stderr.strip()[:200]}")
                    elif not run.stdout.strip():
                        problems.append("printed nothing")
                    elif run.stderr.strip() and not any(word in open(path, encoding="utf-8").read()
                                                          for word in ("stderr", "perror(")):
                        problems.append("unexpected stderr: " + run.stderr.strip()[:200])

                print(("FAIL  " if problems else "ok    ") + name)
                for problem in problems:
                    print("      " + problem)
                if problems:
                    failures.append(name)

    # Every C header the wiki promises must have a page of its own (or an
    # extended page) titled "<header>: ..." that includes it, in the C track.
    for header_name in C_HEADERS:
        match = [name for (track, title), name in titles.items()
                 if track == "c" and title and title.split(":")[0].startswith(header_name)]
        source = open(os.path.join(HERE, match[0]), encoding="utf-8").read() if match else ""
        if not match:
            print("FAIL  no C page is titled for " + header_name)
            failures.append(header_name)
        elif "#include <" + header_name.replace("/", "/") + ">" not in source:
            print(f"FAIL  {match[0]} does not include <{header_name}>")
            failures.append(header_name)
    print(f"{len(C_HEADERS)} C headers each have a page")

    if count == 0:
        sys.exit("no pages found")
    print(f"{count - len(failures)}/{count} pages ok")
    sys.exit(1 if failures else 0)


if __name__ == "__main__":
    main()
