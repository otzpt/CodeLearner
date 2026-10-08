#!/usr/bin/env python3
"""Walk every module of the Arduino course and check it runs to its SUMMARY.

A plain `printf ... | ./arduino-course` pre-fills the whole pipe, and a
sketch that polls Serial.available() would swallow input meant for a later
prompt. So this feeds one line at a time, only once the course has stopped
printing and is waiting -- the way a person types.

    make && python3 check-course.py [module numbers...]

Exits 0 when every module printed SUMMARY and the course quit cleanly.
"""

import os
import re
import select
import subprocess
import sys

BINARY = os.path.join(os.path.dirname(os.path.abspath(__file__)), "arduino-course")

# What to type at a prompt, matched against the end of what the course has
# printed so far. First match wins; anything not listed gets an empty line.
RULES = [
    (r"Type a short word and press ENTER: $", "hi"),
    (r"Type a whole number and press ENTER: $", "42"),
    (r"Type abc and press ENTER: $", "abc"),
    (r"Type f, s or anything else, then ENTER: $", "f"),
    (r"Type a command[^:]*: $", "go"),
    (r"\(y/N\): $", "y"),
    (r"Your answer: $", "x"),
]


def run_module(number, idle_seconds=0.4, overall_seconds=60):
    process = subprocess.Popen(
        [BINARY],
        stdin=subprocess.PIPE,
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
    )
    output = b""
    sent_module = False
    sent_quit = False
    answered_at = -1
    elapsed = 0.0

    while process.poll() is None and elapsed < overall_seconds:
        ready, _, _ = select.select([process.stdout], [], [], idle_seconds)
        if ready:
            chunk = os.read(process.stdout.fileno(), 65536)
            if not chunk:
                break
            output += chunk
            elapsed = 0.0
            continue

        elapsed += idle_seconds
        if len(output) == answered_at:
            continue  # still inside the call that was just answered (a Serial timeout)
        answered_at = len(output)
        text = output.decode("utf-8", "replace")
        if text.endswith("Pick a module: "):
            if not sent_module:
                line, sent_module = str(number), True
            else:
                line, sent_quit = "0", True
        else:
            line = ""
            for pattern, answer in RULES:
                if re.search(pattern, text):
                    line = answer
                    break
        process.stdin.write((line + "\n").encode())
        process.stdin.flush()

    try:
        process.stdin.close()
    except BrokenPipeError:
        pass
    try:
        code = process.wait(timeout=5)
    except subprocess.TimeoutExpired:
        process.kill()
        code = -1
    return output.decode("utf-8", "replace"), code, sent_quit


def main():
    if not os.path.exists(BINARY):
        sys.exit("build first: make")

    wanted = [int(n) for n in sys.argv[1:]]
    if not wanted:
        listing, _, _ = run_module(0)
        wanted = list(range(1, len(re.findall(r"^\s+\[\s*\d+\]", listing, re.M))))

    failed = []
    for number in wanted:
        text, code, quit_cleanly = run_module(number)
        ok = "SUMMARY" in text and code == 0 and quit_cleanly
        print(f"module {number:2d}: {'ok' if ok else 'FAILED'}")
        if not ok:
            failed.append(number)
            print("  exit code", code, "| quit cleanly:", quit_cleanly)
            print("  last output:", repr(text[-300:]))

    sys.exit(1 if failed else 0)


if __name__ == "__main__":
    main()
