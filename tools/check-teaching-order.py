#!/usr/bin/env python3
"""Check that no exercise needs something its course has not taught yet.

A task that needs `if` before module 4 has taught `if` is a broken task, not
a hard one. This has slipped through before, caught by a student rather than
by the author, so it is checked here instead of remembered.

Run from the repo root, checks every course:

    python3 tools/check-teaching-order.py

Exits 0 when everything is clean, 1 when something reaches ahead.
"""

import pathlib
import re
import sys

# One entry per course. `first_taught` maps a construct's regex to the
# module that introduces it; `glob` is the module source files, relative to
# the course directory; `exercise_marker` / `summary_marker` bound one
# module's exercise section in that language's source.
#
# Patterns are regexes so that `for (` does not match the English word
# "for". 99 means the course never teaches it at all: using it in an
# exercise is always wrong, whatever the module.
COURSES = {
    "c": {
        "dir": "c/src",
        "glob": "lessons_*.c",
        "exercise_marker": r'exercise\((\d+)\);',
        "summary_marker": r'\n    wait_enter\(\);\n    clear_screen\(\);\n    heading\("SUMMARY',
        "first_taught": [
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
            # Never introduced by any module.
            (99, r"\bputchar\s*\("),
            (99, r"\bstrcspn\s*\("),
            (99, r"\bgetchar\s*\("),
            (99, r"\bputs\s*\("),
            (99, r"\bsprintf\s*\("),
            (99, r"\bmemcpy\s*\("),
            (99, r"\bstrcat\s*\("),
            (99, r"\bstrncpy\s*\("),
            (99, r"\bswitch\s*\("),
            (99, r"\bgoto\b"),
            (99, r"\btypedef\b"),
            (99, r"\bunion\b"),
        ],
        # Module 11 (the final test) has no exercise() call: its tasks are
        # interactive and depend on a random number, so they cannot state a
        # single matching output the way every other challenge() does, and
        # are presented by hand instead. Verified once, by hand, that its
        # example code uses nothing past module 10 plus rand()/srand(),
        # which it introduces itself before using.
    },
    "cpp": {
        "dir": "cpp/src",
        "glob": "lessons_*.cpp",
        "exercise_marker": r'exercise\((\d+)\);',
        "summary_marker": r'\n    waitEnter\(\);\n    clearScreen\(\);\n    heading\("SUMMARY',
        "first_taught": [
            (1, r"\bstd::cout\b"),
            (1, r"\breturn\b"),
            (2, r"\bstd::string\b"),
            (2, r"\bauto\s+[a-z_]+\s*="),
            (2, r"\bstatic_cast<"),
            (3, r"\bstd::cin\b"),
            (3, r"\bstd::getline\s*\("),
            (4, r"\bif\s*\("),
            (4, r"\belse\b"),
            (4, r"\bbool\b"),
            (5, r"\bfor\s*\("),
            (5, r"\bwhile\s*\("),
            (5, r"for\s*\(\s*(?:const\s+)?auto"),  # range-based for
            # std::vector's declaration syntax appears in module 5's own
            # demo, as something to range-for over -- its method vocabulary
            # (push_back, sort) is what module 8 actually adds.
            (5, r"\bstd::vector<"),
            (6, r"\.substr\s*\("),
            (6, r"\.find\s*\("),
            (6, r"\bnpos\b"),
            (6, r"\.at\s*\("),
            (6, r"\btry\b"),
            (6, r"\bcatch\s*\("),
            # A reference parameter: a known type token directly before &.
            # A bare "&\s*[a-z_]+[,)]" also matches English prose like
            # "does this need &age, like scanf did?" -- requiring the type
            # keyword is what tells the two apart.
            (7, r"\b(?:int|double|float|bool|char|std::string)\s*&\s*[a-z_]+\s*[,)]"),
            (7, r"\bconst\s+std::string\s*&"),
            (8, r"\.push_back\s*\("),
            (8, r"\bstd::sort\s*\("),
            (9, r"\bclass\s+[A-Z]"),
            (9, r"\bprivate\s*:"),
            (9, r"\bpublic\s*:"),
            # Require what follows to look like an actual new-expression
            # (a type name then ( or [), not the English word "new" --
            # module 6's own solution text says "building a new string".
            (9, r"\bnew\s+[A-Za-z_][A-Za-z0-9_:<>]*\s*[(\[]"),
            (9, r"\bdelete(?:\[\])?\s+[a-z_]"),
            # Never introduced by any module.
            (99, r"\btemplate\s*<"),
            (99, r"\bstd::unique_ptr<"),
            (99, r"\bstd::shared_ptr<"),
            (99, r"\bstd::map<"),
            (99, r"\boperator\s*[a-zA-Z+\-*/=<>]"),
            (99, r"\bvirtual\b"),
            (99, r"\bstd::move\s*\("),
        ],
        # Module 9's own lesson body constructs a Student with
        # `name_(std::move(n))` -- inside the lesson code itself, not inside
        # an exercise() block, so it is outside what this check scans. That
        # is deliberate: the constant is student-facing prose and code
        # shown to the reader either way; std::move is not something an
        # exercise ever requires the student to write.
    },
    "python": {
        "dir": "python/src",
        "glob": "lessons_*.py",
        "exercise_marker": r"ui\.exercise\((\d+)\)",
        "summary_marker": r'\n    ui\.wait_enter\(\)\n    ui\.clear_screen\(\)\n    ui\.heading\("SUMMARY',
        "first_taught": [
            (1, r"\bprint\s*\("),
            (2, r"\btype\s*\("),
            (2, r'f"'),
            (2, r"//"),
            (3, r"\binput\s*\("),
            (3, r"\bint\s*\(\s*input"),
            (4, r"\bif\b"),
            (4, r"\belif\b"),
            (4, r"\belse\b"),
            (5, r"\bfor\s+[a-z_]+\s+in\b"),
            (5, r"\bwhile\b"),
            (5, r"\brange\s*\("),
            (5, r"\benumerate\s*\("),
            # A real slice: an identifier directly against [, then only
            # digits/word-chars/:/-/* inside -- no spaces, no quotes. A bare
            # "\[.*:.*\]" also matches a list of strings where one string's
            # TEXT happens to contain a colon, e.g. ["Number: -5", ...].
            (6, r"[a-zA-Z_]\w*\[[\w:+*-]*:[\w:+*-]*\]"),
            (6, r"\.sort\s*\("),
            (6, r"\bsorted\s*\("),
            (6, r"\.append\s*\("),
            (7, r"\bdef\s+[a-z_]+\s*\("),
            (7, r"\blambda\b"),
            (7, r"\*args\b"),
            (7, r"\*\*kwargs\b"),
            (8, r"\.get\s*\("),
            (8, r"\.items\s*\("),
            (8, r"\.setdefault\s*\("),
            (9, r"\bclass\s+[A-Z]"),
            (9, r"\bself\b"),
            (9, r"\bsuper\s*\("),
            (10, r"\btry\s*:"),
            (10, r"\bexcept\b"),
            (10, r"\braise\b"),
            (10, r"\bfinally\s*:"),
            # Never introduced by any module.
            (99, r"\bwith\b.*\bas\b"),
            (99, r"\byield\b"),
            (99, r"\basync\b"),
            (99, r"\bawait\b"),
            (99, r"\bmatch\b.*:"),
            (99, r"@\w+\n"),  # a decorator
            (99, r":=\b"),  # walrus operator
        ],
        # A dict literal ({"a": 1}) has no regex here distinct enough from
        # an ordinary set/block of code to be worth writing -- module 8 is
        # the only place one is ever needed in a solution, so there is
        # nothing earlier for a missing rule to fail to catch.
    },
}


