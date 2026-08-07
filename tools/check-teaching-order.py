#!/usr/bin/env python3
"""Check that no exercise needs something the course has not taught yet.

A task that needs `if` before module 4 has taught `if` is a broken task, not
a hard one. This has slipped through twice, both times caught by a student
rather than by me, so it is checked here instead of remembered.

Run from the repo root:

    python3 tools/check-teaching-order.py

Exits 0 when clean, 1 when something is out of order.
"""

import pathlib
import re
import sys

# The first module that teaches each construct, and the pattern that finds it.
# Patterns are regexes so that `for (` does not match the English word "for".
# 99 means the course never teaches it: using it in an exercise is always
# wrong, whatever the module.
FIRST_TAUGHT = [
    (1, r"\bprintf\s*\("),
    (1, r"\breturn\b"),
    (1, r"\bint main\s*\(void\)"),
    (2, r"\bint\s+[a-z_]+\s*[;=,)]"),
    (2, r"\bfloat\b"),
    (2, r"\bchar\s+[a-z_]+\s*[;=\[]"),
    (2, r"\bsizeof\b"),
    (2, r"\(float\)"),
    (2, r"%[dfc]\b"),
    (2, r"%s\b"),
    (3, r"\bscanf\s*\("),
    (3, r"\bfgets\s*\("),
    (4, r"\bif\s*\("),
    (4, r"\belse\b"),
    (5, r"\bfor\s*\("),
    (5, r"\bwhile\s*\("),
    (5, r"\bbreak\b"),
    (5, r"\bcontinue\b"),
    (6, r"\bstrlen\s*\("),
    (6, r"\bstrcmp\s*\("),
    (8, r"\bNULL\b"),
    (9, r"\bmalloc\s*\("),
    (9, r"\bfree\s*\("),
    (10, r"\bstruct\b"),
    (10, r"\bstrcpy\s*\("),
    (10, r"->"),
    (11, r"\brand\s*\("),
    (11, r"\bsrand\s*\("),
    (12, r"\brealloc\s*\("),
    (12, r"\bcalloc\s*\("),
    # Never introduced by any module. If an exercise needs one of these,
    # either teach it first or rewrite the exercise.
    (99, r"\bputchar\s*\("),
    (99, r"\bstrcspn\s*\("),
    (99, r"\bgetchar\s*\("),
    (99, r"\bputs\s*\("),
    (99, r"\bsprintf\s*\("),
    (99, r"\bcalloc\s*\("),
    (99, r"\brealloc\s*\("),
    (99, r"\bmemcpy\s*\("),
    (99, r"\bstrcat\s*\("),
    (99, r"\bstrncpy\s*\("),
    (99, r"\bswitch\s*\("),
    (99, r"\bgoto\b"),
    (99, r"\btypedef\b"),
    (99, r"\bunion\b"),
]


def exercise_blocks(source):
    """Yield (module_number, text_of_its_exercise) for every module.

    Module 11 (the final test) has no exercise() call: its two tasks are
    interactive and depend on a random number, so they cannot state a single
    matching output the way every other challenge() does, and are presented
    by hand instead. It is therefore not covered by this check -- verified
    once, by hand, that its example code (guess.c, grades.c) uses nothing
    past module 10 plus rand()/srand(), which it introduces itself before
    using.
    """
    pattern = re.compile(
        r'exercise\((\d+)\);(.*?)(?=\n    wait_enter\(\);\n    clear_screen\(\);'
        r'\n    heading\("SUMMARY|\Z)',
        re.S,
    )
    for match in pattern.finditer(source):
        yield int(match.group(1)), match.group(2)


def solution_of(block):
    found = re.search(r'const char \*solution\[\] = \{(.*?)\n        \};', block, re.S)
    return found.group(1) if found else ""


def questions_of(block):
    return " ".join(re.findall(r"question\((.*?)\);", block, re.S))


def main():
    src_dir = pathlib.Path(__file__).resolve().parent.parent / "c" / "src"
    source = "".join(f.read_text() for f in sorted(src_dir.glob("lessons_*.c")))

    problems = 0

    for module, block in exercise_blocks(source):
        for label, text in (("solution", solution_of(block)),
                            ("question", questions_of(block))):
            for taught_in, pattern in FIRST_TAUGHT:
                if taught_in <= module:
                    continue
                if re.search(pattern, text):
                    reason = ("never taught by any module"
                              if taught_in == 99
                              else f"first taught in module {taught_in}")
                    print(f"module {module:2d}  {label}: /{pattern}/  -- {reason}")
                    problems += 1

    if problems:
        print(f"\n{problems} exercise(s) need something not taught yet.")
        return 1

    print(f"All exercises use only what their module has already taught.")
    return 0


if __name__ == "__main__":
    sys.exit(main())