def exercise_blocks(source, exercise_marker, summary_marker):
    """Yield (module_number, text_of_its_exercise) for every module in source."""
    pattern = re.compile(
        exercise_marker + r"(.*?)(?=" + summary_marker + r"|\Z)", re.S
    )
    for match in pattern.finditer(source):
        yield int(match.group(1)), match.group(2)


def check_course(name, config, repo_root):
    src_dir = repo_root / config["dir"]
    files = sorted(src_dir.glob(config["glob"]))
    if not files:
        return 0, 0

    source = "".join(f.read_text() for f in files)
    problems = 0
    checked = 0

    for module, block in exercise_blocks(
        source, config["exercise_marker"], config["summary_marker"]
    ):
        checked += 1
        for taught_in, pattern in config["first_taught"]:
            if taught_in <= module:
                continue
            if re.search(pattern, block):
                reason = (
                    "never taught by any module"
                    if taught_in == 99
                    else f"first taught in module {taught_in}"
                )
                print(f"{name}: module {module:2d}  /{pattern}/  -- {reason}")
                problems += 1

    return problems, checked


def main():
    repo_root = pathlib.Path(__file__).resolve().parent.parent
    total_problems = 0
    total_checked = 0

    for name, config in COURSES.items():
        problems, checked = check_course(name, config, repo_root)
        total_problems += problems
        total_checked += checked

    if total_problems:
        print(f"\n{total_problems} exercise(s) need something not taught yet.")
        return 1

    print(f"All exercises ({total_checked} checked) use only what their module has already taught.")
    return 0


if __name__ == "__main__":
    sys.exit(main())
